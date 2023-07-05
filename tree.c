#include "tree.h"
#include <stdlib.h>
#include <stdio.h>


tree_node* _node_create(){
	tree_node *new = (tree_node*) malloc(sizeof(tree_node));
	if(new == NULL)
		exit(-1);
	return(new);
}

void tree_node_init(tree_node *junk_node){
	short i;
	for(i=0 ;i<EDGES_NUM ;i++){
		junk_node->edges[i] = NULL;
	}
	//every other init//
	junk_node->color=RED;
	return;
}

tree_node* tree_node_create(){
	tree_node *new = _node_create();
	tree_node_init(new);
	return(new);
}

void make_right_child(tree_node *parent,tree_node *child){
	if(parent == NULL){
		return;
	}
	
	parent->edges[RIGHT] = child;
	if(child ){
		child->edges[PARENT] = parent;
	}
	
}

void make_left_child(tree_node *parent,tree_node *child){
	if(parent == NULL){
		return;
	}

	parent->edges[LEFT] = child;
	if(child){
		child->edges[PARENT] = parent;
	}
}

tree_node* postorder_to_tree(unsigned *array ,unsigned size){
	unsigned i= size-1;
	tree_node *root = _postorder_to_tree(array,&i,0,0xFFFFFFFF);
	return(root);
}

tree_node* _postorder_to_tree(unsigned *array,unsigned *i,unsigned min ,unsigned max){

	if(*i == 0xFFFFFFFF){
		return(NULL);
	}

	tree_node *root = NULL;

	if( min < array[*i] && array[*i] < max ){
		
		root =tree_node_create();
		root->key = array[(*i)--];

		make_right_child(root,_postorder_to_tree(array,i,root->key,max));
		
		make_left_child(root,_postorder_to_tree(array,i,min,root->key));

	}

	return(root);
}

void postorder_print(tree_node* root){
	if(root == NULL){
		return;
	}

	postorder_print(root->edges[LEFT]);
	postorder_print(root->edges[RIGHT]);
	printf("%u ",root->key);
}

unsigned tree_node_lvl_calc(tree_node *node){
	unsigned lvl;
	for(lvl=0 ;node ;lvl++){
		node = node->edges[PARENT];
	}
	return(lvl);
}

unsigned tree_height_calc(tree_node *root){
	unsigned height=0;
	height = _tree_height_calc(root,height);
	return(height);
}

unsigned _tree_height_calc(tree_node *root,unsigned height){
	if(root == NULL){
		return(height);
	}

	height++;
	unsigned new_height;
	
	new_height = _tree_height_calc(root->edges[LEFT],height);
	
	height = _tree_height_calc(root->edges[RIGHT],height);
	if(height < new_height){
		height = new_height;
	}
	return(height);

}

void tree_to_levelorder_reversed(tree_node *root ,unsigned *array){//array must be large enough
	unsigned height = tree_height_calc(root);
	for(;height ;height--){
		_tree_to_levelorder_reversed(root,&array,height);
	}

}

void _tree_to_levelorder_reversed(tree_node *root,unsigned **array,unsigned height){
	if(root == NULL){
		return;
	}
	else if(height == 1){
		**array = root->key;
		(*array)++;
		return;
	}

	height--;
	_tree_to_levelorder_reversed(root->edges[RIGHT],array,height);
	_tree_to_levelorder_reversed(root->edges[LEFT],array,height);

}

void tree_to_levelorder_lvl_print(tree_node *root,unsigned height){

	if(root == NULL){
		return;
	}
	else if(height == 1){
		printf("%d ",root->key);
		return;
	}

	height--;
	tree_to_levelorder_lvl_print(root->edges[LEFT],height);
	tree_to_levelorder_lvl_print(root->edges[RIGHT],height);

}


tree_node* levelorder_to_tree(unsigned *array ,unsigned height ,unsigned size){
	tree_node *root = NULL;
	unsigned h;

	if(size){
		root = tree_node_create();
		root->key = *array;
		array++;
		size--;
	}
	
	for(h=1 ; h<height ;h++){
		_levelorder_to_tree(root,&array,h,&size);
	}

	return(root);
}


void _levelorder_to_tree(tree_node *root ,unsigned **array ,unsigned height ,unsigned *size){
	if(root == NULL){
		return;
	}

	if(height == 1){
		if(*size){
			tree_node *new = tree_node_create();
			new->key = **array;
			make_left_child(root,new);
			(*array)++;
			(*size)--;
			if(*size){
				new = tree_node_create();
				new->key = **array;
				make_right_child(root,new);
				(*array)++;
				(*size)--;
			}
		}

		return;
	}

	_levelorder_to_tree(root->edges[LEFT],array,height-1,size);
	_levelorder_to_tree(root->edges[RIGHT],array,height-1,size);

	return;
}


short is_binary_search_tree(tree_node *root){
	return(_is_binary_search_tree(root,0,0xFFFFFFFF));
}

short _is_binary_search_tree(tree_node *root ,unsigned min ,unsigned max){
	if(root == NULL){
		return(1);
	}

	if(min <root->key && root->key <max){
		char trigger;
		trigger = _is_binary_search_tree(root->edges[LEFT],min,root->key);
		if(trigger == 0){
			return(0);
		}
		trigger = _is_binary_search_tree(root->edges[RIGHT],root->key,max);
		return(trigger);
	}

	return(0);
}


tree_node* preorder_to_tree(unsigned *array ,unsigned size){
	unsigned *endof_array = array + size;
	tree_node *root = _preorder_to_tree(&array,endof_array,0,0xFFFFFFFF);
	return(root);
}

tree_node* _preorder_to_tree(unsigned **array,unsigned *end,unsigned min ,unsigned max){

	if(*array == end){
		return(NULL);
	}

	tree_node *root = NULL;

	if(min == **array || max == **array){
		(*array)++;
	}
	if( min < **array && **array < max ){
		
		root =tree_node_create();
		root->key = **array;
		(*array)++;

		make_left_child(root,_preorder_to_tree(array,end,min,root->key));

		make_right_child(root,_preorder_to_tree(array,end,root->key,max));
	}

	return(root);
}

void preorder_print(tree_node *root){
	
	if(root == NULL){
		return;
	}
	
	printf("%d ",root->key);
	preorder_print(root->edges[LEFT]);
	preorder_print(root->edges[RIGHT]);
}


tree_node* search_bst(tree_node *root ,unsigned key){
	
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

unsigned* tree_node_commonpath(tree_node *node1,tree_node *node2){
	unsigned size1 = tree_node_lvl_calc(node1);
	unsigned size2 = tree_node_lvl_calc(node2);
	unsigned array1[size1],array2[size2];
	unsigned i,j,commonpath_size;

	//Construct path from node->root
	for(i=0 ;i<size1 ;i++,node1 = node1->edges[PARENT]){
		array1[i] = node1->key;
	}
	for(j=0 ;j<size2 ;j++,node2 = node2->edges[PARENT]){
		array2[j] = node2->key;
	}
	//

	//Find common node_key to calculate commonpath_size
	for(i=size1-1,j=size2-1 ;array1[i] == array2[j] ;i--,j--);
	size1 = i+2;//+2 to include the common node 
	size2 = j+1;
	commonpath_size = size1 + size2 +1;//+1 to include terminating 0
	//

	//Create and fill array with the keys of the path
	unsigned *commonpath_array = (unsigned*) malloc(commonpath_size * sizeof(unsigned));
	for(i=0 ;i<size1 ;i++){
		commonpath_array[i] = array1[i];		
	}
	for(j=size2-1 ;j<0xFFFFFFFF ;j--,i++){
		commonpath_array[i] = array2[j];
	}
	commonpath_array[i] = 0;//adding the terminating 0
	//

	return(commonpath_array);
}

tree_node* tree_node_detach(tree_node **root,tree_node *detachable){
	_tree_node_detach(detachable);
	if(detachable == *root){
		*root = detachable->edges[PARENT];
	}
	return(detachable);
}


tree_node* _tree_node_detach(tree_node *node){
	short node_pos= node->edges[PARENT]->edges[LEFT] == node ? LEFT : RIGHT;
	short child_pos;

	if(node->edges[LEFT] && node->edges[RIGHT]){//moribund node has 2 children
		tree_node *ptr,*tmp;

		//find the more left node of the right subtree
		tmp = node->edges[RIGHT];
		for(;tmp->edges[LEFT] ;tmp =tmp->edges[LEFT]);
		//

		//detach it (the code for that is the other two cases)
		tmp = _tree_node_detach(tmp);
		ptr = tmp->edges[PARENT] == node ? tmp :tmp->edges[PARENT];//careful there
		//and save the parent (will be needed)
		
		//replace detached node
		make_left_child(tmp,node->edges[LEFT]);
		make_right_child(tmp,node->edges[RIGHT]);
		if(node->edges[PARENT]){
			node_pos == LEFT ? make_left_child(node->edges[PARENT],tmp) : make_right_child(node->edges[PARENT],tmp);
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


void tree_delete(tree_node *root){
	char trigger;
	short i; // MUST i >= EDGES_NUM
	tree_node *leaf; 
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


tree_node* left_rotation_avl(tree_node *node){
	tree_node *right = node->edges[RIGHT];
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
	make_right_child(node,right->edges[LEFT]);
	make_left_child(right,node);

	return(right);
}

tree_node* right_rotation_avl(tree_node *node){
	tree_node *left = node->edges[LEFT];
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

	return(left);
}

tree_node* double_right_rotation_avl(tree_node *node){
	tree_node *parent,*left, *l_right;
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

	return(l_right);
} 

tree_node* double_left_rotation_avl(tree_node *node){
	tree_node *parent,*right, *r_left;
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

	return(r_left);
} 

void add2_avl_tree(tree_node **root ,tree_node *new){

	if(search_bst(*root,new->key)){
		return;
	}

	if(*root == NULL){
		*root = new ;
		return;
	}

	tree_node *curr = *root;
	unsigned l_height,r_height;


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

	while(curr){

		l_height = tree_height_calc(curr->edges[LEFT]);
		r_height = tree_height_calc(curr->edges[RIGHT]);

		if(abs(l_height - r_height) >= 2){
			if(l_height > r_height){

				l_height = tree_height_calc(curr->edges[LEFT]->edges[LEFT]);
				r_height = tree_height_calc(curr->edges[LEFT]->edges[RIGHT]);
				if(l_height > r_height){
					curr = right_rotation_avl(curr);
				}
				else{
					curr = double_right_rotation_avl(curr);
				}


			}
			else{
				l_height = tree_height_calc(curr->edges[RIGHT]->edges[LEFT]);
				r_height = tree_height_calc(curr->edges[RIGHT]->edges[RIGHT]);
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

	return;
}


unsigned tree_black_height_calc(tree_node *root){
	unsigned height=0;
	height = _tree_black_height_calc(root,height);
	return(height);
}

unsigned _tree_black_height_calc(tree_node *root,unsigned height){
	if(root == NULL){
		return(height);
	}

	height+=root->color;
	unsigned new_height;
	
	new_height = _tree_black_height_calc(root->edges[LEFT],height);
	
	height = _tree_black_height_calc(root->edges[RIGHT],height);
	if(height < new_height){
		height = new_height;
	}
	return(height);

}


short is_possible_red_black_tree(tree_node *root){
	if(root == NULL){
		return(1);
	}

	unsigned left,right,res=1;

	res*=is_possible_red_black_tree(root->edges[LEFT]);
	res*=is_possible_red_black_tree(root->edges[RIGHT]);

	left = tree_height_calc(root->edges[LEFT]);
	right = tree_height_calc(root->edges[RIGHT]);
	if( (left>right ? left : right) <= 2*(left<right?left:right) +1){
		return(res);
	}
	return(0);
}

short node_has_red_grandchildren(tree_node *node){
	char res=0;
	res += node_has_red_children(node->edges[LEFT]);
	res +=  node_has_red_children(node->edges[RIGHT]);
	return(res);
}

short node_has_red_children(tree_node *node){
	short res=0;
	if(node){
		if(node->edges[LEFT]){
			res += !node->edges[LEFT]->color;
		}
		if(node->edges[RIGHT]){
			res += !node->edges[RIGHT]->color;
		}
	}
	return(res);
}

void enlarge_black_height_of_children(tree_node *root ,unsigned height){
	if(root == NULL || height==0){
		return;
	}

	if(root->edges[LEFT] && root->edges[LEFT]->color == RED){
		root->edges[LEFT]->color = BLACK;
		enlarge_black_height_of_children(root->edges[LEFT],height-1);	
	}
	else{
		enlarge_black_height_of_children(root->edges[LEFT],height);	
	}

	if(root->edges[RIGHT] && root->edges[RIGHT]->color == RED){
		root->edges[RIGHT]->color = BLACK;
		enlarge_black_height_of_children(root->edges[RIGHT],height-1);	
	}
	else{
		enlarge_black_height_of_children(root->edges[RIGHT],height);
	}

}


void color_red_black_cappable_tree(tree_node *root){
	if(root==NULL){
		return;
	}
	_color_red_black_cappable_tree(root);
	root->color = BLACK;
	return;
}

void _color_red_black_cappable_tree(tree_node *root){
	if(root==NULL){
		return;
	}
	if(root->edges[LEFT] == NULL && root->edges[RIGHT] ==NULL){
		return;
	}

	_color_red_black_cappable_tree(root->edges[LEFT]);
	_color_red_black_cappable_tree(root->edges[RIGHT]);
	
	short left,right;
	if(root->edges[LEFT] && root->edges[RIGHT]){
		
		left = tree_black_height_calc(root->edges[LEFT]);
		right = tree_black_height_calc(root->edges[RIGHT]);
		
		if(node_has_red_grandchildren(root)){
			
			if(left>right){
				root->edges[LEFT]->color = RED;
				root->edges[RIGHT]->color= BLACK;
				enlarge_black_height_of_children(root->edges[RIGHT],(left-right)-1);	
			}
			else if(right>left){
				root->edges[LEFT]->color = BLACK;
				root->edges[RIGHT]->color= RED;
				enlarge_black_height_of_children(root->edges[LEFT],(right-left)-1);
			}
			else{
				root->edges[LEFT]->color = BLACK;
				root->edges[RIGHT]->color= BLACK;	
			}
		}
		else{
			
			if(left>right){
				root->edges[LEFT]->color = RED;
				root->edges[RIGHT]->color= BLACK;
				enlarge_black_height_of_children(root->edges[RIGHT],(left-right)-1);
			}
			else if(right>left){
				root->edges[LEFT]->color = BLACK;
				root->edges[RIGHT]->color= RED;
				enlarge_black_height_of_children(root->edges[LEFT],(right-left)-1);
			}
			else{
				root->edges[LEFT]->color = RED;
				root->edges[RIGHT]->color= RED;	
			}
		
		}

	}
	else if(root->edges[LEFT]){
		root->edges[LEFT]->color= RED;
	}
	else{
		root->edges[RIGHT]->color = RED/*BLACK*/;
	}

}


void preorder_rb_print(tree_node *root){
	
	if(root == NULL){
		return;
	}
	
	printf("%d#%c ",root->key,root->color?'B':'R');
	preorder_rb_print(root->edges[LEFT]);
	preorder_rb_print(root->edges[RIGHT]);
}


