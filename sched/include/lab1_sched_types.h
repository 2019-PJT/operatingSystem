/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 32174224, 32157034
*	    Student name : Cho min seo, Kwon bo yoon
*
*   lab1_sched_types.h :
*       - lab1 header file.
*       - must contains scueduler algorithm function's declations.
*
*/

#ifndef _LAB1_HEADER_H
#define _LAB1_HEADER_H


/*
 * You need to Declare functions in  here
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
#define SIZE 5   

typedef struct PCB{
   int pid;
   int completion;// finish time
   int arrival;
   int firstrun; //start time
   int turnaround; //turnarround time
   int response; // respense time
   int performT; // 
   int work; //service time
   int ticket; //lottery
   int remain; //time remain
   int waiting;// for complecation. 
   int Priority;
}PCB;

/* global variable { */
struct PCB   *queue[SIZE],   // queue
            task[SIZE];      // task
int qt,      // queue top index
   ql;      // queue last index

   
PCB *Pop();         // queue pop
void push( PCB *);      // queue push
void print_table(int *,int number);      // shceduling table print
void sortByPrt();

void RR_P();
void FIFO(int number);
void Lottery(int number);
void mlfq(int number); 
void Create_process(int number);
void result_make_process(int number, PCB * cprocess);
void Start_time(PCB * copy_process, int work_time, int number);
void Response_time(PCB * copy_process, int number);
void TurnAround_time(PCB * copy_process, int number);
int Sum_Work_time(PCB * copy_process, int number);
PCB * process;
int number;

#endif /* LAB1_HEADER_H*/



