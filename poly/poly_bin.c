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
int tot=1<<28;
char memory[1<<28];
int size;
int blocks;
pheap plist;
int curmax;
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
void init(int n){
    size=n;	
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
struct pnode {
    float  coeff;
    int  exp;
    struct pnode *next;
    struct pnode *prev;
};
typedef struct pnode pnode;
void ll_insert_reverse(pnode ** ptr,float coeff,int exp){
    pnode *it=*ptr;
    // Here do the timing analysis
    pnode *temp=(pnode *)myalloc(sizeof(pnode));
    
    temp->coeff=coeff;
    temp->exp=exp;
    temp->next=NULL;
    
    if(it==NULL){
        temp->prev=NULL;
        (*ptr)=temp;
    }
    else{
        while(it->next!=NULL){
            it=it->next;
        }
        temp->prev=it;
        it->next=temp;
        
    }
}

pnode *div_mul(pnode *ptr,float coeff,int exp){
    pnode *ret=NULL;
    pnode *it=ptr;
    while(it!=NULL){
        ll_insert_reverse(&ret,it->coeff*(coeff),it->exp+exp);
        it=it->next;
    }
    return ret;
}
void ll_print(pnode *ptr){
    pnode *it=ptr;
    while(it!=NULL){
        printf("(%d,%f) ",it->exp,it->coeff);
        it=it->next;
    }
    printf("\n");
}

pnode * ll_sub(pnode *ptr1, pnode *ptr2){
    pnode *it1=ptr1;
    pnode *it2=ptr2;
    // resultant
    pnode *ret=NULL;
    int deg_ptr1,deg_ptr2;
    while(it1!=NULL && it2!=NULL){
        deg_ptr1=it1->exp;
        deg_ptr2=it2->exp;
        while(it1!=NULL && it1->exp>it2->exp){
            ll_insert_reverse(&ret,it1->coeff,it1->exp);
            it1=it1->next;
        }
        if(it1==NULL)break;

        // if power equal then simply subtract
        if(it1->exp==it2->exp){
            // insert only if non zero
            if(it1->coeff!=it2->coeff)
                ll_insert_reverse(&ret,it1->coeff-it2->coeff,it1->exp);
                it1=it1->next;
                it2=it2->next;
        }
        else{
            ll_insert_reverse(&ret,(-1)*it2->coeff,it2->exp);
            it2=it2->next;
        }
    }
    while(it2!=NULL){
        ll_insert_reverse(&ret,(-1)*it2->coeff,it2->exp);
        it2=it2->next;
    }
    while(it1!=NULL){
        ll_insert_reverse(&ret,it1->coeff,it1->exp);
        it1=it1->next;
    }
    return ret;
}
pnode * ll_div(pnode *ptr1,pnode *ptr2){
    pnode *it1=ptr1;
    pnode *it2=ptr2;
    float mul_coeff;
    float mul_exp;
    pnode *quo=NULL;
    pnode *ret=NULL;
    while(it1!=NULL && it2!=NULL){
        if(it1->exp<it2->exp){
            // printf("DEgree of numerator is lesser\n");
            break;
        }
        mul_coeff=it1->coeff/it2->coeff;
        mul_exp=it1->exp-it2->exp;
        ll_insert_reverse(&quo,mul_coeff,mul_exp);
        ret=div_mul(ptr2,mul_coeff,mul_exp);
        ret=ll_sub(ptr1,ret);
        ptr1=ret;
        it1=ptr1;
    }
    // ll_print(ret);
    //printf("Quotient : ");
    //ll_print(quo);
    return ret;
}
void menu_program(){
    int n,cnt=0;;
    char ch;
    float coeff;
    int exp;
    struct pnode *head=NULL;
	head=(pnode *)myalloc(sizeof(pnode));
    head->exp=100;
	head->coeff=1;	
	struct pnode *temp2=NULL;
		temp2=(pnode *)myalloc(sizeof(pnode));

	temp2->exp=0;
	temp2->coeff=-1;
	temp2->next=NULL;
	head->next=temp2;    
	struct pnode *head2=NULL;
head2=(pnode *)myalloc(sizeof(pnode));
head2->exp=1;
head2->coeff=1;
	temp2->exp=0;
	temp2->coeff=-1;
	temp2->next=NULL;
	head2->next=temp2;
    /*printf("Enter the Dividend and divisor in decreasing magnitude of powers \n");
    printf("Enter the Polynomial as (power,coefficient)[space separated] terminated with a newline\n");
    while(1){
        scanf("(%d,%f)%c",&exp,&coeff,&ch);
        // cnt++;
        ll_insert_reverse(&head,coeff,exp);
        if(ch=='\n')break;
        }
        printf("Enter the Polynomial as (power,coefficient)[space separated] terminated with a newline\n");
    while(1){
        scanf("(%d,%f)%c",&exp,&coeff,&ch);
        // cnt++;
        ll_insert_reverse(&head2,coeff,exp);
        if(ch=='\n')break;
    }*/
        
        pnode *ret=ll_div(head,head2);
        //printf("Remainder :");
        //if(ret==NULL)
          //  printf("(0,0)\n");
        //else ll_print(ret);
        // We must time here 
        freem(head);
        freem(head2);
        // We must time here 
        head=NULL;
        head2=NULL;

}
int main(int argc,char **argv){
    init(24);
    int time_max=8000;
    int allocation_max=tot;
    int i=0;
for(i=0;i<1000;i++)
    menu_program();
   return 0;
}
