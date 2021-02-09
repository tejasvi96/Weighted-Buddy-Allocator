#include "stdio.h"
#include "stdlib.h"


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
    pnode *temp=(pnode *)malloc(sizeof(pnode));
    
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
   // printf("Quotient : ");
    //ll_print(quo);
    return ret;
}
void menu_program(){
    int n,cnt=0;;
    char ch;
    float coeff;
    int exp;
    struct pnode *head=NULL;
	head=(pnode *)malloc(sizeof(pnode));
    head->exp=100;
	head->coeff=1;	
	struct pnode *temp2=NULL;
		temp2=(pnode *)malloc(sizeof(pnode));

	temp2->exp=0;
	temp2->coeff=-1;
	temp2->next=NULL;
	head->next=temp2;    
	struct pnode *head2=NULL;
head2=(pnode *)malloc(sizeof(pnode));
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
       // if(ret==NULL)
         //   printf("(0,0)\n");
        //else ll_print(ret);
        // We must time here 
        free(head);
        free(head2);
        // We must time here 
        head=NULL;
        head2=NULL;

}
int main(int argc,char **argv){
    int i=0;
for(i=0;i<1000;i++)
    menu_program();
   return 0;
}
