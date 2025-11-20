#include "graph.h"

// ========================================
// Constructors and Destructor
// ========================================

template <typename D, typename K>
Graph<D, K>::Graph() { 
}

template <typename D, typename K>
Graph<D, K>::Graph(vector<K> keys, vector<D> data, vector<vector<K>> edges)
{
    for (size_t i = 0; i < keys.size(); i++) {
    vertices[keys[i]] = new Vertex<D, K>(keys[i], data[i]);
    vertices[keys[i]]->adj = edges[i];
}
}

template <typename D, typename K>
Graph<D, K>::~Graph()
{
    for (auto& pair : vertices) {
        delete pair.second;
    }
}



/*
G.get(k) should return a pointer to the vertex corresponding to the key k in the graph G
*/

// Precondition: key k exists in graph G
// Postcondition: returns pointer to vertex with key k

template <typename D, typename K>
Vertex<D, K> *Graph<D, K>::get(K key)
{
    // 1) find key on vertices map 
    auto it = vertices.find(key);

    // 2) return pointer to vertex if found, else nullptr
    if (it != vertices.end()) // If key was found
    {
        return it->second; // Return its pointer (it->first is the key, it->second is the value)
    }
    return nullptr;
}

/*
reachable(u, v): G.reachable(u, v) should indicate if the vertex corresponding to the key v is reachable from the vertex corresponding to the key u in the graph G.
Should only print the string "X is reachable from T"
*/
template <typename D, typename K>
bool Graph<D, K>::reachable(K u, K v)
{
    bfs(u);
    Vertex<D, K>* target = get(v);
    if (target != nullptr && target->distance != -1)
    {
        return true;
    }
    return false;
}

/*
G.bfs(s) should execute the breadth-first search algorithm for the graph G from the source vertex
corresponding to the key value s.
*/
template <typename D, typename K>
void Graph<D, K>::bfs(K s)
{
    reset_bfs_state();
    Vertex<D, K> *source = get(s);
    if (source == nullptr)
    {
        return;
    }

    source->color = "gray";
    source->distance = 0;

    queue<K> q;
    q.push(s);

    while (!q.empty())
    {
        K u_key = q.front();
        q.pop();
        Vertex<D, K> *u = get(u_key);
        if (u == nullptr) continue; 
        for (auto v_key : u->adj)
        {
            Vertex<D, K> *v = get(v_key);
            if (v == nullptr) continue;
            if (v->color == "white")
            {
                v->color = "gray";
                v->distance = u->distance + 1;
                v->pi = u_key;
                q.push(v_key);
            }
            
        }
        u->color = "black";
    }
}

template <typename D, typename K>
void Graph<D, K>::print_path(K u, K v)
{
    bfs(u);
    
    Vertex<D, K>* target = get(v);
    if (target == nullptr || target->distance == -1) {
        return;
    }
    
    vector<K> path;
    K current = v;
    
    while (current != u) {
        path.push_back(current);
        Vertex<D, K>* curr_vertex = get(current);
        if (curr_vertex == nullptr) break;
        current = curr_vertex->pi;
    }
    path.push_back(u);
    
    for (int i = path.size() - 1; i >= 0; i--) {
        cout << path[i];
        if (i > 0) cout << " -> ";
    }
}


template <typename D, typename K>
string Graph<D, K>::edge_class(K u, K v)
{
    Vertex<D, K>* u_vertex = get(u);
    if (u_vertex == nullptr) return "no edge";
    
    // Check if edge exists
    bool edge_exists = false;
    for (K adj_key : u_vertex->adj) {
        if (adj_key == v) {
            edge_exists = true;
            break;
        }
    }
    if (!edge_exists) return "no edge";
    
    // Find source from last BFS (vertex with distance 0)
    K source = find_source();
    if (source != K()) {
        dfs(source);
    } else {
        return "no edge";
    }


    Vertex<D, K>* v_vertex = get(v);
    if (v_vertex == nullptr) {
        return "no edge";
    }
    
    // Tree edge: v.π = u
    if (v_vertex->pi == u) {
        return "tree edge";
    }
    
    // Back edge: edge to ancestor (v's interval contains u's interval)
    if (v_vertex->discovery_time < u_vertex->discovery_time && 
        u_vertex->finish_time < v_vertex->finish_time) {
        return "back edge";
    }
    
    // Forward edge: edge to descendant (u's interval contains v's interval)
    if (u_vertex->discovery_time < v_vertex->discovery_time && 
        v_vertex->finish_time < u_vertex->finish_time) {
        return "forward edge";
    }
    
    // Cross edge: everything else
    return "cross edge";
}


template <typename D, typename K>
void Graph<D, K>::bfs_tree(K s)
{
    reset_bfs_state();
    Vertex<D, K> *source = get(s);
    if (source == nullptr) return;

    source->color = "gray";
    source->distance = 0;

    queue<K> q;
    q.push(s);
    
    vector<K> discovery_order;  // Track order during BFS
    map<int, vector<K>> levels;

    while (!q.empty()) {
        K u_key = q.front();
        q.pop();
        
        Vertex<D, K> *u = get(u_key);
        if (u == nullptr) continue;
        
        discovery_order.push_back(u_key);
        levels[u->distance].push_back(u_key);
        
        for (auto v_key : u->adj) {
            Vertex<D, K> *v = get(v_key);
            if (v == nullptr) continue;
            if (v->color == "white") {
                v->color = "gray";
                v->distance = u->distance + 1;
                v->pi = u_key;
                q.push(v_key);
            }
        }
        u->color = "black";
    }
    
    // Print levels
    int total_levels = levels.size();
    int current_level_index = 0;
    
    for (auto& level_pair : levels) {
        vector<K>& level_vertices = level_pair.second;
        
        for (int i = 0; i < level_vertices.size(); i++) {
            cout << level_vertices[i];
            if (i < level_vertices.size() - 1) cout << " ";
        }
        
        current_level_index++;
        if (current_level_index < total_levels) {
            cout << endl;
        }
    }
}
// ========================================
// Helper Methods
// ========================================

template <typename D, typename K>
void Graph<D, K>::reset_bfs_state()
{
    for (auto& pair : vertices)
    {
        pair.second->color = "white";
        pair.second->pi = K();
        pair.second->distance = -1;
    }
}

template <typename D, typename K>
void Graph<D, K>::dfs(K source)
{
    reset_dfs_state();
    int time = 0;
    // Visit ALL vertices, creating a forest if needed
    for (auto& pair : vertices) {
        K vertex_key = pair.first;
        Vertex<D, K>* vertex = pair.second;
        
        if (vertex->color == "white") {
            dfs_visit(vertex_key, time);  // Start new tree
        }
    }
}   


template <typename D, typename K>
void Graph<D, K>::reset_dfs_state()
{
    for (auto& pair : vertices)
    {
        pair.second->color = "white";
        pair.second->pi = K();
        pair.second->discovery_time = -1;
        pair.second->finish_time = -1;
    }
}

template <typename D, typename K>
void Graph<D, K>::dfs_visit(K u_key, int& time)
{
    Vertex<D, K>* u = get(u_key);
    if (u == nullptr) return;
    
    time++;
    u->discovery_time = time;
    u->color = "gray";
    
    for (K v_key : u->adj) {
        Vertex<D, K>* v = get(v_key);
        if (v == nullptr) continue;
        
        if (v->color == "white") {
            v->pi = u_key;
            dfs_visit(v_key, time);
        }
    }
    
    u->color = "black";
    time++;
    u->finish_time = time;
}

template <typename D, typename K>
K Graph<D, K>::find_source()
{
    for (auto& pair : vertices) {
        if (pair.second->distance == 0) {
            return pair.first;
        }
    }
    return K();  // Default constructed key
}