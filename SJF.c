#include "assignment3.h"


void insertbuffer(buffer_t value) {
    if (buffer_index < SIZE) {
        buffer[buffer_index++] = value;
    } else {
        printf("Buffer overflow\n");
    }
}

buffer_t dequeuebuffer() {
    if (buffer_index > 0) {
        return buffer[--buffer_index]; // buffer_index-- would be error!
    } else {
        printf("Buffer underflow\n");
    }
    return 0;
}
 
//Method for the consumer threads may need a thread number
void consumer(int instances){
	pthread_t thread_id[instances];
	int index;
	for(index = 0; index < instances; ++index){
		pthread_create(&thread_id[index], NULL, consume, NULL);
	}
	
	for(index = 0; index < instances; ++index){
		pthread_join(thread_id[index], NULL);
	}
}

void consume(void *vargp){
	//After the consumer sleeps for a random amount of time, look in the queue for the smallest job
	//After finding the smallest job avalible, set the semaphores to allocate it to this thread
	//Not quite sure what the actual processing of the thread should look like since all we have is the size value, maybe a sleep with a random?
	//After the thread finishes, mark the item as completed somehow and repeat
	double timer = 0.1 * (rand() % 10);
	sleep(timer);
}

//Method for the producer producer processes to call after a fork
void producer(int instances){
	int index;
	for(index = 0; index < instances; ++i){
        if(fork() == 0){
			//DO SHIT
        }
    }
}

int main(int argc, char * argv[]){
	//initializes everything starts the consumer threads and producer processes
	//Command line arguments:number of (user)producer processes, and the number consumer threads 
	int producer_processes;
    int consumer_threads;

   	//ask for the number of producer processes and consumer threads 
   	producer_processes = argv[1];
    consumer_threads = argv[2];
	
	producer(producer_processes);
	consumer(consumer_threads);
	return 0;
}