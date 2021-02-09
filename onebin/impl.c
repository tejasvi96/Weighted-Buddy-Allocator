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
extern unsigned long int internal_fragmentation,allocation_request,deallocation_requests,sum_sizes,allocated_area;

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
typedef struct pheap pheap;
extern int tot;
char memory[256*1024*1024];
extern int size;
extern int blocks;
pheap plist;
extern int curmax;
void insert_checklist(checklist *p,char *ptr){
    checklist_node *temp=(checklist_node *)malloc(sizeof(checklist_node));
    temp->p=ptr;
    temp->next=NULL;
    if(p->head==NULL){
        p->head=temp;
    }
    else{
        temp->next=p->head;
        p->head=temp;
    }
}
int find_remove_checklist(checklist *p,char *addr){
    if(p->head==NULL){
        printf("Error encountered not possible\n");
        return -1;
    }
    else{
        checklist_node *it=p->head;
        checklist_node *prev=NULL;
        while(it!=NULL){
            if(it->p==addr){
                if(prev!=NULL)
                prev->next=it->next;
                else{
                    p->head=NULL;
                }
                return 1;
            }
                prev=it;
                it=it->next;
        }
    }
}
void printlist(pheap *p){
    heapnode *it=p->head;
    while(it->next!=NULL){
        printf("%p\t",it->addr);
        it=it->next;
    }
}
void insert(pheap *p,char * addr){
    heapnode *temp=(heapnode *)malloc(sizeof(heapnode));
    temp->addr=addr;
    temp->next=NULL;
    temp->prev=NULL;
    
    if(p->head==NULL){
        p->head=temp;
    }
    else{
        temp->next=p->head;
        p->head=temp;
    }
}
void init(){
    blocks=tot/size;
    curmax=(unsigned)memory;
    // init(&plist);
    plist.head=NULL;
    clist.head=NULL;
}
void * getmemory(unsigned int reqsize){
    internal_fragmentation+=size-reqsize;
    
     char * addr;
     heapnode *temp=plist.head;
    if(temp!=NULL){
       addr=temp->addr;
       free(temp); 
        plist.head=plist.head->next;
    }
    insert_checklist(&clist,addr);
    return addr;
}
void *myalloc(int reqsize){
    if(reqsize>size){
        printf("Not possible\n");
        return NULL;
    }
    else{
        if(curmax==tot){
            printf("not possible");
            return NULL;
        }
        allocation_request++;
        internal_fragmentation+=(size-reqsize);
        sum_sizes+=reqsize;
        allocated_area+=size;
        if(plist.head!=NULL){
            return getmemory(reqsize);
        }
        else{
            for(int i=0;i<10;i++){
                insert(&plist,(char *)(memory +curmax+i*size) );   
            }
            curmax=curmax+10*size;
            return getmemory(reqsize);
        }
    }

}
void freem(void *ptr){
    if(find_remove_checklist(&clist,(char *)ptr)==-1){
        printf("Envalid Request\n");
        exit(0);
    }
    deallocation_requests++;
    internal_fragmentation-=(size-((heapnode *)ptr)->allocated);
    heapnode *temp=malloc(sizeof(heapnode));
    temp->addr=(char *)ptr;
    temp->next=plist.head;
    plist.head=temp;
}