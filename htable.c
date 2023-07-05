#include <stdlib.h>
#include <stdio.h>
#include "htable.h"
//#include <math.h>

void hashtable_init(hashtable_t *hashtable){
	hashtable->size =2;
	hashtable->members=0;
	hashtable->tombstones=0;
	hashtable->array = (unsigned **) calloc(hashtable->size,sizeof(unsigned*));
}

unsigned* create_unsigned(unsigned key){
	unsigned* ptr = (unsigned*) malloc(sizeof(unsigned));
	*ptr = key;
	return(ptr);
}

int hash_it(int ht_size,unsigned key){
	return(key % ht_size);
}

int hashtable_find(hashtable_t *hashtable ,unsigned key){
	int pos, i=0;
	
	//It may be a good addition to check if it has done a full circle
	for(i=0 ;1 ;i++){
		pos = hash_it(hashtable->size,key+i);
		if(hashtable->array[pos] == NULL){
			return(-1);
		}
		else if(hashtable->array[pos] != (unsigned*) 0x1){
			if(*(hashtable->array[pos]) == key){
				return(pos);
			}
		}

	}

}

short hashtable_add(hashtable_t *hashtable, unsigned *new, char rehash_trigger){//WORK IN PROGRESS//

	if(hashtable_find(hashtable,*new) >= 0){//if item exists
		free(new);
		return(0);
	}

	if(rehash_trigger){
		hashtable_rehash(hashtable);
	}

	// Everything ok, add it
	int i,pos;
	for(i=0 ;1 ;i++){
		pos = hash_it(hashtable->size,(*new) + i);
		if(hashtable->array[pos]== NULL || hashtable->array[pos] == (unsigned*) 0x1){
			if(hashtable->array[pos] == (unsigned*) 0x1){
				hashtable->tombstones--;
			}
			hashtable->array[pos] = new;
			break;
		}

	}
	hashtable->members++;
	//
	return(1);
}

void hashtable_rehash(hashtable_t *hashtable){
	double load_factor = (double) hashtable->members / hashtable->size;
	int new_size, j ,i ,pos;

	
	//set new size (if needed)
	if(hashtable->size == 2 && hashtable->members == 0){
		return;
	}
	else if(load_factor >= MAX_LOAD_FACTOR){
		new_size = 2 * hashtable->size;
	}
	else if(load_factor <= MIN_LOAD_FACTOR){
		new_size = hashtable->size / 2;
	}
	else{
		return;
	}
	//
	
	//create new array and put all the items inside
	unsigned **new_array = (unsigned**) calloc(new_size,sizeof(unsigned*));
	for(i=0 ;i<hashtable->size ;i++){
		if(hashtable->array[i] == NULL || hashtable->array[i] == (unsigned*) 0x1){
			continue;
		}

		for(j=0 ;1 ;j++){
			pos = hash_it(new_size, *(hashtable->array[i]) + j);
			if(new_array[pos] == NULL){
				new_array[pos] = hashtable->array[i];
				break;
			}
		}

	}
	//
	free(hashtable->array);//delete old array

	//update hashtable
	hashtable->array = new_array;
	hashtable->size = new_size;
	hashtable->tombstones =0;
	//
	return;
}

unsigned* hashtable_item_detach(hashtable_t *hashtable, unsigned key, char rehash_trigger){
	int pos = hashtable_find(hashtable,key);
	if(pos<0){
		return(NULL);
	}

	unsigned *ptr = hashtable->array[pos]; //get the item
	hashtable->array[pos] = (unsigned*) 0x1;// put tombstone
	hashtable->members--;
	hashtable->tombstones++;
	if(rehash_trigger){//check for rehash
		hashtable_rehash(hashtable);
	}
	return(ptr);
}

void hashtable_data_print(hashtable_t *hashtable){
	printf("SIZE: %d, INSERTED: %d, DELETED: %d\n",hashtable->size,hashtable->members,hashtable->tombstones);

	int i;
	for(i=0 ;i<hashtable->size ;printf("%3d ",i),i++);
	putchar('\n');
	for(i=0 ;i<hashtable->size ;i++){
		if(hashtable->array[i] == (unsigned*) 0x1){
			printf("  # ");
		}
		else if(hashtable->array[i]){
			printf("%3d ",*(hashtable->array[i]));
		}
		else{
			printf("  * ");
		}
	}
	putchar('\n');

}

void hashtable_delete(hashtable_t *hashtable){
	int i;

	for(i=0 ;i<hashtable->size ;i++){
		if(hashtable->array[i] && hashtable->array[i] != (unsigned*) 0x1){
			free(hashtable->array[i]);			
		}

	}
	free(hashtable->array);
}
