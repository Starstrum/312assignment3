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
void* consumer(){

}

//Method for the producer producer processes to call after a fork
//may need a process number 0 - how ever many
void producer(){

}

int main(int argc, char * argv[]){
	//initializes everything starts the consumer threads and producer processes
	//Command line arguments:number of (user)producer processes, and the number consumer threads 
	
	return 0;
}