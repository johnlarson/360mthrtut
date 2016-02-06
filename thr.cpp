#include "thr.h"

#define NQUEUE 100
#define ERROR -1

void sigHandler(int status);
void setSigStuff();
void* threadHandler(void* arg);

sem_t mutex, spaceOnQueue, workToDo;

class SocketQueue {
	std::queue<int> stlqueue;
	public:
		void push(int sock) {
			sem_wait(&spaceOnQueue);
			sem_wait(&mutex);
			stlqueue.push(sock);
			sem_post(&mutex);
			sem_post(&workToDo);
		}
		int pop() {
			sem_wait(&workToDo);
			sem_wait(&mutex);
			int rval = stlqueue.front();
			stlqueue.pop();
			sem_post(&mutex);
			sem_post(&spaceOnQueue);
			return rval;
		}
	
} sockQueue;

struct ThreadArgs {
	int id;
	string directory;
};

void server(int port, int threadNum, string directory) {
	setSigStuff();
	long threadid;
	pthread_t threads[threadNum];
	sem_init(&mutex, PTHREAD_PROCESS_PRIVATE, 1);
	sem_init(&workToDo, PTHREAD_PROCESS_PRIVATE, 0);
	sem_init(&spaceOnQueue, PTHREAD_PROCESS_PRIVATE, NQUEUE);
	for(threadid = 0; threadid < threadNum; threadid++) {
		struct ThreadArgs threadArgs;
		threadArgs.id = threadid;
		threadArgs.directory = directory;
		pthread_create(&threads[threadid], NULL, threadHandler, (void*)&threadArgs);
	}
	int sSocket;
	struct sockaddr_in address;
	setUpServerSocket(sSocket, address, port);
	int addressSize;
	while(1) {
		int pSocket = accept(sSocket, (struct sockaddr*)&address, (socklen_t*)&addressSize);
		sockQueue.push(pSocket);
	}
	pthread_exit(NULL);

}

void setSigStuff() {
	struct sigaction sigold, signew;
	signew.sa_handler=sigHandler;
	sigemptyset(&signew.sa_mask);
	sigaddset(&signew.sa_mask, SIGINT);
	signew.sa_flags = SA_RESTART;
	sigaction(SIGINT, &signew, &sigold);
	sigaction(SIGHUP, &signew, &sigold);
	sigaction(SIGPIPE, &signew, &sigold);
}

void sigHandler(int status) {
	printf("received signal %d\n", status);
}

void* threadHandler(void* arg) {
	struct ThreadArgs args = *((struct ThreadArgs*)arg);
	string root = args.directory;
	while(1) {
		int pSocket = sockQueue.pop();
		linger lin;
		unsigned int y=sizeof(lin);
		lin.l_onoff=1;
		lin.l_linger=10;
		setsockopt(pSocket,SOL_SOCKET, SO_LINGER,&lin,sizeof(lin));
		struct Request request = getRequest(pSocket);
		string path = joinPath(root, request.path);
		respond(pSocket, request.version, path, request.path);
		shutdown(pSocket, SHUT_RDWR);
		if(close(pSocket) == ERROR) {
			errorOut("Couldn't close message socket");
		}
	}
}
