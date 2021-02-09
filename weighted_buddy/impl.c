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

typedef struct simulation_node simulation_node;

struct simulation_node{
    char* p;
    int t;
    simulation_node *next;
};

struct simulation_ll{
    simulation_node *head;
};
typedef struct simulation_ll simulation_ll;

char memory[1<<28];
extern int tot;
int twos[57];
int nums[60];
extern int size;
typedef struct block block;
long int internal_fragmentation,allocation_request,deallocation_requests,area_split,buddy_recombinations,sum_sizes,allocated_area;
block arr[1<<28];
list asl[57];
par_list par;

// char memory[1<<28];
// int tot=1<<28; 
// int twos[57];
// int nums[60];
// int size=56;
// typedef struct block block;
// long int internal_fragmentation,allocation_request,deallocation_requests,area_split,buddy_recombinations,sum_sizes,allocated_area;
// block arr[1<<28];
// list asl[57];
// par_list par;
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
    int index,lb;
    while(arr[offset].size>reqsize){
       //remove the element from the Linked list
       
            if(arr[offset].size%3==0){
            lb=(2*arr[offset].size)/3;
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
            lb=(3*arr[offset].size)/4;
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
            // arr[offset].tag=1;
            
            // arr[offset].type=1;
            lb=2*arr[offset].size/3;
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
            lb=3*arr[offset].size/4;
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
int find_type(int size,int offset){
    par_ptr_node *it=par.head;
    while(it!=NULL){
        if(it->size==size && it->offset==offset){
            return it->type;
        }
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

int uniform_distribution(long int rangeLow,int rangeHigh) {
     
    long int range = rangeHigh - rangeLow + 1;
    long int myRand = rand()%(range);
    long int myRand_scaled = myRand + rangeLow;
    return myRand_scaled;
}
int expo(int low,int high){
   
    double f1=1-exp2l(-(double)low*0.001);
    double f2=1-exp2l(-(double)high*0.001);
    double rand1=low+(rand())%(high-low+1);
    double rv=1-exp2l(-(double)rand1*0.001);
    double scaling=(rv-f1)/(f2- f1);
    return((int)((double)low+(double)(high-low)*(scaling)));
    // return (double)scaling;
}
void insert_sim(simulation_ll *head,void * request,int time){
    simulation_node *it=head->head;
    simulation_node *temp=(simulation_node *)malloc(sizeof(simulation_node));
    temp->p=(char*)request;
    temp->t=time;
    temp->next=NULL;
    if(it==NULL){
        head->head=temp;
       
    }
    else{
        simulation_node *prev=NULL;
        while(it!=NULL &&it->t<time){
            prev=it;
            it=it->next;
        }
        if(prev==NULL){
            temp->next=head->head;
            head->head=temp;
        }
        else{
            temp->next=it;
        prev->next=temp;
        }
        
    }
}
simulation_node * find_remove(simulation_ll *head,int time){
    simulation_node *it=head->head;
    if(it==NULL){
        return NULL;
    }
    simulation_node *prev=NULL;
    while(it->next!=NULL && it->t<time){
        prev=it;
        it=it->next;
    }
    if(prev==NULL){
        if(it->t==time){
            head->head=head->head->next;
            return it;
        }
    }
    if(it->t==time){
        prev->next=it->next;
        return it;
    }
    return NULL;
    
}

void printll(simulation_ll head){
    printf("Printing the sumiulation list \n");
    simulation_node *it=head.head;
    while(it!=NULL){
        printf("%p %d \n",it->p,it->t);
        it=it->next;
    }
    printf("Exiting\n");
}
