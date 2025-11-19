#include "graph.h"

// ========================================
// Constructors and Destructor
// ========================================

template <typename D, typename K>
Graph<D, K>::Graph()
{
}

template <typename D, typename K>
Graph<D, K>::Graph(vector<K> keys, vector<D> data, vector<vector<K>> edges)
{
    for (int i = 0; i < keys.size(); i++) {
        vertices[keys[i]] = new Vertex<D, K>(keys[i], data[i]);
    }
    
    for (int i = 0; i < keys.size(); i++) {
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
template <typename D, typename K>
Vertex<D, K> *Graph<D, K>::get(K key)
{
    auto it = vertices.find(key);
    if (it != vertices.end()) // If key was found
    {
        return it->second; // Return its pointer
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
    
    // Check if edge (u, v) exists
    bool edge_exists = false;
    for (K adj_key : u_vertex->adj) {
        if (adj_key == v) {
            edge_exists = true;
            break;
        }
    }
    if (!edge_exists) return "no edge";
    
    Vertex<D, K>* v_vertex = get(v);
    if (v_vertex == nullptr || v_vertex->distance == -1) {
        return "no edge";
    }
    
    // Tree edge: v's parent is u in BFS tree
    if (v_vertex->pi == u) {
        return "tree edge";
    }
    // Back edge: edge to ancestor (v was discovered before u)
    else if (v_vertex->distance < u_vertex->distance) {
        return "back edge";
    }
    // Forward edge: edge to descendant (v discovered after u, but not direct child)
    else if (v_vertex->distance > u_vertex->distance) {
        return "forward edge";
    }
    // Cross edge: same level or other relationships
    else {
        return "cross edge";
    }
}


template <typename D, typename K>
void Graph<D, K>::bfs_tree(K s)
{
    bfs(s);
    
    // Build level structure preserving BFS discovery order
    vector<pair<int, K>> ordered_vertices;
    
    // Traverse in the order BFS discovered them
    queue<K> q;
    map<K, bool> added;
    
    q.push(s);
    added[s] = true;
    
    while (!q.empty()) {
        K curr_key = q.front();
        q.pop();
        
        Vertex<D, K>* curr = get(curr_key);
        if (curr != nullptr && curr->distance != -1) {
            ordered_vertices.push_back({curr->distance, curr_key});
            
            // Add neighbors in adjacency list order
            for (K neighbor : curr->adj) {
                Vertex<D, K>* nbr = get(neighbor);
                if (nbr != nullptr && nbr->distance != -1 && !added[neighbor]) {
                    added[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
    }
    
    // Print by level in discovery order
    int current_level = -1;
    for (int i = 0; i < ordered_vertices.size(); i++) {
        int level = ordered_vertices[i].first;
        K key = ordered_vertices[i].second;
        
        if (level != current_level) {
            if (current_level != -1) cout << endl;
            current_level = level;
            cout << key;
        } else {
            cout << " " << key;
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
