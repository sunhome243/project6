#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <queue>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

// Vertex structure
template <typename D, typename K>
struct Vertex
{
    K key;
    D data;
    vector<K> adj; // Adjacency list (stored as keys)

    // BFS properties
    string color; // "white", "gray", "black"
    int distance; // Distance from source (-1 represents infinity)
    K pi;         // Predecessor key

    // Constructor
    Vertex(K k, D d) : key(k), data(d), color("white"), distance(-1) {}
    Vertex() : color("white"), distance(-1) {}
};

// Graph class template: <DataType, KeyType>
template <typename D, typename K>
class Graph
{
public:
    // Public members (as per test requirements)
    map<K, Vertex<D, K> * vertices>; // V - vertices stored in a map for O(log n) access

    // Constructors
    Graph();
    Graph(vector<K> keys, vector<D> data, vector<vector<K>> edges);

    // Destructor
    ~Graph();

    // Required methods
    Vertex<D, K> *get(K key);
    bool reachable(K u, K v);
    void bfs(K s);
    void print_path(K u, K v);
    string edge_class(K u, K v);
    void bfs_tree(K s);

private:
    // Helper methods
    void reset_bfs_state();
};

#endif // GRAPH_H
