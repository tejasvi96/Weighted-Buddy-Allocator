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
// int tot=1<<28; 
int twos[57];
int nums[60];
// int size=56;
typedef struct block block;
long int internal_fragmentation,allocation_request,deallocation_requests,area_split,buddy_recombinations,sum_sizes,allocated_area;
block arr[1<<28];
list asl[57];
par_list par;
void init();





void *myalloc(int reqsize);


void freem(void *p);
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

int uniform_distribution(long int rangeLow,int rangeHigh);
int expo(int low,int high);
void insert_sim(simulation_ll *head,void * request,int time);
simulation_node * find_remove(simulation_ll *head,int time);
