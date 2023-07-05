#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

#define MAX_SIZE 128

int main(void){
	unsigned i;
	int key;
	unsigned *array = (unsigned*) malloc(MAX_SIZE*sizeof(int));
	scanf(" %d",&key);
	for(i=0 ;key>=0 && i<MAX_SIZE ;i++){
		array[i] = key;
		scanf(" %d",&key);
	}

	tree_node *root = preorder_to_tree(array,i);
	
	//key = result
	key = is_possible_red_black_tree(root); 
	key ? printf("RB OK") : printf("RB NOK");
	printf("\n\n");

	if(key){
		color_red_black_cappable_tree(root);
		preorder_rb_print(root);
		putchar('\n');
	}
	
	free(array);
	tree_delete(root);
	return(0);
}