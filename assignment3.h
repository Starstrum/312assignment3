#include "counting_semaphore.h"
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
//This is the header file for assignment 3 
//all methods, structs and definitions for the assignment the counting_semaphore.h file includes everything needed already

//Names for using shared memory semaphores 
//sem_open can be used to call and create theses semaphores
#define FULLSEM "full_sem"
#define EMPTYSEM "empty_sem"
#define MUTEX "mutex_lock"
#define VALUESHM "val"

//keys for the needed shared memory
#define FULLSEMKEY 2673
#define EMPTYSEMKEY 3467
#define MUTEXKEY 1023
#define FULLVALUEKEY 0968
#define EMPTYVALUEKEY 4231
#define QUEUEKEY 1211

//Size of the buffer 
#define SIZE 25

//buffer element struct for the pid and size
typedef struct{
	pid_t pid;
	int size;
}print_request;

void producer(int i);
void get_shm();