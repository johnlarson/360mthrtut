thread: thr.cpp
	g++ -std=c++11 -pthread main.cpp thr.cpp server.cpp utils.cpp -o server
