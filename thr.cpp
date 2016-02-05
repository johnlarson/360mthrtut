#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <iostream>
#include <semaphore.h>

sem_t mutex, empty, full;

class SocketQueue {
	std::queue<int> stlqueue;
	public:
		void push(int sock) {
			sem_wait(&empty);
			sem_wait(&mutex);
			stlqueue.push(sock);
			sem_post(&mutex);
			sem_post(&full);
		}
		int pop() {
			sem_wait(&full);
			sem_wait(&mutex);
			int rval = stlqueue.front();
			stlqueue.pop();
			sem_post(&mutex);
			sem_post(&full);
			return rval;
		}
	
} sockQueue;

void* howdy(void* arg) {
	std::cout << "GOT " << sockQueue.pop() << std::endl;
	//int tid;
	//tid = (long)arg;
	//printf("Hi %d\n", tid);
}

main() {
	#define NTHREADS 10
	#define NQUEUE 20
	sigaction(SIGINT, &signew, &sigold);
	sigaction(SIGHUP, &signew, &sigold);
	sigaction(SIGPIPE, &signew, &sigold);
	long threadid;
	pthread_t threads[NTHREADS];
	sem_init(&mutex, PTHREAD_PROCESS_PRIVATE, 1);
	sem_init(&full, PTHREAD_PROCESS_PRIVATE, 0);
	sem_init(&empty, PTHREAD_PROCESS_PRIVATE, NQUEUE);
	for(int i = 0; i < NQUEUE; i++) {
		sockQueue.push(i);
	}
	for(threadid = 0; threadid < NTHREADS	; threadid++) {
		pthread_create(&threads[threadid], NULL, howdy, (void*)threadid);
	}
	pthread_exit(NULL);

}
