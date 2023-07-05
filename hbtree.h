#ifndef __HBTREE__
#define __HBTREE__

#define EDGES_NUM 3

#define PARENT 0
#define LEFT 1
#define RIGHT 2 

struct hbtree_node{
	struct hbtree_node *edges[EDGES_NUM];// 0=PARENT ,1=LEFT ,2=RIGHT
	int key;
    unsigned short height;
    unsigned weight; //how many nodes has below it
};
typedef struct hbtree_node hbtree_node; 

//unitialised node
hbtree_node* _hbtree_node_create();

void hbtree_node_init(hbtree_node *junk_node);

//initialised node
hbtree_node* hbtree_node_create();

//initialised node (with key)
hbtree_node* hbtree_node_create_wk(unsigned key);


void make_right_child(hbtree_node *parent,hbtree_node *child);

void make_left_child(hbtree_node *parent,hbtree_node *child);

hbtree_node* search_bst(hbtree_node *root ,unsigned key);

void preorder_print(hbtree_node *root);


unsigned hbtree_node_lvl_calc(hbtree_node *node);

short hbtree_height_calc(hbtree_node *node);

short _hbtree_height_calc(hbtree_node *root,unsigned height);

short hbtree_node_height(hbtree_node *root);

short hbtree_relative_height_calc(hbtree_node *root);

unsigned hbtree_node_weight(hbtree_node *root);

unsigned hbtree_relative_weight_calc(hbtree_node *node);


hbtree_node* left_rotation_avl(hbtree_node *node);

hbtree_node* right_rotation_avl(hbtree_node *node);

hbtree_node* double_right_rotation_avl(hbtree_node *node);

hbtree_node* double_left_rotation_avl(hbtree_node *node);

void hbtree_rebalance_child_nodes(hbtree_node *curr);

void hbtree_rebalance_node(hbtree_node *curr);

short add2_avl_hbtree(hbtree_node **root ,hbtree_node *new);

hbtree_node* hbtree_node_detach(hbtree_node **root,hbtree_node *detachable);
hbtree_node* tree_node_detach(hbtree_node **root,hbtree_node *detachable);
hbtree_node* _tree_node_detach(hbtree_node *node);


void tree_delete(hbtree_node *root);

#endif