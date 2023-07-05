#ifndef __HTABLE_H__
#define __HTABLE_H__

#define MAX_LOAD_FACTOR 0.5
#define MIN_LOAD_FACTOR 0.125

struct hashtable_matrix{
    int size;
    int members;
    int tombstones;
    unsigned **array;//hashtable (array of ptr to items)
};
typedef struct hashtable_matrix hashtable_t;

void hashtable_array_init(unsigned **array ,const int SIZE);

void hashtable_init(hashtable_t *hashtable);

unsigned* create_unsigned(unsigned key); 

int hash_it(int ht_size,unsigned key);

//Gives pos in array else value <0
int hashtable_find(hashtable_t *hashtable ,unsigned key);

short hashtable_add(hashtable_t *hashtable, unsigned *new, char rehash_trigger);

void hashtable_rehash(hashtable_t *hashtable);

unsigned* hashtable_item_detach(hashtable_t *hashtable ,unsigned key ,char rehash_trigger);

void hashtable_data_print(hashtable_t *hashtable);

//deletes the hashtable_array and all the items 
void hashtable_delete(hashtable_t *hashtable);


#endif