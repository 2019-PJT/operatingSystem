/*
*Operating System Lab
*	    Lab (Scheduler Algorithm Simulator)
*	    Student name : Cho min seo, Kwon bo yoon
*
*   lab_sched.c :
*       - source file.
*       - Must contains scueduler algorithm test code.
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

#include "sched_types.h"



int main(){
  
   while(1){
      printf("process number : ");
      scanf("%d",&number);
      if(number <=5){
         break;
      }
      
   }
    Create_process(number);

    FIFO(number);
    Lottery(number);
    RR_P();
    mlfq(number);


}


