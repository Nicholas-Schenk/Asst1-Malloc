#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymalloc.h"

char static memory[4096];

//function to initialize to free block of memory size - 5( 5 bytes for metadata)
//for example, if memory size = 4096, it will be initialized as one free block of4091 bytes

void initialize_memory(){
	char* free = &memory[0];
	*free = 'f';
	int size = sizeof(memory)-5;
	memcpy(&memory[1], &size, sizeof(int));
	return;
}

//function to print memory state
void print_memory() { 
    int i = 0,size;
    printf("----------------\n");
    while (i<sizeof(memory)){
        printf("%c\n", memory[i]);
	memcpy(&size, &memory[i+1], sizeof(int));
	printf("%d\n\n", size);
	i+= size+5;
    }
    printf("-------------------\n\n");
}

//function to find free block of sufficient size to malloc
//returns NULL if no sufficiently sized blocks are found

void* find_free(int size) {
    void* ptr = NULL;
    int i = 0, block_size;
    while (i < sizeof(memory)) {  
        if (memory[i] == 'd') { //data block, jump using size of block + 4 for size int
            memcpy(&block_size, &memory[i+1], sizeof(int));
            i += block_size + 5;
        } else if (memory[i] == 'f') {	//free block
            memcpy(&block_size, &memory[i+1], sizeof(int));
            if (block_size < size) { // block isn't big enough
                i += block_size + 5;
            } else {  // free block of sufficient size found, return pointer to the block
                ptr = &memory[i];
                return ptr;
            }
        }
    }
    

    return ptr; // ptr is NULL, only reached if no free block of sufficient size found

}

void* mymalloc (int size, char file[], int line) {
    if (!(memory[0])) {    //initialize memory as one free block of (memory size - 5) 
	initialize_memory();    //metadata = 1 byte 'f' or 'd' + 4 bytes int size of block
    }
	
    if (size < 1) {
	printf("Call from %s Line %d for %d bytes could not be allocated. At least 1 byte must be requested. \n", file, line, size);
        return NULL;
    }

    char* free_block = (char*)find_free(size);     //find next free block with large enough size
    if (free_block == NULL) {
        printf("Call from %s Line %d for %d bytes could not be allocated. No block of sufficient size was found. \n", file, line, size);
        return NULL;
    }
    *free_block = 'd';    // mark block as data because it will now be allocated
    int* size_ptr = (int*)(free_block+1);
    int free_size;
    memcpy(&free_size, size_ptr, sizeof(int));    //store size of previously free block
    if(free_size >= size+6){ // split if the free block is large enough
        memcpy(size_ptr, &size, sizeof(int));      // we need at least 6 bytes for a new block (5 for metadata and at least 1 for data). If we don't have that then the data block gets the extra couple of bytes. 
        char temp = 'f';  // making extra free block w/ leftover space
        memcpy(&free_block[size+5], &temp, sizeof(char));
        free_size -= size+5;
        memcpy(&free_block[size+6], &free_size, sizeof(int));  
    }
    


   void* final_ptr = free_block+5; // move ptr past the metadata of the block and return the ptr to the first byte of data

    return final_ptr;
}
//after freeing blocks of data, there may be consecutive blocks which should be combined so that we can store as large of blocks of data as possible. this method check the memory for consecutive free blocks and combines them 
void combine_free(){
    int i = 0;
    while (i <sizeof(memory)) {  
        if (memory[i] == 'f') {
	    int free_size;
	    int start_pos = i+1; // store where the potential consecutive free blocks starts
	    memcpy(&free_size, &memory[i+1], sizeof(int)); // store size of free block
	    i += free_size + 5; // move i to next metadata
	    while(memory[i] == 'f'&& i < 4095){ // for consecutive free blocks, take their size and add it +5(metadata) to the size of the original
	        int curr_size;
                memcpy(&curr_size, &memory[i+1], sizeof(int));
		free_size +=5 +curr_size;
                i += curr_size + 5;
            }
	    memcpy(&memory[start_pos], &free_size, sizeof(int));// copy the sum of the free consecutive blocks sizes into the first ones size part of metadata
        } else if (memory[i] == 'd') { // move on to next block's metadata
            int free_size;
            memcpy(&free_size, &memory[i+1], sizeof(int));
            i += free_size + 5;
        }
        
    
    }



}


int myfree(void* ptr, char file[], int line){
    int i = 0;
    while (i < sizeof(memory)) {  
        if (memory[i] == 'f') { //if block is free, can't have the data, move to next
	    int free_size;
            memcpy(&free_size, &memory[i+1], sizeof(int));
            i += free_size + 5;
        } else if (memory[i] == 'd') {	//gotta check each block of dat
            void * temp = &memory[i];
            void* size_ptr = temp + 5;
            if (size_ptr == ptr) { //we found the ptr to free, just need to change metadata to char 'f'
                memory[i] = 'f';
		combine_free(); // check if freeing block made any consecutive blocks that need to be combined
		return 1;
            } else {
                int free_size;
                memcpy(&free_size, &memory[i+1], sizeof(int));
                i += free_size + 5;
            }
        }else{
		//should only happen if memory has not initialized at all 
		printf("Unable to free pointer given at %s line %d as the memory has not been initialized(no malloc calls have been made yet).\n", file, line);
		return 0;
	}
    }
    // if we reach here, ptr wasn't in memory
    printf("Unable to free, pointer given at %s line %d was not found in memory. \n", file, line);
    return 0;
               

}
