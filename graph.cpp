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
}

template <typename D, typename K>
Graph<D, K>::~Graph()
{
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
    Vertex<D, K> target = get(v);
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
        K *u_key = q.front();
        q.pop();
        Vertex<D, K> *u = get(u_key);
        for (auto v_key : u->adj)
        {
            Vertex<D, K> *v = get(v_key);
            if (v->color == "white")
            {
                v->color == "gray";
                v->distance = u.distnace + 1;
                v->pi = u_key;
                q.push(v_key)
            }
            u.color = "black"
        }
    }
}

template <typename D, typename K>
void Graph<D, K>::print_path(K u, K v)
{
}

template <typename D, typename K>
string Graph<D, K>::edge_class(K u, K v)
{

    return "";
}

template <typename D, typename K>
void Graph<D, K>::bfs_tree(K s)
{
}

// ========================================
// Helper Methods
// ========================================

template <typename D, typename K>
void Graph<D, K>::reset_bfs_state()
{
    for (&auto pair : vertices)
    {
        pair.second->color = "white";
        pair.second->pi = nullptr;
        pair.second->distance = -1;
    }
}
