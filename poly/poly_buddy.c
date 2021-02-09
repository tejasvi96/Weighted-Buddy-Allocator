#include <stdio.h>
#include <stdlib.h>
#include <math.h>
struct block{
    int tag;
    int type;
    int kvalue;
    int size;
    int allocated;
}
;
struct par_ptr_node{
    int size;
    int offset;
    int type;
    struct par_ptr_node *next;
};
typedef struct par_ptr_node par_ptr_node;

struct par_list{
    par_ptr_node *head;
}
;
typedef struct par_list par_list;

typedef struct node node;
struct node{
    char * addr;
    node *next;
    node *prev;
};

struct list{
    node *head;
};


typedef struct list list;
char memory[1<<28];
int tot=1<<28; 
int twos[57];
int nums[60];
int size=56;
typedef struct block block;
long int internal_fragmentation,allocation_request,deallocation_requests,area_split,buddy_recombinations,sum_sizes,allocated_area;
block arr[1<<28];
list asl[57];
par_list par;
void powers(){
    twos[0]=1;
    for(int i=1;i<=size/2;i++)
    twos[i]=2*twos[i-1];
}
int seaarch (int a[],int value,int n){
    int flag=0;
        for(int j=0;j<n;j++){
            // printf("%d ",a[j]);
            if(a[j]==value)
                {
                    flag=1;
                    break;
                }
        }
        return flag;
}
void sort(int a[],int n){
    for(int i=0;i<n;i++){
        for(int j=0;j<n-i-1;j++){
            if(a[j]>a[j+1]){
                int temp=a[j];
                a[j]=a[j+1];
                a[j+1]=temp;
            }
        }
    }
}
 
void init(){
    par.head=NULL;
    internal_fragmentation=0;
    allocation_request=0;
    deallocation_requests=0;
    area_split=0;
    buddy_recombinations=0;
  
    sum_sizes=0;
    allocated_area=0;
    powers();
    arr[0].size=tot;
    arr[0].kvalue=size/2;
    for(int i=0;i<=size;i++){
        asl[i].head=NULL;
    }
    node *ptr;
    ptr=(node *)malloc(sizeof(node));
    ptr->next=NULL;
    ptr->prev=NULL;
    ptr->addr=memory;
    asl[size-1].head=ptr;   
    int num;
    nums[0]=tot;
    // nums[1]=16;
    int i=1;
    int k=0;
    while(k<i){
        num=nums[k];
        k++;
        if(num==0)
            continue;

        if(num%3==0){
            if( seaarch(nums,(2*num)/3,i)!=1 )
                nums[i++]=(2*num)/3;
            if(seaarch(nums,num/3,i)!=1)
                nums[i++]=(num)/3;
        }
        else{   
            if(num%4==0){
            
            if(seaarch(nums,(3*num)/4,i)!=1)
                nums[i++]=(3*num)/4;
            
            if(seaarch(nums,num/4,i)!=1)
                 nums[i++]=num/4;
            
            }

         
        }
    }
    sort(nums,i);
}

int binarysearch(int a[],int key,int low,int high){
    
    int mid=(low +high)/2;
    int flag=0;
    if(low<=high){
        if(a[mid]==key){
            return mid;
        }
        else if(a[mid]>key) return binarysearch(a,key,low,mid-1);
        else return binarysearch(a,key,mid+1,high);
    }
    // if(flag==1) return mid;
    // else 
    //Smallest entry that is greter than the key
    return mid+1;
}


// inserting in the ASL list 
// insertion at the beginning
void insert(list *l,char *addr){
    node *temp=(node *)malloc(sizeof(node));
    temp->addr=addr;
    temp->next=NULL;
    temp->prev=NULL;
    if (l->head==NULL){
        l->head=temp;
    }
    else
    {
        temp->next=l->head;
        l->head->prev=temp;
        l->head=temp;
        
    }
    
}
void removenode(list *l,char *addr){
    
    node *it=l->head;
    if(it==NULL) {
        printf("Couldnt delete\n");
        return;
    }
    if(it->addr==addr){
        l->head=it->next;
        if(l->head!=NULL)
            l->head->prev=NULL;
        return;    
    }
    while(it!=NULL){
        if(it->addr==addr){

            if(it->prev!=NULL)
            it->prev->next=it->next;
            if(it->next!=NULL)
            it->next->prev=it->prev;
        
        break;
        }
        it=it->next;
    }
    if(it==NULL){
        printf("Nothing was deleted here \n");
    }
}
void update_info(int index,int tag,int kvalue,int bsize,int type){
    arr[index].tag=tag;
    arr[index].kvalue=kvalue;
    arr[index].type=type;
    arr[index].size=bsize;
}
void insert_ptr(int offset){
    par_ptr_node *temp=(par_ptr_node *)malloc(sizeof(par_ptr_node));
    temp-> size=arr[offset].size;
        temp->type=arr[offset].type;
        temp->next=NULL;
        temp->offset=offset;
    if(par.head==NULL){
              par.head=temp;
    }
    else{
        temp->next=par.head;
        par.head=temp;
    }
}
void* split(char *ptr,int reqsize){
    // printf("in the split finction");
    int offset=ptr-memory;
    // printf("offset is %lld\t ",offset);
    int index;
    while(arr[offset].size>reqsize){
       //remove the element from the Linked list
       
            if(arr[offset].size%3==0){
            int lb=(2*arr[offset].size)/3;
                if((2*arr[offset].size)/3<=reqsize){
                    break;
                }
            update_info(offset+lb,0,arr[offset].kvalue,(arr[offset].size)/3,2); 
            index=binarysearch(nums,(arr[offset].size)/3,0,size);
            insert(&asl[index],(char * )(ptr+(2*arr[offset].size)/3 ));
            update_info(offset,0,arr[offset].kvalue+1,(2*arr[offset].size)/3,1);
            area_split++;
            insert_ptr(offset); 
            insert_ptr(offset+lb);
            // int sb=(arr[offset].size)/3;
        }
    else if(arr[offset].size%4==0){
            long int lb=(3*arr[offset].size)/4;
                        if((3*arr[offset].size)/4<=reqsize){
                        break;
                    }

            update_info(offset+lb,0,arr[offset].kvalue-2,(arr[offset].size)/4,3);
            index=binarysearch(nums,(arr[offset].size)/4,0,size);
            insert(&asl[index],(char * )( ptr+(3*arr[offset].size)/4 ));
            update_info(offset,0,arr[offset].kvalue-2,(3*arr[offset].size)/4,3);
            area_split++;
            insert_ptr(offset+lb);
    }
    else if(arr[offset].size==2){
        break;
    }
    // printf(" offset %d \n",offset);
     offset=ptr-memory;
    }
    arr[offset].tag=1;


 if(arr[offset].size%3==0){
    if(2*arr[offset].size/3>=reqsize){
            arr[offset].tag=1;
            
            arr[offset].type=1;
            int lb=2*arr[offset].size/3;
            update_info(offset+lb,0,arr[offset].kvalue,arr[offset].size/3,2);
            index=binarysearch(nums,(arr[offset].size)/3,0,size);
            insert(&asl[index],(char * )(ptr+(2*arr[offset].size)/3 ));
            update_info(offset,1,arr[offset].kvalue+1,(2*arr[offset].size)/3,1);
            insert_ptr(offset); 
            insert_ptr(offset+lb);
            area_split++;
            }
        }
    else if( (arr[offset].size)%4 ==0){
        if(3*arr[offset].size/4>=reqsize){
            int lb=3*arr[offset].size/4;
            update_info(offset+lb,0,arr[offset].kvalue-2,arr[offset].size/4,3);
            index=binarysearch(nums,(arr[offset].size)/4,0,size);
            insert(&asl[index],(char * )( ptr+(3*arr[offset].size)/4 ));
            update_info(offset,1,arr[offset].kvalue-2,(3*arr[offset].size)/4,3);
             insert_ptr(offset+lb);
             area_split++;
        }
    }
    else{
        //printf("2 case %d\n",arr[offset].size);
    }
    // printf("Address is %u size returned is %d\n",offset+memory,arr[offset].size);

    //Updating the stats
    internal_fragmentation+=(arr[offset].size-reqsize);
    allocation_request++;
    sum_sizes+=reqsize;
    allocated_area+=arr[offset].size;
    arr[offset].allocated=reqsize;
    return memory+offset;
// We have to determine its type using shifting 
    
}
void *myalloc(int reqsize){
    if(reqsize>tot){
        printf("not possible To allocate\n");
        return NULL;
    }
    int index=binarysearch(nums,reqsize,0,size);
    while(asl[index].head==NULL&&index<size-1){
        // printf("NEed to split the node\n");
        index=index+1;
    }
    void *p;
    if(index==size-1){
        // NEed to split the head node 
        if(asl[size-1].head==NULL){
            printf("Not possible need to report the External Fragmenetation %d %f\n",reqsize,(float)(allocated_area)/tot);
            // Here we can calculate the external fragmentation which is possible 
		exit (0);
            return NULL;
        }
        removenode(&asl[size-1],memory);
       p= (void *)split(memory,reqsize);
       
    }
    else if(asl[index].head!=NULL){
        char *addr=asl[index].head->addr;
        removenode(&(asl[index]),asl[index].head->addr);
  
        p=(void *)split((void *)addr,reqsize);
        
        
    }
    else{
        printf("This is not possible");
    }
    return p;
}
void printasl(){
    printf("\nin printasl\n");
    for(int i=0;i<=size;i++){
        if(asl[i].head!=NULL){
            node *it =asl[i].head;
            printf("\n%d\t",i);
            while(it!=NULL){
                printf("%p\t",it->addr);
                it=it->next;
            }
            
        }
    }
    printf("Exiting asl\n");
}
void printpar_list(){
    par_ptr_node *it=par.head;
    while(it!=NULL){
        printf("%d %d %d\n",it->offset,it->size,it->type);
        it=it->next;
    }
}
int find_type(int rsize,int offset){
    par_ptr_node *it=par.head;
    par_ptr_node *prev=NULL;
    while(it!=NULL){
        if(it->size==rsize && it->offset==offset){
		/*if(prev!=NULL)
			prev->next=it->next;
		else
		  par.head=it->next;	*/	
            return it->type;
        }
	prev=it;
        it=it->next;
    }
    printf("SOme errotr has occured\n");
}
void freem(void *p){
    char *addr=p;
    int offset=(char *)p-memory;
    if(offset<0 || offset >tot){
        printf("Not a valid allocated address\n");
        exit(-1);
    }
    int buddy=0;
    int old_buddy_size=0;
    int flag=0;

    
    if(arr[offset].allocated!=0){
          internal_fragmentation-=(arr[offset].size -arr[offset].allocated);
        allocated_area-=arr[offset].size;
	sum_sizes-=arr[offset].allocated;        
	arr[offset].allocated=0; 
        deallocation_requests++;
    }
    arr[offset].tag=0;
    int ch; 
    if(arr[offset].type==1){
        buddy=offset+twos[arr[offset].kvalue];
         old_buddy_size=arr[buddy].size;
        //parent
        if(arr[buddy].tag==0 && arr[buddy].kvalue==arr[offset].kvalue-1){
                update_info(offset,0,arr[offset].kvalue-1,arr[offset].size+arr[buddy].size,3);
                flag=1;
        }
           
        ch=1;
    }
    else if(arr[offset].type==2){
        buddy=offset-twos[arr[offset].kvalue+1];
         old_buddy_size=arr[buddy].size;
        
        //parent
        if(arr[buddy].tag==0 && arr[buddy].kvalue==arr[offset].kvalue+1){
                 update_info(buddy,0,arr[buddy].kvalue-1,arr[buddy].size+arr[offset].size,3);
                 update_info(offset,0,-1,0,0);
                 flag=1;
        }
       
        ch=2;
    }
    else if(arr[offset].type==3){
        if(arr[offset].size%3==0){
            buddy=offset+3*twos[arr[offset].kvalue];
             old_buddy_size=arr[buddy].size;
          if(arr[buddy].tag==0 && arr[buddy].kvalue==arr[offset].kvalue){
              flag=1;
              int type;
                 if(arr[offset].size+arr[buddy].size==tot){
                     //printf("reached the root somehow\n");
                     type=1;
                 }
                 else{
                     type=find_type(arr[buddy].size+arr[offset].size,offset);
                 }
            ch=1;
            update_info(offset,0,arr[offset].kvalue+2,arr[offset].size+arr[buddy].size,type);
          }
        }
        else{
            buddy=offset-3*twos[arr[offset].kvalue];
            old_buddy_size=arr[buddy].size;

            if(arr[buddy].size%3==0 && arr[buddy].tag==0 && arr[buddy].kvalue==arr[offset].kvalue){
                // if(arr[buddy].tag==0 ){
                 flag=1;
                 int type;
                 if(arr[offset].size+arr[buddy].size==tot){
                    // printf("reached the root\n");
                     type=1;
                 }
                 else{		   
                    type=find_type(arr[buddy].size+arr[offset].size,buddy);
                 }
                update_info(buddy,0,arr[buddy].kvalue+2,arr[offset].size+arr[buddy].size,type);    
                //Just for error checking
                update_info(offset,0,-1,-1,0);
                ch=2;
            //  }
          }

        // }
    }
          //parent
          //Form a type 1
          
           
    
    }
    else{
        printf("Type is not known \n");
    }
        
        
        // Cases possible are buddy is not possibly free 
    
    // if(buddy==0){printf("Failed ");
    // return;}
    if(flag==0){
        //in use only insert in the ASL
        int index=binarysearch(nums,arr[offset].size,0,size);
        insert(&asl[index],memory+offset);
    }
    else{
        //Buddy also free
        //Form a bigger Block
        if(ch==2 &&flag==1){
            buddy_recombinations++;
            // free request has come for the offset only 
            int index=binarysearch(nums, old_buddy_size,0,size);
            removenode(&asl[index],memory+buddy);
            
            if(arr[buddy].size==tot){
                insert(&asl[size-1],memory);
                return;
            }
            freem((void *)(memory+buddy));
        }
        else if(ch==1){
            buddy_recombinations++;
            int index=binarysearch(nums, old_buddy_size,0,size);
            removenode(&asl[index],memory+buddy);
            
            if(arr[offset].size==tot){
                insert(&asl[size-1],memory);
                return;
            }
            freem((void *)(memory+offset));
        }
        else{
            printf("What else is possible\n");
        }
    }
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
       // if(ret==NULL)
         //   printf("(0,0)\n");
        //else ll_print(ret);
        // We must time here 
        freem(head);
        freem(head2);
        // We must time here 
        head=NULL;
        head2=NULL;

}
int main(int argc,char **argv){
    init();
    int time_max=8000;
    int allocation_max=tot;
printf("True");
    int i=0;
for(i=0;i<1000;i++)
    menu_program();
   return 0;
}
