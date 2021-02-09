#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "header.h"

int tot=1<<28; 
int size=56;

int main(){
    init();
    simulation_ll sim;
    srand(time(NULL));
    sim.head=NULL;
    int time_max=8000;
    int allocation_max=tot;
    int i=0;
    float avg_internal_frag=0.0;
	//menu_program();
    printf("Running the Simulation Uniform\n");
    // printf("Time \tInternal Fragmentation \t Allocation Requests \t Deallocation Requests\t  Splits \t Recombinations\n");
    while(i<=time_max+1){
        if(i%200==0 &&i!=0){
            if(allocated_area!=0){
                avg_internal_frag+=(float)internal_fragmentation/sum_sizes;
            // printf("%d\t\t%f\t\t",i,(float)internal_fragmentation/sum_sizes);
            // printf("%ld \t\t %ld\t\t",allocation_request,deallocation_requests);
            // printf("%ld \t\t %ld\n",area_split,buddy_recombinations);
            }


        }
        //if(i<time_max/2){
            
        int time=uniform_distribution(0,100);
        int request=expo(100,2000)	;
        // printf("REquest is of %ld\n",request);
        void *p=myalloc(request);
        // printf("p is %u\n",p);
        if(p!=NULL){
            if((unsigned)p<(unsigned)memory){
            printf("not possible at all\n");
        }
            else{
                insert_sim(&sim,p,i+time);
            }
        }
        
        //}
        while(1){
            simulation_node *it=find_remove(&sim,i);
            
            if(it==NULL){
                break;
            }
            else{
                freem((void *)it->p);
            }
        }
        i++;
    }
    printf("Average internal frag %f\n",avg_internal_frag/(time_max/200));
    return 0;
}
