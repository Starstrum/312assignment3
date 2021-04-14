#include "counting_semaphore.h"

//The method to initialize the counting semaphore in the correct way
void initialize_sem(c_sem *cs, int  k){
	cs->val = *k;
	pthread_mutex_init(&cs->mutex,NULL);
	sem_init(&cs->wait,0,min(1,k));
}

//Method to destroy our structure
void delete_sem(c_sem *cs){
	sem_destroy(&cs->wait);
	pthread_mutex_destroy(&cs->mutex);
}

//The good wait and signal methods
void wait_good(c_sem * cs){
	sem_wait(&cs->wait);
	pthread_mutex_lock(&cs->mutex);
	cs->val = cs->val - 1;
	if(cs->val > 0){
		sem_post(&cs->wait);
	}
	pthread_mutex_unlock(&cs->mutex);
}

void signal_good(c_sem * cs){
	pthread_mutex_lock(&cs->mutex);
	cs->val = cs->val + 1;
	if(cs->val == 1){
		sem_post(&cs->wait);
	}
	pthread_mutex_unlock(&cs->mutex);
}


//simple max function
int max(int a, int b){
    if(a > b){
        return a;
    }
    else{
        return b;
    }
}

//function to find the minimal value of 2 numbers
int min(int a, int b){
    if(a < b){
        return a;
    }
    else{
        return b;
    }
}
