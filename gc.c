#include <stdio.h>
#include <stdlib.h>
#include "gc.h"
#include "memorymgr.h"

void mark(int *p){

    if((p >= firstBlock() && p <= lastBlock()) == 0){
        return; 
    }

    int *curr = firstBlock();
    while(p > nextBlock(curr)){
        curr = nextBlock(curr);
    }

    if((*curr % 2) == 1){
        setLength(curr, *curr + 2);
        printf("%d\n", *curr);
    }
}

void sweep(){

    int *ptr = firstBlock();

    while(ptr != lastBlock()){
        if ((*ptr % 4) != 3){
            if((*ptr%2) == 1){
                myfree(ptr);
            }
        } 
        ptr = nextBlock(ptr);
    }
}

void *stackBottom() {
    unsigned long bottom;    
    FILE *statfp = fopen("/proc/self/stat", "r");    
    fscanf(statfp, 
            "%*d %*s %*c %*d %*d %*d %*d %*d %*u "
            "%*u %*u %*u %*u %*u %*u %*d %*d " 
            "%*d %*d %*d %*d %*u %*u %*d "
            "%*u %*u %*u %lu", 
            &bottom);    
    fclose(statfp);
    return (void *) bottom;
}

void gc(){
    int **max = (int **)stackBottom();
    int *dummy;
    // dummy, at first, points to top of the stack
    int **p = &dummy;
    while(p < max){
        // we need to treat each 4 bytes of the stack as a potential point to the heap
        // pass address to mark (dereferences pointer once)
        mark(*p);
        p++;
    }
    sweep();
    coalesce();
}
