#include <iostream>
#include <list>
#include <vector>
#include <set>
#include <stack>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

#define INF 1e10

// Class representing a vertex in the graph
class Vertex {
private:
    string name;
    double latitude;
    double longitude;
    vector<Vertex*> edge;    // Adjacent vertices
    vector<double> weight;   // Corresponding edge weights

public:
    Vertex(const string& name, double latitude, double longitude);
    const string& getName() const;
    double getLatitude() const;
    double getLongitude() const;
    vector<double>& getWeight();
    vector<Vertex*>& getEdge();
    void addEdge(Vertex*);
    void addWeight(double);
};

// Constructor for the Vertex class
Vertex::Vertex(const string& name, double latitude, double longitude) : name(name), latitude(latitude), longitude(longitude) {}

// Getter functions for Vertex class
const string& Vertex::getName() const {
    return name;
}

double Vertex::getLatitude() const {
    return latitude;
}

double Vertex::getLongitude() const {
    return longitude;
}

vector<double>& Vertex::getWeight() {
    return weight;
}

vector<Vertex*>& Vertex::getEdge() {
    return edge;
}

// Add an adjacent vertex and corresponding weight to the current vertex
void Vertex::addEdge(Vertex* v) {
    edge.push_back(v);
}

void Vertex::addWeight(double w) {
    weight.push_back(w);
}

// Class representing a graph
class Graph {
private:
    list<Vertex*> vertices;

public:
    Graph(const string& filename);  // Constructor reads vertex data from a file and creates the graph
    list<Vertex*>& getVertices();
    vector<Vertex*> shortestPath(const string& sourceName, const string& sinkName, double& cost);
    double computeCost(const vector<Vertex*>& path) const;
    Vertex* findVertex(const string& name) const;
    int findIndex(const string& name) const;
};

// Constructor for the Graph class
Graph::Graph(const string& filename) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        exit(EXIT_FAILURE);
    }

    // Read vertex data from the file and create vertices
    string line;
    while (getline(inputFile, line)) {
        istringstream iss(line);
        string name;
        double latitude, longitude;
        getline(iss, name, ',');
        iss >> latitude >> longitude;

        Vertex* temp = new Vertex(name, latitude, longitude);
        vertices.push_back(temp);
    }

    // Create edges and assign weights based on geographical distances
    for (auto v : vertices) {
        for (auto u : vertices) {
            if (v == u) {
                continue;
            }

            double cur_distance = sqrt(pow(v->getLatitude() - u->getLatitude(), 2) + pow(v->getLongitude() - u->getLongitude(), 2));

            v->addWeight(cur_distance);
            v->addEdge(u);
        }
    }
}

// Getter function for vertices in the graph
list<Vertex*>& Graph::getVertices() {
    return vertices;
}

// Compute the total cost of a given path in the graph
double Graph::computeCost(const vector<Vertex*>& path) const {
    double cost = 0;

    for (int i = 1; i < path.size(); i++) {
        for (int j = 0; j < path[i - 1]->getEdge().size(); j++) {
            if (path[i]->getName() == path[i - 1]->getEdge()[j]->getName()) {
                cost = cost + path[i - 1]->getWeight()[j];
                break;
            }
        }
    }

    return cost;
}

// Find a vertex in the graph based on its name
Vertex* Graph::findVertex(const string& name) const {
    auto it = vertices.begin();

    for (; it != vertices.end(); it++) {
        Vertex* temp = *it;

        if (temp->getName() == name)
            return temp;
    }

    return *it;
}

// Find the index of a vertex in the graph based on its name
int Graph::findIndex(const string& name) const {
    int index = 0;

    for (auto it = vertices.begin(); it != vertices.end(); it++) {
        Vertex* temp = *it;

        if (temp->getName() == name)
            return index;

        index++;
    }

    return index;
}

// Find the shortest path between two vertices using Dijkstra's algorithm
vector<Vertex*> Graph::shortestPath(const string& sourceName, const string& sinkName, double& dist) {
    Vertex* curVertex;
    set<Vertex*> visited;
    vector<Vertex*> shortest;
    double* sdist = new double[vertices.size()];

    // Initialize distances to infinity
    curVertex = findVertex(sourceName);
    for (auto it = vertices.begin(); it != vertices.end(); it++) {
        Vertex* temp = *it;
        int index;
        index = findIndex(temp->getName());
        *(sdist + index) = INF;
    }
    *(sdist + findIndex(sourceName)) = 0;

    visited.insert(curVertex);

    // Dijkstra's algorithm
    while (visited.size() < vertices.size()) {
        vector<Vertex*> neighbors = curVertex->getEdge();
        vector<double> costs = curVertex->getWeight();
        int currentIndex = findIndex(curVertex->getName());

        for (int i = 0; i < neighbors.size(); i++) {
            double distance = costs[i] + *(sdist + currentIndex);
            int nIndex = findIndex(neighbors[i]->getName());

            if (distance < *(sdist + nIndex)) {
                *(sdist + nIndex) = distance;
            }
        }

        double cheapest = INF;
        int cheapIndex = -1;
        string cheapName;

        for (auto it = vertices.begin(); it != vertices.end(); it++) {
            Vertex* temp = *it;

            if (visited.count(temp) != 0)
                continue;

            if (cheapest > *(sdist + findIndex(temp->getName()))) {
                curVertex = temp;
                cheapIndex = findIndex(temp->getName());
                cheapest = *(sdist + cheapIndex);
                cheapName = temp->getName();
            }
        }

        visited.insert(curVertex);
    }

    // Reconstruct the path from sink to source
    int current = findIndex(sinkName);
    Vertex* currentVertex = findVertex(sinkName);

    while (currentVertex != findVertex(sourceName)) {
        string currentName = currentVertex->getName();
        shortest.push_back(findVertex(currentName));

        double cheapest = *(sdist + findIndex(currentName));
        vector<double> costs = currentVertex->getWeight();
        vector<Vertex*> neighbors = currentVertex->getEdge();

        for (int i = 0; i < neighbors.size(); i++) {
            double nCost = *(sdist + findIndex(neighbors[i]->getName()));

            if (cheapest - costs[i] == nCost) {
                currentVertex = neighbors[i];
                break;
            }
        }
    }

        // Add the source vertex to the shortest path
    shortest.push_back(findVertex(sourceName));

    // Set the output distance
    dist = *(sdist + findIndex(sinkName));

    return shortest;
}


int main() {
    double minCost;
    vector<Vertex*> shortestPath;

    // Create a graph by reading data from "palomarMap.txt" file
    Graph myGraph("palomarMap.txt");

    // Get user input for starting and ending locations
    string sourceLocation, sinkLocation;
    cout << "Enter starting location: ";
    cin >> sourceLocation;
    cout << "Enter ending location: ";
    cin >> sinkLocation;

    // Find the specified vertices
    Vertex* sourceVertex = myGraph.findVertex(sourceLocation);
    Vertex* sinkVertex = myGraph.findVertex(sinkLocation);

    // Check if the specified locations exist in the map
    if (sourceVertex && sinkVertex) {
        // Find the shortest path and print it
        shortestPath = myGraph.shortestPath(sourceLocation, sinkLocation, minCost);

        cout << "Shortest Path: ";
        for (int i = shortestPath.size() - 1; i >= 0; i--) {
            cout << shortestPath[i]->getName() << " ";
        }

        cout << "\nMinimum Cost: " << minCost << endl;
    } else {
        cerr << "Error: Specified locations not found in the map." << endl;
    }

    return 0;
}

