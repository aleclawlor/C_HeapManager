#include <stdio.h>
#include <stdlib.h>
#include "memorymgr.h"

int *first;
int *sentinelBlock;

void allocate(int *p);
void initmemory(int size);
void deallocate(int *p);
int getAllocation(int *p);
int nextMultipleOf8(int n);
void setLength(int *p, int n);
int sizeOf(int *p);
int *nextBlock(int *p); // return a pointer to the block that follows p

void initmemory(int size){
    //39  + 4 + 4 + 4 (includes header and sentinel)
    int heapLength = nextMultipleOf8(size + 12);
    int *heap = malloc(heapLength);

    first = heap + 1; // skip the header; allign on a 4-byte boundary
    // set length for the first block to heapLength-8
    setLength(first, heapLength-8);

    sentinelBlock = nextBlock(first);
    setLength(sentinelBlock, 0);
}

int nextMultipleOf8(int size){

    int curr = size;
    while((curr % 8) != 0){
        curr ++;
    }
    return curr; 
}

void *myalloc(int length){
    // nextMultipleOf8 is used for allignment purposes
    int newLength = nextMultipleOf8(length + 4);
    int *ptr = firstBlock();

    // while size > new length and we are not at the end {
        // keep going and update ptr
    //}

    while(*ptr != *lastBlock()){
        // if the pointer is allocated and size > our new length
       if(!getAllocation(ptr) && sizeOf(ptr) >= newLength){
           // allocate here and split 
           int currentSize = *ptr;
           int currNewLength = newLength + 1;
           setLength(ptr, currNewLength);
           int *next = nextBlock(ptr);
           *next = currentSize - newLength;
           return ptr + 1;
       }
       
        ptr = nextBlock(ptr);  // go to the next block
    }
    
    // returns null at the end only if it never got to a large enough block
    return NULL;

    // split the block
    // we need to divide the space into allocated and unallocated
    // we want to set size of first block to what we already defined and update size and head value of second

    // while (not at the end && space is allocated or space is less than what we need)
        // {update ptr;}
    // split the block
}

void myfree(void *p){
    // typecast to integer pointer
    int *ptr = (int *)p-1;
    // deallocate 
    *ptr = *ptr - 1;
}

int *nextBlock(int *p){
    // have to divide by 4 because our pointer is int*
    return p + *p/4;
}

void coalesce(){
    // follow pseudocode on the slides
    int *current = firstBlock();
    int *next = nextBlock(first);

    // go through every block
    while(*next != *lastBlock()){
        // if the current pointer and one after it are allocated
        if(!getAllocation(current) == 1 & !getAllocation(next) == 1){
            int sizeCurrent = sizeOf(current);
            int sizeNext = sizeOf(next);
            setLength(current, sizeCurrent + sizeNext);
            next = nextBlock(current);
        }
        else{
            // look at the next two blocks
            current = nextBlock(current);
            next = nextBlock(current);
        }
    }
}

void printallocation(){
    // for each block, print the block size and status on whether it's allocated or not
    int * current = firstBlock();
    int i = 0;
    while(current != lastBlock()){
        printf("Block Number: %d Size: %d \n", i, sizeOf(current));
        switch (getAllocation(current)){
            case(1): 
                printf("Is allocated \n\n");
                break;
            case(0):
                printf("Unallocated \n\n");
                break; 
        }
        current = nextBlock(current);
        i++;
    }
}

int getAllocation(int *p){
    // if current block is allocated, its lsb will contain a 1
    return ((*p % 2) == 1);
}

int sizeOf(int *p){
    return(*p/8*8);
}

void setLength(int * p, int length){
    *p = length;
}

int * firstBlock(){
    int* temp = first;
    return(temp);
}

int * lastBlock(){
    int *temp = sentinelBlock;
    return(temp);
}