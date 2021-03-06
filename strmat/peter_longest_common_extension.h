#ifndef _PETER_LONGEST_COMMON_EXTENSIONL_H_
#define _PETER_LONGEST_COMMON_EXTENSIONL_H_

#include <map>

struct LCA_STRUCT;
struct STRING;
//struct STREE_NODE;
struct stree_node;

struct LCE {
    LCA_STRUCT *_lca;
    stree_node **_nodemap;
   // std::map<int, std::map<int, int>> _suffix_node_map;
    STRING *_s1;
    STRING *_s2;
};
//Preparacao para LCE (Modificado por Jean)
int get_label_len(STREE_NODE node);
int lookup(LCE *lce, int ofs1, int ofs2);
static int compute_depth(SUFFIX_TREE tree, STREE_NODE node);
static void compute_node_map(SUFFIX_TREE tree, STREE_NODE node, STREE_NODE *nodemap, int half);
LCE *prepare_longest_common_extension(const STRING *s1, const STRING *s2, bool print_stats);

void longest_common_extension_free(LCE *lce);
/*
 * Return longest common extension of
 *      offset ofs1 into string lce->s1
 *      offset ofs2 into string lce->s2
 */
STREE_NODE lookup_lce(LCE *lce, int ofs1, int ofs2);

struct SubString
{
    const int _offset;
    const int _length;
    SubString(int offset, int length) : _offset(offset), _length(length) {}
};

SubString find_longest_palindrome(const STRING *s, bool print_stats);


#endif // #define _PETER_LONGEST_COMMON_EXTENSIONL_H_
