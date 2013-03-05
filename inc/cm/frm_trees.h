#ifndef FRM_TREES_H
#define FRM_TREES_H

#undef __BEGIN_DECLS
#undef __END_DECLS
#ifdef __cplusplus
# define  __BEGIN_DECLS extern "C" {
# define  __END_DECLS }
#else  
# define  __BEGIN_DECLS /* empty */
# define  __END_DECLS /* empty */
#endif

__BEGIN_DECLS

/* A different random can be defined later. Must return like rand() of C
 * standard library */
#define cm_ftn_random() random() 
#define cm_ftn_srandom(n) srandom(n)

typedef struct __ftreenode {
    char val;
    struct __ftreenode *parent;
    struct __ftreenode **children;
    int num_children;
} FormTreeNode;

FormTreeNode *
cm_ftn_make_ftn(char val, FormTreeNode *parent, FormTreeNode **children, 
		int num_children);

/* pfn is a function that accepts an integer and returns a char. The default
 * provided here takes an integer i and returns the char 'a' + i. numsects is
 * the number of different sections whereas seqlen is the total length of the
 * sequence, so for example if numsects is 3 and seqlen is 8, a possible
 * sequence could be abaabcba (sections can be one of 3: a,b, or c)*/
char
cm_ftn_dflt_node_namer( int i );

FormTreeNode *
cm_ftn_make_form_tree( char (*pfn)(int), char val, FormTreeNode *parent,
	int seqlen, int numsects, int depth );

int
cm_ftn_get_depth(FormTreeNode *t);

char *
cm_ftn_print_ftn_string(FormTreeNode *t);

int
cm_ftn_get_num_leaves(FormTreeNode *t);

char **
cm_ftn_get_all_leaf_strings(FormTreeNode *t);

__END_DECLS

#endif /* FRM_TREES_H */
