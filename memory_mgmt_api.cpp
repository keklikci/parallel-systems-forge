#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <stdlib.h> 
#include <string>
#include <queue> 
#include <semaphore.h>
#include <assert.h>
using namespace std;

#define NUM_THREADS 10
#define MEMORY_SIZE 150

struct node
{
	int id;
	int size;
};


queue<node> myqueue; // shared que
pthread_mutex_t sharedLock = PTHREAD_MUTEX_INITIALIZER; // mutex
pthread_t server; // server thread handle
sem_t semlist[NUM_THREADS]; // thread semaphores

int thread_message[NUM_THREADS]; // thread memory information
char  memory[MEMORY_SIZE]; // memory size





void my_malloc(int thread_id, int size)
{
	//This function will add the struct to the queue
	// pthread_mutex_lock(&sharedLock); 
	node info;
	info.id = thread_id;
	info.size = size;
	myqueue.push(info);
	// pthread_mutex_unlock(&sharedLock); 
}

void * server_function(void *)
{
	//This function should grant or decline a thread depending on memory size.
	int accum_memory = 0;
	static int rem_memory = MEMORY_SIZE; // track remaining memory 
	// loop continously 
	while(true) {
		// pthread_mutex_lock(&sharedLock);
		if(myqueue.empty() == false) {
			node info = myqueue.front();
			myqueue.pop();
			int thread_ID = info.id;
			// grant or deny request 
			if(info.size <= rem_memory) {
				// start point of memory location
				thread_message[thread_ID] = accum_memory;
				accum_memory += info.size; // update accumulated 
				rem_memory = MEMORY_SIZE - accum_memory; // track remaining
			}
			// insufficient memory 
			else {
				thread_message[thread_ID] = -1;
			}
			pthread_mutex_unlock(&sharedLock);
			sem_post(&semlist[thread_ID]); // unblocks the semaphore for the requesting thread
		}
		pthread_mutex_unlock(&sharedLock);
	}
}

void * thread_function(void * id) 
{
	//This function will create a random size, and call my_malloc
	//Block
	int *thread_ID = (int*) id;
	int random_size = rand() % (MEMORY_SIZE / 6) + 1;
	pthread_mutex_lock(&sharedLock); 
	my_malloc(*thread_ID, random_size);
	pthread_mutex_unlock(&sharedLock); 
	// decrements the semaphore for that thread 
	sem_wait(&semlist[*thread_ID]); 
	pthread_mutex_lock(&sharedLock);
	if(thread_message[*thread_ID] == -1) {
		// cout << "Insufficient Memory for Thread " + to_string(*thread_ID) + "...";
		printf("Thread %d: Not enough memory\n", (*thread_ID));
	}
	else {
		// allocate all of them to the thread ID after casting
		for(int i=0; i<random_size; i++) {
			memory[thread_message[*thread_ID]+i] = '0' + static_cast<char>(*thread_ID);
		}
	}
	pthread_mutex_unlock(&sharedLock);
}

void init()	 
{
	pthread_mutex_lock(&sharedLock);	//lock
	for(int i = 0; i < NUM_THREADS; i++) //initialize semaphores
	{sem_init(&semlist[i],0,0);}
	for (int i = 0; i < MEMORY_SIZE; i++)	//initialize memory 
  	{char zero = '0'; memory[i] = zero;}
   	int server_create = pthread_create(&server,NULL,server_function,NULL); //start server 
	assert(server_create == 0);
	pthread_mutex_unlock(&sharedLock); //unlock
}



void dump_memory() 
{
// You need to print the whole memory array here.
printf("\nMemory Array - (5,30 matrix format):");
for(int mem=0; mem<MEMORY_SIZE; mem++) {
	if(mem % 30 == 0) {cout << endl;} // print in (5*30) matrix format 
	cout << memory[mem] << " "; 
}
}

int main (int argc, char *argv[])
{

 	//You need to create a thread ID array here
	int thread_IDS[NUM_THREADS];
	int init_ID = 0;
	while(init_ID != NUM_THREADS) {
		thread_IDS[init_ID] = init_ID;
		init_ID ++;
	}

 	init();	// call init

 	//You need to create threads with using thread ID array, using pthread_create()
	pthread_t threads[NUM_THREADS];
	init_ID = 0; 
	while(init_ID != NUM_THREADS) {
		int success_thread_create = pthread_create(&threads[init_ID], NULL, thread_function, (void*) &thread_IDS[init_ID]);
		assert(success_thread_create == 0);
		init_ID++;
	}

 	//You need to join the threads
	init_ID = 0;
	while(init_ID != NUM_THREADS) {
		int thread_success = pthread_join(threads[init_ID], NULL);
		assert(thread_success == 0);
		init_ID++;
	}

 	dump_memory(); // this will print out the memory
 	
 	printf("\nMemory Indexes:\n" );
 	for (int i = 0; i < NUM_THREADS; i++)
 	{
 		printf("[%d]" ,thread_message[i]); // this will print out the memory indexes
 	}
 	printf("\nTerminating...\n");
	return 0;
}