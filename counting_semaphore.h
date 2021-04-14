#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

//The idea behind this is to have two semaphore structs
//one struct is for the correct implementation the other for the bad
//same ideas with the P and V functions
//g means good b means bad

//good counting semaphore struct
typedef struct c_sem{
	//good c_sem val initialized to k, wait to the min between 1 and val
	//bad c_sem val initialized to k, wait to 0
	//mutex for both initialized to 1
	int val;
	sem_t wait;
	pthread_mutex_t mutex;
}c_sem;

//Functions to initialize semaphores
void initialize_sem(c_sem * cs, int * val, char * name);

//function to destroy the semaphore when the run is finished 
void delete_sem(c_sem * cs);

//min function representing the function in the psuedocode
int min(int a, int b);

int max(int a, int b);

//good P function
void wait_good(c_sem * cs);

//good V function
void signal_good(c_sem * cs);
