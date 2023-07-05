#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hbtree.h"

int main(void){
	char choice=0;
	char array[12]={'\0'};
	int key=0;
	short res;
	hbtree_node *tmp,*root = NULL;

	while(1){
		scanf(" %s",array);
		key = atoi(array);
		if(key == 0 && strcmp(array,"0")){
			if(array[1] == 'p'){
				putchar('\n');
				preorder_print(root);
				putchar('\n');
				//printf("root->%d h:%u w:%u\n",root->key,root->height,root->weight);
			}
			else if(array[1] == 'q'){
				tree_delete(root);
				break;
			}
			else{
				choice = array[1];
			}
			continue;
		}

		switch(choice){
			case 'i':
				res = add2_avl_hbtree(&root,hbtree_node_create_wk(key));
				putchar('\n');
				res ? printf("INSERTED %d\n",key) : printf("NOT INSERTED %d\n",key);
				break;
			case 'd':
				tmp = hbtree_node_detach(&root,search_bst(root,key));
				tmp ? free(tmp) : 0;
				putchar('\n');
				tmp ? printf("DELETED %d\n",key) : printf("NOT DELETED %d\n",key);
				break;
			case 'f':
				tmp = search_bst(root,key);
				putchar('\n');
				tmp ? printf("FOUND %d\n",key) : printf("NOT FOUND %d\n",key);
 				break;
			default:
				break;
		}
			

	}
	
	return(0);
}