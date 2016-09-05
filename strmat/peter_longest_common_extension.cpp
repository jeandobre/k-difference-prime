/*
 * This is based on Gusfield but http://homepage.usask.ca/~ctl271/810/approximate_matching.shtml
 *  has a nice overview of the same Gusfield text behind the code I copied.
 */

#include <iostream>
#include <string>
#include <map>
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "strmat.h"
#include "strmat_print.h"
#include "strmat_util.h"
#include "stree_ukkonen.h"
#include "stree_lca.h"
#include "peter_longest_common_extension.h"

using namespace std;

/*
 * compute_nodemap
 *
 * Compute the mapping from identifiers to suffix tree nodes and store
 * that mapping in "nodemap"
 *
 * Parameters:  tree     -  a suffix tree
 *              node     -  a suffix tree node
 *              nodemap  -  the nodemap being computed
 */

 /*
static void compute_nodemap(SUFFIX_TREE tree, STREE_NODE node, STREE_NODE *nodemap)
{
    int ident = stree_get_ident(tree, node);
    nodemap[ident] = node;
  //  cout << "nodemap[" << ident << "] isaleaf=" << node->isaleaf << ",leaves=" << node->leaves << endl;

    for (STREE_NODE child = stree_get_children(tree, node); child; child = stree_get_next(tree, child)) {
        compute_nodemap(tree, child, nodemap);
    }
}
*/
static int compute_depth(SUFFIX_TREE tree, STREE_NODE node){
   //if(node->edgelen > 1) return node->edgelen;
   return get_label_len(node);
}

static void compute_node_map(SUFFIX_TREE tree, STREE_NODE node, STREE_NODE *nodemap){

    int pos, leafnum, index, vr;
  //  CHAR_TYPE *edgestr, buffer[34];
    STREE_NODE child;
    CHAR_TYPE *str;

    leafnum = 1;
    node->depth = compute_depth(tree, node);
    while (stree_get_leaf(tree, node, leafnum, &str, &pos, &index)) {
        vr = (index == 2 ? 9 : 0) + pos;
        nodemap[vr] = node;
        leafnum++;
    }
/*
    if(!node->isaleaf)
       for (STREE_INTLEAF leaf = node->leaves; leaf; leaf = leaf->next) {
         //cout<<leaf<<":"<<leaf->strid<<"-"<<leaf->pos<<endl;
          vr = (leaf->strid == 1 ? 9 : 0) + leaf->pos;
          nodemap[vr] = (STREE_NODE) leaf;
       }*/

    child = stree_get_children(tree, node);
    while (child != NULL) {
        leafnum = 1;
        child->depth = compute_depth(tree, child);
        while (stree_get_leaf(tree, child, leafnum, &str, &pos, &index)) {
            vr = (index == 2 ? 9 : 0) + pos;
            nodemap[vr] = child;
            leafnum++;
        }

      /*  if(!child->isaleaf)
          for (STREE_INTLEAF leaf = child->leaves; leaf; leaf = leaf->next) {
            //cout<<leaf<<":"<<leaf->strid<<"-"<<leaf->pos<<endl;
             vr = (leaf->strid == 1 ? 9 : 0) + leaf->pos;
             nodemap[vr] = (STREE_NODE) leaf;
          }*/

        child = stree_get_next(tree, child);
    }

    child = stree_get_children(tree, node);
    while (child != NULL) {
        child->depth = compute_depth(tree, child);
        if (stree_get_num_children(tree, child) > 0) {
            compute_node_map(tree, child, nodemap);
        }
        child = stree_get_next(tree, child);
    }
}

/*
static map<int, map<int, int>> compute_suffix_node_map(SUFFIX_TREE tree, STREE_NODE *nodemap)
{
    map<int, map<int, int>> suffix_node_map;
    suffix_node_map[0] = map<int, int>();
    suffix_node_map[1] = map<int, int>();

    int num_nodes = (int)stree_get_num_nodes(tree);

    for (int i = 0; i < num_nodes; i++) {
        STREE_NODE node = nodemap[i];

        cout << " Node id=" << node->id;
        if (node->isaleaf)  {
       //     cout << ", pos=" << node->edgelen << endl;
           // suffix_node_map[node->strid-1][leaf->pos] = node->id;
            continue;
        }
        cout << ", Leaves:" << endl;
        int leafnum;
        int pos, index;
        CHAR_TYPE *str;
        for (STREE_INTLEAF leaf = node->leaves; leaf; leaf = leaf->next) {
            cout << "leaf=" << leaf;
            cout << "    strid=" << leaf->strid << ",pos=" << leaf->pos << endl;
            suffix_node_map[leaf->strid-1][leaf->pos] = node->id;
            leafnum = 1;
            //child = stree_get_children(tree, leaf);
/*            while(stree_get_leaf(tree, leaf, leafnum, &str, &pos, &index)){
               printf("%d:%d", index, pos + 1);
               leafnum++;
            }
        }
    }

    return suffix_node_map;
}
*/

LCE *prepare_longest_common_extension(const STRING *s1, const STRING *s2, bool print_stats){
     assert(s1 && s2);

    // Build the suffix tree.
    bool ok = true;
    int num_strings = 2;
    const STRING *strings[2];
    strings[0] = s1;
    strings[1] = s2;
    SUFFIX_TREE tree = stree_gen_ukkonen_build(strings, num_strings, &ok, print_stats);
    if (!tree) {
        return false;
    }

    if(print_stats){
      printf("\nSuffix Tree:\n");
      small_print_tree(tree, stree_get_root(tree), 0, 2);
     // large_print_tree(tree, stree_get_root(tree), 0);
    }
    // Build the LCA tables
    LCE *lce = (LCE *)my_calloc(sizeof(LCE),1);
    lce->_lca = lca_prep(tree);

     // Build the map of suffix tree nodes.
    lce->_nodemap = (STREE_NODE *)my_calloc(21, sizeof(STREE_NODE));
    compute_node_map(tree, stree_get_root(tree), lce->_nodemap);

    return lce;
}

 // Free everything allocated.
void longest_common_extension_free(LCE *lce)
{
    stree_delete_tree(lce->_lca->tree);
    lca_free(lce->_lca);
    free(lce->_nodemap);
    free(lce);
}

static bool print_labels = true;
/*
 * Return longest common extension of
 *      offset ofs1 into string lce->s1
 *      offset ofs2 into string lce->s2
 */
STREE_NODE lookup_lce(LCE *lce, int ofs1, int ofs2){

    STREE_NODE x = lce->_nodemap[ofs1];
    STREE_NODE y = lce->_nodemap[ofs2];

    if(x->id == y->id) return x;

    STREE_NODE z = lca_lookup(lce->_lca, x,  y);
    return z;
}

int lookup(LCE *lce, int ofs1, int ofs2){

    STREE_NODE x = lce->_nodemap[ofs1];
    STREE_NODE y = lce->_nodemap[ofs2];

    if(x->id == y->id) return x->depth;

    STREE_NODE z = lca_lookup(lce->_lca, x,  y);
    return z->depth;
}

int get_label_len(STREE_NODE node){
    int len = 0;
    while (node) {
        len += node->edgelen;
        node = node->parent;
    }
    return len;
}

SubString find_longest_palindrome(const STRING *s, bool print_stats)
{
    STRING *r = make_seqn("reverse", s->sequence, s->length, false);
    int m = s->length;
    for (int i = 0; i < m; i++) {
        r->sequence[i] = s->sequence[m-1-i];
    }

    LCE *lce = prepare_longest_common_extension(s, r, print_stats);

    STREE_NODE longest_node = 0;
    int longest_radius = -1;
    int longest_center = -1;

    for (int i = 0; i < m-1; i++) {
        cout << i << ": ";
        STREE_NODE node = lookup_lce(lce, i, m-2-i);
        int len = get_label_len(node);
        if (len > longest_radius) {
            longest_node = node;
            longest_radius = len;
            longest_center = i;
        }
    }

    longest_common_extension_free(lce);
    free_seq(r);
    return SubString(longest_center - longest_radius, longest_radius * 2);
}
