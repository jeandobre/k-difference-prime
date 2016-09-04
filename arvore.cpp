
#include <string.h>

#include "strmat/peter_longest_common_extension.h"
#include "strmat/stree_lca.h"
#include <iostream>
#include <stdio.h>
#include "classes.h"


//peter_longest_common_extension.cpp
//#include "strmat.h"
//#include "stree_ukkonen.h"
//#include "stree_lca.h"


using namespace std;

char *get_char_array_1(const char *cstring, int length, char *buffer)
{
    int i;

    buffer[0] = 0;
    for (i = 0; i < length; i++) {
        char s[20];
        sprintf(s, "%c%02x",  cstring[i] > 127 ? '.' : ',', (int)cstring[i]);
        if (strlen(buffer) + strlen(s) + 1 >= CHAR_BUFFER_LEN) {
            break;
        }
        strcat(buffer, s);
    }
    return buffer;
}

STRING *make_seqn(const char *title, const char *cstring, int length, bool print_flag)
{
    STRING *sptr;
    char *sequence;
    char buffer[CHAR_BUFFER_LEN];

    if (print_flag) {
        printf("make_seqn('%s', %d, %s)\n", title, length, get_char_array_1(cstring, length, buffer));
    }

    if ((sequence = (char *) calloc(length, sizeof(char))) == NULL) {
        fprintf(stderr, "Ran out of memory. Unable to add new sequence.\n");
        return NULL;
    }

    memcpy(sequence, cstring, length * sizeof(char));

    if ((sptr = (STRING *) calloc(sizeof(STRING), 1)) == NULL) {
        free(sequence);
        return NULL;
    }

    sptr->sequence = sequence;
    sptr->length = length;
    strcpy(sptr->title, title);

    return sptr;
}

static const STRING **make_test_strings(int num_strings, int num_unique, int length, int max_char)
{
    STRING **strings = (STRING **) calloc(num_strings, sizeof(STRING *));
    char *cstring = (char *) calloc(length, sizeof(char));
    char title[129];

    printf("make_test_strings(num_strings=%d, num_unique=%d, length=%d, max_char=%d)\n",
        num_strings, num_unique, length, max_char);

    for (int i = 0; i < num_strings; i++) {
        sprintf(title, "string %2d", i+1);
        for (int j = 0; j < length; j++) {
            cstring[j] = (j + i % num_unique) % max_char;
        }
        strings[i] = make_seqn(title, cstring, length, false);
    }
    free(cstring);
    return (const STRING **)strings;
}

static void lce_test(int length, int max_char, int num_tests)
{
    const STRING **strings = make_test_strings(2, 2, length, max_char);

    printf("lce_test(length=%d, max_char=%d)\n", length, max_char);
    LCE *lce_ctx = prepare_longest_common_extension(strings[0], strings[1], true);

    int delta = (length + num_tests - 1)/num_tests;
    for (int i = 0; i < num_tests; i++) {
        int ofs1 = ((i+1) * delta) % length;
        int ofs2 = ((i+2) * delta) % length;
        printf("%4d: (ofs1=%3d,ofs2=%3d) ", i, ofs1, ofs2);
        STREE_NODE lce_node = lookup_lce(lce_ctx, ofs1, ofs2);
        cout << " => ";
        print_node(lce_node, "LCE");
        cout << endl;
    }


    longest_common_extension_free(lce_ctx);
//    free_test_strings(2, strings);
}

int main(){
    string alfa = "CCCGGCCC";
    string beta = "CCCGTGCCC";

   //string alfa = lerArquivo("dados/alfa_50000.txt");
   //string beta = lerArquivo("dados/beta_50000.txt");
   int m = alfa.length();
   int n = beta.length();

   lce_test(6, 10, 10);
   //LCE *lce_ctx = prepare_longest_common_extension(alfa.c_str(), beta.c_str(), false);
/*
   for(int ofs1 = 0; ofs1 <= m; ofs1++){
     for(int ofs2 = m + 1; ofs2 < m + n + 1; ofs2++){
       // printf("(%3d,%3d) = ", ofs1, ofs2);
        STREE_NODE lce_node = lookup_lce(lce_ctx, ofs1, ofs2);
       // cout << lce_node->edgelen<<endl;
      // cout<<lce_ctx->_suffix_node_map[0][ofs1] <<",";
    //   cout<<lce_ctx->_suffix_node_map[1][ofs2] <<endl;
     }
     }

*/

 //  SUFFIX_TREE tree = stree_new_tree(0);

  // cout<<int_stree_insert_string(tree, cr1, m, 0)<<endl;
  // cout<<stree_add_string(tree, cr2, n, 2);

 //  LCA_STRUCT *estrutura = lca_prep(tree);
   //int num_nodes = stree_get_num_nodes(tree);

 /* cout<<num_nodes - 1<<endl;
   STREE_NODE x, y, z, *nodemap;
   nodemap = new STREE_NODE[num_nodes];
   compute_nodemap(tree, stree_get_root(tree), nodemap);
   x = nodemap[0];
   y = nodemap[9];
   z = lca_lookup(estrutura, x, y);
   cout<<stree_get_ident(tree, z);
*/
//   lca_free(estrutura);
   //stree_delete_tree(tree);

//    longest_common_extension_free(lce_ctx);
    //free_test_strings(2, strings);

   return 1;
}
