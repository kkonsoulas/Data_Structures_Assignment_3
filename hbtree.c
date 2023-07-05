#include "hbtree.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

long max(long num1,long num2){
	return(num1>num2?num1:num2);
}

hbtree_node* _hbtree_node_create(){
	hbtree_node *new = (hbtree_node*) malloc(sizeof(hbtree_node));
	if(new == NULL)
		exit(-1);
	return(new);
}

void hbtree_node_init(hbtree_node *junk_node){
	short i;
	for(i=0 ;i<EDGES_NUM ;i++){
		junk_node->edges[i] = NULL;
	}
	junk_node->height=1;
	junk_node->weight=1;
	//every other init//
	return;
}

hbtree_node* hbtree_node_create(){
	hbtree_node *new = _hbtree_node_create();
	hbtree_node_init(new);
	return(new);
}

hbtree_node* hbtree_node_create_wk(unsigned key){
	hbtree_node *new = _hbtree_node_create();
	hbtree_node_init(new);
	new->key = key;
	return(new);
}

void make_right_child(hbtree_node *parent,hbtree_node *child){
	if(parent == NULL){
		return;
	}
	
	parent->edges[RIGHT] = child;
	if(child ){
		child->edges[PARENT] = parent;
	}
	
}

void make_left_child(hbtree_node *parent,hbtree_node *child){
	if(parent == NULL){
		return;
	}

	parent->edges[LEFT] = child;
	if(child){
		child->edges[PARENT] = parent;
	}
}

hbtree_node* search_bst(hbtree_node *root ,unsigned key){
	
	while(root && root->key != key){
		if(root->key < key){
			root = root->edges[RIGHT];
		}
		else{
			root = root->edges[LEFT];
		}
	}

	return(root);
}

void preorder_print(hbtree_node *root){
	
	if(root == NULL){
		return;
	}
	
	printf("%d ",root->key);
	preorder_print(root->edges[LEFT]);
	preorder_print(root->edges[RIGHT]);
}

unsigned hbtree_node_lvl_calc(hbtree_node *node){//not functioning
	unsigned lvl;
	for(lvl=0 ;node ;lvl++){
		node = node->edges[PARENT];
	}
	return(lvl);
}


short hbtree_height_calc(hbtree_node *root){
	unsigned height=0;
	height = _hbtree_height_calc(root,height);
	return(height);
}

short _hbtree_height_calc(hbtree_node *root,unsigned height){
	if(root == NULL){
		return(height);
	}

	height++;
	unsigned new_height;
	
	new_height = _hbtree_height_calc(root->edges[LEFT],height);
	
	height = _hbtree_height_calc(root->edges[RIGHT],height);
	if(height < new_height){
		height = new_height;
	}
	return(height);

}

short hbtree_node_height(hbtree_node *root){
	if(root){
		return(root->height);
	}
	return(0);
}

short hbtree_relative_height_calc(hbtree_node *root){
	unsigned h1,h2;
	h1 = hbtree_node_height(root->edges[LEFT]);
	h2 = hbtree_node_height(root->edges[RIGHT]);
	return(max(h1,h2)+1);
}

unsigned hbtree_node_weight(hbtree_node *node){	
	if(node){
		return(node->weight);
	}
	return(0);
}

unsigned hbtree_relative_weight_calc(hbtree_node *node){
	unsigned weight=0;
	weight += hbtree_node_weight(node->edges[LEFT]);
	weight += hbtree_node_weight(node->edges[RIGHT]);
	return(weight+1);
}

void hbtree_node_stat_update(hbtree_node *node){
	node->weight = hbtree_relative_weight_calc(node);
	node->height = hbtree_relative_height_calc(node);
	return;
}

hbtree_node* left_rotation_avl(hbtree_node *node){
	hbtree_node *right = node->edges[RIGHT];
	short pos;

	if(node->edges[PARENT]){
		pos = node->edges[PARENT]->edges[LEFT] == node ? LEFT : RIGHT;

		//connect node's PARENT <-> RIGHT
		node->edges[PARENT]->edges[pos] = right;
		right->edges[PARENT] = node->edges[PARENT];
		//
	}
	else{
		right->edges[PARENT] = NULL;
	}
	//connect node's RIGHT <-> right's LEFT and then right <-> node
	make_right_child(node,right->edges[LEFT]);
	make_left_child(right,node);
	//

	// update stats (weight,height)
	hbtree_node_stat_update(node);
	hbtree_node_stat_update(right);
	//
	
	return(right);
}

hbtree_node* right_rotation_avl(hbtree_node *node){
	hbtree_node *left = node->edges[LEFT];
	short pos;
	
	if(node->edges[PARENT]){
		pos = node->edges[PARENT]->edges[LEFT] == node ? LEFT : RIGHT;

		//connect node's PARENT <-> LEFT
		node->edges[PARENT]->edges[pos] = left;
		left->edges[PARENT] = node->edges[PARENT];
		//
	}
	else{
		left->edges[PARENT] = NULL;
	}
	make_left_child(node,left->edges[RIGHT]);
	make_right_child(left,node);

	// update stats (weight,height)
	hbtree_node_stat_update(node);
	hbtree_node_stat_update(left);
	//order is important

	return(left);
}

hbtree_node* double_right_rotation_avl(hbtree_node *node){
	hbtree_node *parent,*left, *l_right;
	parent = node->edges[PARENT];
	left = node->edges[LEFT];
	l_right = node->edges[LEFT]->edges[RIGHT];

	make_left_child(node ,l_right->edges[RIGHT]);
	make_right_child(left ,l_right->edges[LEFT]);

	make_left_child(l_right ,left);
	make_right_child(l_right ,node);
	
	if(parent && parent->edges[LEFT] == node){
		make_left_child(parent,l_right);
	}
	else if(parent){
		make_right_child(parent,l_right);
	}
	else{
		l_right->edges[PARENT] = NULL;
	}

	//update stats (weight,height)
	hbtree_node_stat_update(node);
	hbtree_node_stat_update(left);
	hbtree_node_stat_update(l_right);
	//order is important

	return(l_right);
} 

hbtree_node* double_left_rotation_avl(hbtree_node *node){
	hbtree_node *parent,*right, *r_left;
	parent = node->edges[PARENT];
	right = node->edges[RIGHT];
	r_left = node->edges[RIGHT]->edges[LEFT];

	make_right_child(node ,r_left->edges[LEFT]);
	make_left_child(right ,r_left->edges[RIGHT]);

	make_right_child(r_left ,right);
	make_left_child(r_left ,node);
	
	if(parent && parent->edges[LEFT] == node){
		make_left_child(parent,r_left);
	}
	else if(parent){
		make_right_child(parent,r_left);
	}
	else{
		r_left->edges[PARENT] = NULL;
	}

	//update stats (weight and height)
	hbtree_node_stat_update(node);
	hbtree_node_stat_update(right);
	hbtree_node_stat_update(r_left);
	//order is important

	return(r_left);
} 

void hbtree_rebalance_child_nodes(hbtree_node *curr){
	if(curr==NULL){
		return;
	}
	hbtree_rebalance_child_nodes(curr->edges[LEFT]);
	hbtree_rebalance_child_nodes(curr->edges[RIGHT]);
	hbtree_rebalance_node(curr);
	return;
}

void hbtree_rebalance_node(hbtree_node *curr){
	if(curr == NULL){
		return;
	}
	unsigned short l_height,r_height;
	unsigned the_weight;
		
	//update current node status
	hbtree_node_stat_update(curr);
	//

	l_height = hbtree_node_height(curr->edges[LEFT]);
	r_height = hbtree_node_height(curr->edges[RIGHT]);

	the_weight = curr->weight;
	if(curr->weight<2){// we want to be between max(1,floor(log2(height)))
		the_weight = 2;
	}

	if(abs(l_height - r_height) > floor(log2(the_weight)) ){
		if(l_height > r_height){

			l_height = hbtree_node_height(curr->edges[LEFT]->edges[LEFT]);
			r_height = hbtree_node_height(curr->edges[LEFT]->edges[RIGHT]);
			if(l_height >= r_height){
				curr = right_rotation_avl(curr);
			}
			else{
				curr = double_right_rotation_avl(curr);
			}
			hbtree_rebalance_child_nodes(curr);
		}
		else{
			l_height = hbtree_node_height(curr->edges[RIGHT]->edges[LEFT]);
			r_height = hbtree_node_height(curr->edges[RIGHT]->edges[RIGHT]);
			if(r_height >= l_height){
				curr = left_rotation_avl(curr);
			}
			else{
				curr = double_left_rotation_avl(curr);
			}
			hbtree_rebalance_child_nodes(curr);
		}
	}
}


short add2_avl_hbtree(hbtree_node **root ,hbtree_node *new){

	//if key exists don't add
	if(search_bst(*root,new->key)){
		free(new);
		return(0);
	}

	//if first node add as root
	if(*root == NULL){
		*root = new ;
		return(1);
	}

	hbtree_node *curr = *root;
	short l_height,r_height;
	unsigned the_weight;

	//add to bst
	while(1){
		if(curr->key<new->key){
			if(curr->edges[RIGHT] == NULL){
				make_right_child(curr,new);
				break;
			}
			curr = curr->edges[RIGHT];
		}
		else{
			if(curr->edges[LEFT] == NULL){
				make_left_child(curr,new);
				break;
			}
			curr = curr->edges[LEFT];
		}
	}
	//

	while(curr){

		//update current node status
		hbtree_node_stat_update(curr);
		//

		l_height = hbtree_node_height(curr->edges[LEFT]);
		r_height = hbtree_node_height(curr->edges[RIGHT]);

		the_weight = curr->weight;
		if(curr->weight<2){// we want to be between max(1,floor(log2(height)))
			the_weight = 2;
		}
		
		if(abs(l_height - r_height) > floor(log2(the_weight)) ){
			if(l_height > r_height){

				l_height = hbtree_node_height(curr->edges[LEFT]->edges[LEFT]);
				r_height = hbtree_node_height(curr->edges[LEFT]->edges[RIGHT]);
				if(l_height > r_height){
					curr = right_rotation_avl(curr);
				}
				else{
					curr = double_right_rotation_avl(curr);
				}

			}
			else{
				l_height = hbtree_node_height(curr->edges[RIGHT]->edges[LEFT]);
				r_height = hbtree_node_height(curr->edges[RIGHT]->edges[RIGHT]);
				if(r_height > l_height){
					curr = left_rotation_avl(curr);
				}
				else{
					curr = double_left_rotation_avl(curr);
				}

			}

		}

		curr = curr->edges[PARENT];
		
	}

	if((*root)->edges[PARENT]){
		*root = (*root)->edges[PARENT];
	}

	return(1);
}


hbtree_node* hbtree_node_detach(hbtree_node **root,hbtree_node *detachable){
	hbtree_node *curr,*tmp = tree_node_detach(root,detachable);
	if(tmp == NULL){
		return(NULL);
	}
	curr = tmp->edges[PARENT];
	

	unsigned the_weight;
	short l_height,r_height;

	while(curr){

		//update current node status
		hbtree_node_stat_update(curr);
		//

		l_height = hbtree_node_height(curr->edges[LEFT]);
		r_height = hbtree_node_height(curr->edges[RIGHT]);

		the_weight = curr->weight;
		if(curr->weight<2){// we want to be between max(1,floor(log2(height)))
			the_weight = 2;
		}

		if(abs(l_height - r_height) > floor(log2(the_weight)) ){
			if(l_height > r_height){

				l_height = hbtree_node_height(curr->edges[LEFT]->edges[LEFT]);
				r_height = hbtree_node_height(curr->edges[LEFT]->edges[RIGHT]);
				if(l_height >= r_height){
					curr = right_rotation_avl(curr);
				}
				else{
					curr = double_right_rotation_avl(curr);
				}
				hbtree_rebalance_child_nodes(curr);
			}
			else{
				l_height = hbtree_node_height(curr->edges[RIGHT]->edges[LEFT]);
				r_height = hbtree_node_height(curr->edges[RIGHT]->edges[RIGHT]);
				if(r_height >= l_height){
					curr = left_rotation_avl(curr);
				}
				else{
					curr = double_left_rotation_avl(curr);
				}
				hbtree_rebalance_child_nodes(curr);
			}
			continue;
		}

		curr = curr->edges[PARENT];
		
	}

	for(;*root && (*root)->edges[PARENT] ; *root = (*root)->edges[PARENT]);

	return(detachable);
} 

hbtree_node* tree_node_detach(hbtree_node **root,hbtree_node *detachable){
	if(detachable == NULL){
		return(NULL);
	}
	_tree_node_detach(detachable);
	if(detachable == *root){
		*root = detachable->edges[PARENT];
	}
	return(detachable);
}


hbtree_node* _tree_node_detach(hbtree_node *node){
	short node_pos, child_pos;
	if(node->edges[PARENT]){
		node_pos= node->edges[PARENT]->edges[LEFT] == node ? LEFT : RIGHT;
	}

	if(node->edges[LEFT] && node->edges[RIGHT]){//moribund node has 2 children
		hbtree_node *ptr,*tmp;

		//find the more left node of the right subtree
		tmp = node->edges[RIGHT];
		for(;tmp->edges[LEFT] ;tmp =tmp->edges[LEFT]);
		//

		//detach it (the code for that is the other two cases)
		tmp = _tree_node_detach(tmp);
		ptr = tmp->edges[PARENT] == node ? /*node->edges[PARENT]*/ tmp :tmp->edges[PARENT];//careful there
		//and save the parent (will be needed)
		
		//replace detached node
		make_left_child(tmp,node->edges[LEFT]);
		make_right_child(tmp,node->edges[RIGHT]);
		if(node->edges[PARENT]){
			node_pos == LEFT ? make_left_child(node->edges[PARENT],tmp) : make_right_child(node->edges[PARENT],tmp);
		}
		else{
			tmp->edges[PARENT] = NULL;
		}
		//

		//fix some pointers to have the same algorithmic behaviour
		node->edges[PARENT] = ptr; 
		//

	}
	else if(node->edges[LEFT] || node->edges[RIGHT]){//moribund node has 1 child	

		//determine positions
		child_pos = node->edges[LEFT] ? LEFT : RIGHT;
		//detach child
		node->edges[child_pos]->edges[PARENT] = node->edges[PARENT];

		if(node->edges[PARENT]){		
			//detach parent
			node->edges[PARENT]->edges[node_pos] = node->edges[child_pos];
			//		
		}
		else{
			//if node is in root place put in parent the child that is the next root 
			node->edges[PARENT] = node->edges[child_pos];
		}
		
	}
	else{//moribund node has no child
		if(node->edges[PARENT]){
			node->edges[PARENT]->edges[node_pos] = NULL;
		}
	}

	return(node);
}


void tree_delete(hbtree_node *root){
	char trigger;
	short i; // MUST i >= EDGES_NUM
	hbtree_node *leaf; 
	do{	
		if(root == NULL){
			return;
		}
		//Child searcher
		trigger=0;
		for(i=1 ;i<EDGES_NUM ;i++){
			if(root->edges[i]){
				trigger = 1;
				break;
			}
		}
		//

		//if there are childs go into them
		if(trigger){
			root = root->edges[i];
		}
		else{//delete the current node
			leaf = root;
			root = root->edges[0];

			if(root == NULL){
				//printf("FREED %d\n",leaf->key);
				free(leaf);
				return;
			}

			for(i=1 ;i<EDGES_NUM ;i++){
				if(root->edges[i] == leaf){
					root->edges[i]=NULL;
				}
			}
			
			//printf("FREED %d\n",leaf->key);
			free(leaf);
			
		}

	}while(1);

}
