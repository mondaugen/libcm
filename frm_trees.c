/* Generate trees to be used as forms */

#include <cm/frm_trees.h> 
#include <stdlib.h>

FormTreeNode *
cm_ftn_make_ftn(char val, FormTreeNode *parent, FormTreeNode **children, 
		int num_children)
{
    FormTreeNode *t = (FormTreeNode*)malloc(sizeof(FormTreeNode));
    t->val = val;
    t->parent = parent;
    t->children = children;
    t->num_children = num_children;
    return t;
}

void
cm_ftn_free_tree(FormTreeNode *parent)
{
    if( parent != NULL){
	if( parent->num_children > 0 ){
	    int i = 0;
	    for(i = 0; i < parent->num_children; i++)
		cm_ftn_free_tree((parent->children)[i]);
	}
	free(parent);
    }
}

/* pfn is a function that accepts an integer and returns a char. The default
 * provided here takes an integer i and returns the char 'a' + i. numsects is
 * the number of different sections whereas seqlen is the total length of the
 * sequence, so for example if numsects is 3 and seqlen is 8, a possible
 * sequence could be abaabcba (sections can be one of 3: a,b, or c)*/
char
cm_ftn_dflt_node_namer( int i )
{
    return 'a' + i;
}

FormTreeNode *
cm_ftn_make_form_tree( char (*pfn)(int), char val, FormTreeNode *parent,
	int seqlen, int numsects, int depth )
{
    if( depth > 1 ){
	FormTreeNode **newchildren = 
	    (FormTreeNode**)malloc(sizeof(FormTreeNode*)*seqlen);
	FormTreeNode **ptnlchilds = 
	    (FormTreeNode **)malloc(sizeof(FormTreeNode*)*numsects);
	FormTreeNode *rslt = cm_ftn_make_ftn( val, parent, newchildren, seqlen );
	int i ;
	for( i = 0 ; i < numsects ; i++ )
	    ptnlchilds[i] = 
		cm_ftn_make_form_tree(pfn, (*pfn)(i), rslt, seqlen, numsects,
					depth - 1);
	for( i = 0 ; i < seqlen ; i++ )
	    newchildren[i] = ptnlchilds[ cm_ftn_random() % numsects ];
	return rslt;
    }else{
	return cm_ftn_make_ftn(val, parent, NULL, 0 ) ;
    }
}

int
cm_ftn_get_depth(FormTreeNode *t)
{
    if(t == NULL)
	return 0;
    int depth = 0;
    do{
	depth++;
	t = t->parent;
    }while(t != NULL);
    return depth;
}

char *
cm_ftn_print_ftn_string(FormTreeNode *t)
{
    int i = cm_ftn_get_depth(t);
    char *rslt = (char*)calloc(i+1, sizeof(char));
    char *ptr = rslt;
    do{
	*(ptr++) = t->val;
	t = t->parent;
    }while(t != NULL);
    return rslt;
}

int
cm_ftn_get_num_leaves(FormTreeNode *t)
{
    if(t->num_children == 0)
	return 1;
    int sum = 0;
    int i;
    for(i = 0; i < t->num_children; i++)
	sum += cm_ftn_get_num_leaves((t->children)[i]);
    return sum;
}

static void
cm_ftn_print_leaf_string(FormTreeNode *t, char ***dest)
{
    if(t->num_children <= 0)
	*(*dest)++ = cm_ftn_print_ftn_string(t);
    else{
	int i;
	for(i = 0; i < t->num_children; i++)
	    cm_ftn_print_leaf_string((t->children)[i], dest);
    }
}

char **
cm_ftn_get_all_leaf_strings(FormTreeNode *t)
{
    char **rslt = (char**)calloc(cm_ftn_get_num_leaves(t)+1, sizeof(char*));
    char **ptr = rslt;
    cm_ftn_print_leaf_string(t, &ptr);
    return rslt;
}

