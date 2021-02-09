#include "stdio.h"
#include "stdlib.h"
typedef struct heapnode heapnode;
struct heapnode{
    char * addr;
    unsigned int allocated;
    heapnode *next;
    heapnode *prev;
}
;
struct pheap{
    heapnode *head;
};
unsigned long int internal_fragmentation,allocation_request,deallocation_requests,sum_sizes,allocated_area;

typedef struct checklist_node checklist_node;
struct checklist_node{
    char *p;
    checklist_node *next;
};
typedef struct checklist checklist;
struct checklist{
    checklist_node *head;
};

checklist clist;
unsigned long int internal_fragmentation=0;
typedef struct pheap pheap;
int tot=256*1024*1024;
char memory[256*1024*1024];
int size;
int blocks;
pheap plist;
int curmax;

int main(int argc,char **argv){
    printf("Enter the block size");
    scanf("%d",&size);
    if(size>tot){
        printf("Not possible\n");
        exit(-1);
    }
    init();
    int *a=myalloc(8);
    if(a!=NULL){
        printf("Allocated");
    }
    if(a==NULL){printf("cannot allocate\n");
    exit(-1);
    }
    *a=10;
    printf("%p %d\n",a,*a);
    printlist(&plist);
    freem(a);
    printf("\nAfter freeing\n");
    printlist(&plist);
    return 0;
}
