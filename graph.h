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
    K key; // Edge key
    D data; // Edge data
    vector<K> adj; // Adjacency list (stored as keys)

    // BFS properties
    string color; // "white", "gray", "black"
    int distance; // Distance from source (-1 represents infinity)
    K pi;         // Predecessor key

    // DFS properties
    int discovery_time;  
    int finish_time;
    // Constructor
    Vertex(K k, D d) : key(k), data(d), color("white"), distance(-1), 
                       discovery_time(-1), finish_time(-1) {}
    Vertex() : color("white"), distance(-1), discovery_time(-1), finish_time(-1) {}
};


// Graph class template: <DataType, KeyType>
template <typename D, typename K>
class Graph
{
public:
    // Public members (as per test requirements)
    map<K, Vertex<D, K> *> vertices; // V - vertices stored in a map for O(log n) access

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
    
    void dfs(K source);
    
    void dfs_visit(K u_key, int &time);
    
    void reset_dfs_state();
    
    void bfs_tree(K s);
    
    K find_source();
private:
    // Helper methods
    void reset_bfs_state();
    
};

#endif // GRAPH_H
