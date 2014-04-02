#include <vector>

#define WHITE 0
#define GRAY 1
#define BLACK 2

using namespace std;

int VERTICE = 0;
int MAX_NODES = 100;

class FordFulkerson;
class Queue;

class Queue {
public:
	int head;
	int tail;
	vector <int> q;
	vector <int> color; 

	Queue();
	void enqueue(int);
	int dequeue();
	int bfs(int, int, FordFulkerson&);
};

class FordFulkerson {
public:
	vector < vector <int> > capacity;
	vector < vector <int> > flow;
	vector <int> pred;

	FordFulkerson();
	int maxFlow(int, int, Queue&);
	vector <int> addFlow();
};
