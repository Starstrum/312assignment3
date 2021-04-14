#include "assignment3.h"

//Counting semaphores from assignment 2 
c_sem *fullsem;
c_sem *emptysem;
pthread_mutex_t * mutex;

int * fullval;
int * emptyval;

//print queue declarations
print_request print_queue[SIZE];
int queue_index;
int NUMB_THREADS;

//shared memory segment for the semaphores and buffer 
int queue_id;
int fullsem_id ;
int emptysem_id;
int mutex_id;
int fullval_id;
int emptyval_id;

//The method for the signal handler 
//detach the semaphores and destroy them
void signal_hander(){
	//detach the shared memory  and then destroy it 	
	shmdt(fullsem_id);
	shmdt(emptysem_id);
	shmdt(mutex_id);
	shmdt(fullval_id);
	shmdt(emptyval_id);
    shmdt(queue_id);
	//destroy the shared memory
	shmctl(fullsem_id,IPC_RMID);
	shmctl(emptysem_id,IPC_RMID);
	shmctl(mutex_id,IPC_RMID);
	shmctl(fullval_id,IPC_RMID);
	shmctl(emptyval_id,IPC_RMID);
    shmctl(queue_id,IPC_RMID);
}

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
//must read and take out from the buffer
void* consumer(int * thread_numb){

}

//Method for the producer producer processes to call after a fork
//may need a process number 0 - how ever many
//need to refacter to add things to the print queue rn this is just copied from assign2
void producer(int process_number ){
	 int thread_numb = *(int *)thread_n;
    buffer_t value;
    int i=0;
    while (i++ < PRODUCER_LOOPS) {
        sleep(rand() % 10);
        value = rand() % 100;
        wait_good(&full_sem);

        pthread_mutex_lock(&buffer_mutex); 
        insertbuffer(value);
        pthread_mutex_unlock(&buffer_mutex);

        signal_good(&empty_sem);
        printf("Producer %d added %d to buffer\n", thread_numb, value);
    }
	//exit after the process is finished so it doesn't return back to the loop
	exit(0);
}

//A method to get and attach the sharedmemory for the main process and child process
void get_shm(){
	//get the shared memory ids will write statements to check the return values
	fullsem_id = shmget(FULLSEMKEY,sizeof(c_sem),IPC_CREAT|0660);
	emptysem_id = shmget(EMPTYSEMKEY,sizeof(c_sem),IPC_CREAT|0660);
	mutex_id = shmget(MUTEXKEY,sizeof(c_sem),IPC_CREAT|0660);
	fullval_id = shmget(FULLVALUEKEY,sizeof(int),IPC_CREAT|0660);
	emptyval_id= shmget(EMPTYVALUEKEY,sizeof(int),IPC_CREAT|0660);
    queue_id = shmget(QUEUEKEY,sizeof(print_request)*SIZE,IPC_CREAT|0660);
	//attach the shared memory to the process 
	fullsem = shmat(fullsem_id, NULL, 0);
	emptysem = shmat(emptysem_id, NULL, 0);
	mutex = shmat(mutex_id, NULL, 0);
	fullval = shmat(fullval_id, NULL, 0);
	emptyval = shmat(emptyval_id, NULL, 0);
    print_queue = shmat(queue_id, NULL, 0);
}

int main(int argc, char * argv[]){
	//initializes everything starts the consumer threads and producer processes
	//Command line arguments:number of (user)producer processes, and the number consumer threads 
    //number of producer and consumer threads
    pid_t pid;
    int producer_processes;
    int consumer_threads;

 	//get and attach shared memory
 	//according to the linux.die.net documentation after a fork the child inherits shared memory
 	get_shm();
    //create semaphores for the 
    fullsem = initialize_sem(fullsem,0, FULLSEM);
    emptysem = initialize_sem(emptysem,SIZE, EMPTYSEM);

    mutex = pthread_mutex_init(&mutex,NULL);
    //call the signal handler 
    signal(SIGINT, signal_handler);
   	//ask for the number of producer processes and consumer threads 
   	producer_processes = argv[1];
    consumer_threads = argv[2];

    buffer_index = 0;

    //loop to create child processes 
    for(int i = 0; i < producer_processes; i++){
    	pid = fork();
    	//if the pid is less than 0 there was an erro 
    	if (pid < 0 ){
    		printf("Forking error")
    	}
    	//if pid is 0 then go into the producer function for the child
    	//this should make it so the children do not fork 
    	if(pid == 0){
    		producer(i);
    	}
    	else{
    		//do nothing I think
    	}

    }

    //create and join threads
    pthread_t thread[consumer_threads];
    int thread_numb[consumer_threads];
    int i;
    for (i = 0; i < NUMB_THREADS; i++) {
       	thread_numb[i] = i;
       	pthread_create(&thread[i], NULL, consumer, &thread_numb[i]); 
    }
 
    for (i = 0; i < NUMB_THREADS; i++)
        pthread_join(thread[i], NULL);
		
	return 0;
}