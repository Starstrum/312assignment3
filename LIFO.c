#include "assignment3.h"

//Counting semaphores from assignment 2 
c_sem * fullsem, * emptysem;
pthread_mutex_t * mutex;
 pthread_mutexattr_t * attr ;
int * fullval;
int * emptyval;
int * queue_index;

//print queue declarations
print_request * print_queue;

//shared memory segment for the semaphores and buffer 
int queue_id, fullsem_id,emptysem_id,mutex_id,queue_index_id;
int status = 0;

pid_t pid, wpid;
//The method for the signal handler 
//detach the semaphores and destroy them
void signal_handler(){
    printf("\ndelete sem calls\n");
    delete_sem(fullsem);
    delete_sem(emptysem);
    pthread_mutex_destroy(mutex);
	//detach the shared memory  and then destroy it
    printf("detach calls \n");
	shmdt(fullsem);
	shmdt(emptysem);
	shmdt(mutex);
	//shmdt(fullval_id);
	//shmdt(emptyval_id);
    shmdt(print_queue);
    printf("ctl calls\n");
	//destroy the shared memory
	shmctl(fullsem_id,IPC_RMID, NULL);
	shmctl(emptysem_id,IPC_RMID, NULL);
	shmctl(mutex_id,IPC_RMID, NULL);
	//shmctl(fullval_id,IPC_RMID);
	//shmctl(emptyval_id,IPC_RMID);
    shmctl(queue_id,IPC_RMID, NULL);
    printf("waiting for children\n");
    while ((wpid = waitpid(-1,&status, 0)) > 0); 
    exit(0);
}

void insertbuffer(print_request value) {
    if (*queue_index < SIZE) {
        printf("inserting into the queue \n");
        print_queue[*(queue_index)++] = value;
    } else {
        printf("Buffer overflow\n");
    }
}

print_request dequeuebuffer() {
    if (queue_index > 0) {
        return print_queue[--*queue_index]; // buffer_index-- would be error!
    } else {
        printf("Buffer underflow\n");
    }
    print_request null_request;
    printf("returning null request\n");
    return null_request;
}
 
//Method for the consumer threads may need a thread number 
//must read and take out from the buffer
void * printer(void * thread_n){
    int thread_numb = *(int *)thread_n;
    print_request request;
    int i = 0;
    int consumer_loops = rand() % 25;
    while (i++ < consumer_loops) {
        wait_good(emptysem);
        /* there could be race condition here, that could cause
           buffer underflow error */
        pthread_mutex_lock(mutex);
        request = dequeuebuffer();
        pthread_mutex_unlock(mutex);
        signal_good(fullsem); // post (increment) fullbuffer semaphore
        printf("Printer %d dequeue %d from buffer\n", thread_numb, request.size);
   }
   pthread_exit(0);
}

//Method for the producer producer processes to call after a fork
//may need a process number 0 - how ever many
//need to refacter to add things to the print queue rn this is just copied from assign2
void user(int user_number){
    printf("entering user process\n");
	int thread_numb = user_number;
    print_request request;
    request.pid = getpid();
    int i=0;
    int producer_loops = rand() % 25;
    while (i++ < producer_loops) {
        sleep(rand() % 10);
        request.size = rand() % (1000 - 100 + 1) - 100;
        wait_good(fullsem);

        pthread_mutex_lock(mutex); 
        insertbuffer(request);
        pthread_mutex_unlock(mutex);

        signal_good(emptysem);
        printf("user %d added %d to buffer\n", user_number, request.size);
    }
	//exit after the process is finished so it doesn't return back to the loop
	exit(0);
}

//A method to get and attach the sharedmemory for the main process and child process
void get_shm(){
	//get the shared memory ids will write statements to check the return values
	fullsem_id = shmget(FULLSEMKEY,sizeof(c_sem),IPC_CREAT|0660);
    if(fullsem_id < 0){
        printf("shmget error 1\n");
        exit(1);
    }
	emptysem_id = shmget(EMPTYSEMKEY,sizeof(c_sem),IPC_CREAT|0660);
    if(emptysem_id < 0){
        printf("shmget error 2\n");
        exit(1);
    }
	mutex_id = shmget(MUTEXKEY,sizeof(c_sem),IPC_CREAT|0660);
    if(mutex_id < 0){
        printf("shmget error 3\n");
        exit(1);
    }

    queue_id = shmget(QUEUEKEY,sizeof(print_request)*SIZE,IPC_CREAT|0660);
    if(queue_id < 0){
        printf("shmget error 4\n");
        exit(1);
    }
    queue_index_id = shmget(POSKEY,sizeof(int),IPC_CREAT|0660);
    if(queue_index_id < 0){
        printf("shmget error 5\n");
        exit(1);
    }
	//attach the shared memory to the process 
	fullsem = shmat(fullsem_id, NULL, 0);
    if(fullsem < 0){
        printf("shmat error\n");
        exit(1);
    }
	emptysem = shmat(emptysem_id, NULL, 0);
     if(emptysem < 0){
        printf("shmat error\n");
        exit(1);
    }
	mutex = shmat(mutex_id, NULL, 0);
    if(mutex < 0){
        printf("shmat error\n");
        exit(1);
    }

    print_queue = shmat(queue_id, NULL, 0);
    if(print_queue < 0){
        printf("shmat error\n");
        exit(1);
    }
    queue_index = shmat(queue_index_id, NULL, 0);
    if(queue_index < 0){
        printf("shmat error\n");
        exit(1);
    }
}

int main(int argc, char * argv[]){
	//initializes everything starts the consumer threads and producer processes
	//Command line arguments:number of (user)producer processes, and the number consumer threads 1
    //number of producer and consumer threads
    int user_processes;
    int printer_threads;

    printf("nutz\n");
    if (pthread_mutexattr_setpshared(attr, 5) < 0){
        printf("nuts\n");
    }
 	//get and attach shared memory
 	//according to the linux.die.net documentation after a fork the child inherits shared memory
 	get_shm();
    //create semaphores
    initialize_sem(fullsem,SIZE);
    initialize_sem(emptysem,0);
    pthread_mutex_init(mutex,attr);
    //call the signal handler 
    signal(SIGINT, signal_handler);
   	//ask for the number of producer processes and consumer threads 
   	user_processes = (int)  (* argv[1]) - '0';
    printer_threads = (int) (* argv[2]) - '0';
    printf("argument 1 %s \n", argv[1]);
    printf("argument 2 %s \n", argv[2]);
    printf("user_processes %d \n", user_processes);
    printf("printer_threads %d \n",printer_threads);

    queue_index = 0;

    //loop to create child processes 
    for(int i = 0; i < user_processes; i++){
    	pid = fork();
    	//if the pid is less than 0 there was an erro 
    	if (pid < 0 ){
    		printf("Forking error");
    	}
    	//if pid is 0 then go into the producer function for the child
    	//this should make it so the children do not fork 
    	if(pid == 0){
    		user(i);
    	}
    	else{
            //else if still in the parent process create the threads 
            //create and join threads
            pthread_t thread[printer_threads];
            int thread_numb[printer_threads];
            int i;
            for (i = 0; i < printer_threads; i++) {
                thread_numb[i] = i;
                pthread_create(&thread[i], NULL, printer, &thread_numb[i]); 
            }
            for (i = 0; i < printer_threads; i++)
                pthread_join(thread[i], NULL);
    	}
    }
	return 0;
}