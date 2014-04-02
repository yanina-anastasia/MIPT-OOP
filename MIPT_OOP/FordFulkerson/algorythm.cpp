#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "support.h"


Queue:: Queue() {
	color.resize(MAX_NODES);
	q.resize(MAX_NODES + 2);
}

FordFulkerson:: FordFulkerson() {
	vector <int> temp;
	temp.resize(MAX_NODES);
	capacity.assign(MAX_NODES, temp);
	flow.assign(MAX_NODES, temp);
	pred.resize(MAX_NODES);
}

void Queue:: enqueue (int arg) {
    q[tail] = arg;
    tail++;
    color[arg] = GRAY;
}

int Queue:: dequeue () {
    int arg = q[head];
    head++;
    color[arg] = BLACK;
    return arg;
}

int Queue:: bfs (int start, int target, FordFulkerson& graph) {
    for (int u = 0; u < VERTICE; u++) {
		color[u] = WHITE;
	}   
    head = tail = 0;
    enqueue(start);
    graph.pred[start] = -1;
    while (head != tail) {
		int u = dequeue();
		for (int v = 0; v < VERTICE; v++) {
			if (color[v] == WHITE && graph.capacity[u][v] - graph.flow[u][v] > 0) {
				enqueue(v);
				graph.pred[v] = u;
			}
		}
	}
    return color[target] == BLACK;
}

int FordFulkerson:: maxFlow (int source, int target, Queue& queue) {
    int maximumFlow = 0;
    for (int i = 0; i < VERTICE; i++) {
		for (int j = 0; j < VERTICE; j++) {
			flow[i][j] = 0;
		}
	}
    while (queue.bfs(source, target, *this)) {
		int increment = numeric_limits <int>:: max();
		for (int u = VERTICE - 1; pred[u] >= 0; u = pred[u]) {
			increment = std::min(increment, capacity[pred[u]][u] - flow[pred[u]][u]);
		}
		for (int u = VERTICE - 1; pred[u] >= 0; u = pred[u]) {
			flow[pred[u]][u] += increment;
			flow[u][pred[u]] -= increment;
		}
		maximumFlow += increment;
	}
	return maximumFlow;
}

vector <int> FordFulkerson:: addFlow() {
	string fromS, toS;
	cin >> fromS;
	cin >> toS;
	int from, to;
	if (fromS == "s") {
		from = 0;
	} else if (fromS == "v") {
		from = numeric_limits <int>:: max();
	} else {
		from = atoi(fromS.c_str());
		if (from > VERTICE) {
			VERTICE = from;
		}	
	}
	if (toS == "s") {
		to = 0;
	} else if (toS == "v") {
		to = numeric_limits <int>:: max();
	} else {
		to = atoi(toS.c_str());
		to;
		if (to > VERTICE) {
			VERTICE = to;
		}
	}
	int flow;
	cin >> flow;
	vector < int > temp;
	if (flow >= 0) {
		temp.push_back(from);
		temp.push_back(to);
		temp.push_back(flow);
		
	} else {
		temp.push_back(-1);
	}
	return temp;
}

int main() {
	freopen("input.txt", "rt", stdin);
	freopen("output.txt", "wt", stdout);
	FordFulkerson graph = FordFulkerson();
	Queue supportQueue = Queue();
	ifstream ifs;
	string line;
	ifs.open("input.txt");
	if(!ifs.is_open())
		cout << "Error opening input.txt";
	vector < int > input;
	vector < vector < int > > cap;
	while (getline(ifs,line)) {
		input = graph.addFlow();
		if (input[0] == -1) {
			cout << "Flow can not be below zero" << endl;
		}
		cap.push_back(input);
	}
	ifs.close();
	int start = 0;
	int destination = 0;
	int curFlow = 0;
	for (int i = 0; i < cap.size(); i++) {
			start = cap[i][0];
			if (cap[i][1] == numeric_limits <int>:: max()) {
				destination = VERTICE + 1;
			} else {
				destination = cap[i][1];
			}
			curFlow = cap[i][2];
			graph.capacity[start][destination] = curFlow;
	}
	int n = VERTICE++;
	VERTICE ++;
	cout << graph.maxFlow(0, n, supportQueue) << endl;
	VERTICE--;
	for (int i = 0; i < cap.size(); i++) {
		char start, destination;
		if (cap[i][0] == 0) {
			start = 's';
		} else if (cap[i][0] == numeric_limits <int>:: max()) {
			start = 'v';
		} else {
			string str;
			ostringstream temp;
			temp << cap[i][0];
			start = temp.str()[0];
		}
		if (cap[i][1] == 0) {
			destination = 's';
		} else if (cap[i][1] == numeric_limits <int>:: max()) {
			destination = 'v';
		} else {
			string str;
			ostringstream temp;
			temp << cap[i][1];
			destination = temp.str()[0];
		}
		if (cap[i][1] == numeric_limits <int>:: max()) {
			cap[i][1] = VERTICE;
		}
		if (graph.capacity[cap[i][0]][cap[i][1]] != 0) {
			cout << start << " "<< destination << " " << graph.flow[cap[i][0]][cap[i][1]] << "/" << graph.capacity[cap[i][0]][cap[i][1]] << endl;
		}
	}
	return 0;
}
