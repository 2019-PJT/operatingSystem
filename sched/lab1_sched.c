/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32174224. 32157034
*	    Student name : Cho min seo, Kwon bo yoon
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm function'definition.
*
*/

#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#include "lab1_sched_types.h"

/*
 * you need to implement FCFS, RR, SPN, SRT, HRRN, MLFQ scheduler. 
 */




// queue pop
 PCB *Pop(){
    PCB *task_top = queue[qt];
   queue[qt] = NULL;
   if(++qt >= SIZE) qt = 0;
   return task_top;
}

// queue push
void push( PCB* task_one){
   queue[ql] = task_one;
   if(++ql >= SIZE) ql = 0;
}

// scheduling table print
void print_table(int arr[],int number){
   int i=0,j,asize=0;
   int max =Sum_Work_time(process,number);
   for(;i<number;i++){
      printf("      %d ",task[i].pid);
      for(j=0;j<max;j++){
         if(task[i].pid == arr[j]){
            printf("> ");
         } else {
            printf("/ ");
         }
      }
      printf("\n");
   }
}
void FIFO( int number){

      PCB *copy_process = malloc(sizeof(PCB)*number);
      memcpy(copy_process,process,sizeof(PCB)*number);
      PCB temp;
      int j, k,work_time;
      for( j = 0; j < number -1 ; j++){
         for( k = j+1; k < number; k++ ){
            if(copy_process[j].arrival > copy_process[k].arrival){
                  temp = copy_process[j];
                  copy_process[j] = copy_process[k];
                  copy_process[k] = temp; 
            }
         }
    }

    work_time= Sum_Work_time(copy_process,number);
    Start_time(copy_process,work_time, number);
    TurnAround_time(copy_process, number);
    Response_time(copy_process,number);
      
    printf("\n      FIFO\n");
    result_make_process(number, copy_process);

}

void Lottery(int number){
    PCB *copy_process = malloc(sizeof(PCB)*number);
    memcpy(copy_process,process,sizeof(PCB)*number);
    int array[number],i,sum=0,temp_num, temp_num2;
    int  repeat=1000,cnt[number][repeat],temporay[number];

    int check[number];

    for(i=0; i<number; i++){
        array[i]=copy_process[i].ticket/number;
        sum+=array[i];
       
    }
    for(i=0; i<number; i++){
        for(int j=0; j<repeat; j++)
        cnt[i][j]=0;
    }


    for(i=0; i<number; i++){
        temp_num = array[i];
        array[i]=(temp_num*100/sum);   
   
        if(i>0){
         int num;
         temporay[i]=array[i];
         num=i;
         while(num>0){
               temporay[i]+=array[num-1];
            num--;
            }
        }
    }
   sum=0;
   for(i=0; i<number; i++){
        sum+=array[i];
        check[i]=copy_process[i].work;
   }
   for(i=1; i<number; i++){
      array[i]=temporay[i];
   }

    for(int j =0; j<repeat; j++){
        int pick=rand()%sum;
        for(i=0; i<number; i++){
           if(i==0){
            if(pick<=array[i])
               cnt[i][j]++;
         }
         else{
            if(pick>array[i-1] && pick<=array[i])
               cnt[i][j]++;
         }
      }        
   }

      PCB temp;
      for(int j = 0; j < number -1 ; j++){
         for(int k = j+1; k < number; k++ ){
            if(copy_process[j].arrival > copy_process[k].arrival){
                  temp = copy_process[j];
                  copy_process[j] = copy_process[k];
                  copy_process[k] = temp; 
            }
         }
      }
    for(i=0; i<repeat; i++){
        for(int j=0; j<number; j++){
            if(copy_process[j].arrival<=i&&copy_process[j].waiting==0){
                // it is for find start && response time of pro
                copy_process[j].firstrun= i ;
                copy_process[j].response= i- copy_process[j].arrival;
                if(cnt[j][i]!=0){
                    if(check[j]!=0)//it is for check thereis left work
                        check[j]--;
                    copy_process[j].waiting++;
                }
            }

            else if(copy_process[j].arrival<=i&&copy_process[j].waiting!=0){
                if(cnt[j][i]!=0){
                    if(check[j]!=0)
                        check[j]--;
                    else if(check[j]==0)
                        copy_process[j].turnaround= i-copy_process[j].response;
                }
            }
        }
    }
    for(int j = 0; j < number -1 ; j++){
         for(int k = j+1; k < number; k++ ){
            if(copy_process[j].pid > copy_process[k].pid){
                  temp = copy_process[j];
                  copy_process[j] = copy_process[k];
                  copy_process[k] = temp; 
            }
         }
      }
   
   printf("\n      LOTTERY\n");   

    for(i=0; i<number; i++){
      if(i==0)
         printf("      pid [%d] ticket : 0 ~ %d\n",i,array[i]);
      else
         printf("      pid [%d] ticket : %d ~ %d\n",i,array[i-1],array[i]);
   }

    result_make_process(number,copy_process);
      
}

void sortByPrt(){
   int now = ql-1,prev;
    PCB *temp;
   if(now < 0) now = SIZE-1;

   // order by priority 
   while(1){
      prev = now-1;
      if(prev < 0) now = SIZE-1;
      if(!queue[prev] || queue[prev]->Priority == queue[now]->Priority || prev == qt) break;
      if(queue[prev]->Priority > queue[now]->Priority){
         temp = queue[now];
         queue[now] = queue[prev];
         queue[prev] = temp;
      }
      now = prev;
   }

}
void RR_P() 
{ 
 
  int cnt,j,n,time,remain,flag=0,timeslice; 
  int wait_time=0,turnaround_time=0,a[5]= {1, 4, 6, 7, 8},b[5] = {5, 7 , 2, 6, 8},r[20]; 
  n = number;
  remain=n; 
  for(cnt=0;cnt<n;cnt++) 
  { 
    r[cnt]=b[cnt]; 
  } 
  
  printf("\tEnter Time slice:\t"); 
  scanf("%d",&timeslice); 
  printf("\n\n      Process\t|Arrival Time\tBoust time\tTurnaround Time    Response Time\n\n"); 
  for(time=0,cnt=0;remain!=0;) 
  { 
    if(r[cnt]<=timeslice && r[cnt]>0) 
    { 
      time+=r[cnt]; 
      r[cnt]=0; 
      flag=1; 
    } 
    else if(r[cnt]>0) 
    { 
      r[cnt]-=timeslice; 
      time+=timeslice; 
    } 
    if(r[cnt]==0 && flag==1) 
    { 
      remain--; 
      printf("      P[%d]\t|\t%d\t|\t%d\t|\t%d\t|\t%d\n",cnt+1,a[cnt],b[cnt],time-a[cnt],time-a[cnt]-b[cnt]); 
      wait_time+=time-a[cnt]-b[cnt]; 
      turnaround_time+=time-a[cnt]; 
      flag=0; 
    } 
    if(cnt==n-1) 
      cnt=0; 
    else if(a[cnt+1]<=time) 
      cnt++; 
    else 
      cnt=0; 
  } 
  printf("\n      Average Waiting Time= %f\n",wait_time*1.0/n); 
  printf("      Avg Turnaround Time = %f",turnaround_time*1.0/n); 
  
}



// multi level feedback queue
void mlfq(int number){
   qt=ql=0;
   int i=0,
      kill_count = 0,
      svc_t = 0,
      next = 0;
  int max= Sum_Work_time(process,number);
 int in[max]; 
    memcpy(task,process,sizeof(PCB)*number);
   
   PCB temp;

   for(int j = 0; j < number -1 ; j++){
         for(int k = j+1; k < number; k++ ){
            if(task[j].arrival > task[k].arrival){
                  temp = task[j];
                  task[j] = task[k];
                  task[k] = temp; 
            }
         }
    }

   printf("\n\n      MLFQ\n\n");
   PCB *now =&task[next++];
 
   push(now);

   
   while(kill_count < number){
      while(svc_t< task[next].arrival){
         svc_t++;
      }

      if(next<number && task[next].arrival < svc_t){
         now = &task[next++];
      } else {
         now = Pop();
      }
     
     if(now && now->response == -1&&now->arrival<=i)
     { 
        now->response = svc_t - now->arrival;
     }
      in[svc_t++] = now->pid;
      now->Priority++;

      if(--now->work <= 0){
         kill_count++;
         now->turnaround = svc_t - now->response;
      } else {
         push(now);
         sortByPrt();
      }
    task[next].response=now->response;
    task[next].turnaround=now->turnaround;
    task[next].firstrun= now->response + now->arrival;
    
   }
   int temp_Re[5]={0,};
   for(int i=0; i<max; i++){
      for(int j=0; j<number; j++){
         if ( task[j].pid==in [i]){
               ++temp_Re[j];
         if(temp_Re[j] ==1){
            task[j].firstrun=i;
            task[j].response= i- task[j].arrival;
            if(temp_Re[j] ==task[j].work)
               task[j].turnaround=1;
         }
         else if(temp_Re[j]==task[j].work)
            task[j].turnaround= i-task[j].firstrun;
         }
         task[j].work=process[j].work;
      }
   }
   
  print_table(in,number);
  result_make_process(number,task);
}

void Create_process(int number){

   int i =0;
   srand(time(NULL));

   printf("make process....");
   process = malloc(sizeof(PCB)*number);

   for(i = 0; i < number; i++){
      process[i].pid = i +1;
      process[i].arrival = rand()%5;
      process[i].work = rand()%5+1;
      process[i].ticket = (rand() %20+1)*5;
      process[i].remain = process[i].work;// 
      process[i].turnaround = 0;
      process[i].completion =0;
      process[i].firstrun=0;
      process[i].response = 0;
      process[i].waiting=0;
      process[i].Priority = 0;
      process[i].performT =0;
      
   }
    result_make_process(number, process);

}

void result_make_process(int number, PCB *cprocess)
{   int i ;
    printf("\n     ---------------------------------------------------------------\n");
    printf("\n     Pid    Arrive   Start   ticket   Response  Turnaround  Work  ");
        printf("\n     ---------------------------------------------------------------\n");

    for (i = 0 ; i < number; i++){
        printf("     %2d %8d %8d %7d %10d %10d %8d  \n",
              cprocess[i].pid, cprocess[i].arrival, cprocess[i].firstrun,cprocess[i].ticket, cprocess[i].response,
               cprocess[i].turnaround, cprocess[i].work);
   }
    printf("     ---------------------------------------------------------------\n");
    printf("\n");
}
void Response_time(PCB * copy_process, int number){
    for(int i=0; i<number; i++){
        int start = copy_process[i].firstrun;
        int arrive =copy_process[i].arrival;
        int respon= start- arrive;
        copy_process[i].response= respon;
    }
}

void Start_time(PCB * copy_process, int work_time, int number){
    int j=0 ,i=0;
    for(j=0; j<work_time; j++){//i need to make it to function //itis for start;
        for(i=0; i<number; i++){
            copy_process[i].firstrun=j;
            if(j>= copy_process[i].arrival)
            {
                j+= copy_process[i].work;
            }
        }
    }
}
    // it is for turn around
void TurnAround_time(PCB * copy_process, int number){
    for(int i=0; i<number; i++){
        int work = copy_process[i].work;
        int start = copy_process[i].firstrun;
        int arrive =copy_process[i].arrival;
        int respon= start- arrive;;
        int turn = work + respon;
        copy_process[i].turnaround= turn;
    }
}

    //it is for Sum of all process work time
int Sum_Work_time(PCB * copy_process, int number){
    int i=0, time=0;
    for(i=0; i<number; i++){
        time += copy_process[i].work;
    }
    return time;
}

