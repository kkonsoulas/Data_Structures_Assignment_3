#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"

int main(void){
char choice=0;
	char array[12]={'\0'};
	int key=0;
	unsigned  *res;
	char rehash_trigger=1;
	hashtable_t hashtable;
	hashtable_init(&hashtable);

	while(1){
		scanf(" %s",array);
		key = atoi(array);
		if(key == 0 && strcmp(array,"0")){
			if(array[1] == 'p'){
				putchar('\n');
				hashtable_data_print(&hashtable);
			}
			else if(array[1] == 'q'){
				hashtable_delete(&hashtable);
				break;
			}
			else if(array[1] == 'r'){
				rehash_trigger = !rehash_trigger;		
			}
			else{
				choice = array[1];
			}
			continue;
		}

		switch(choice){
			case 'i':
				putchar('\n');
				hashtable_add(&hashtable,create_unsigned(key),rehash_trigger) ? printf("INSERTED %d",key) :printf("NOT INSERTED %d",key);
				putchar('\n');
				break;
			case 'd':
				putchar('\n');
				res= hashtable_item_detach(&hashtable,key,rehash_trigger);
				if(res){
					printf("DELETED %d",*res);
					free(res);
				}
				else{
					printf("NOT DELETED %d",key);
				}
				putchar('\n');
				break;
			case 'f':
				putchar('\n');
				hashtable_find(&hashtable,key) >= 0 ?printf("FOUND %d",key) :printf("NOT FOUND %d",key);
				putchar('\n');
 				break;
			default:
				break;
		}
			
	}

	return(0);
}