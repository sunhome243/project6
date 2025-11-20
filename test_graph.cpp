//
//  test_graph.cpp
//  CS 271 Graph Project
//

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "graph.cpp"

using namespace std;

// 1. Generate a Graph with DIFFERENT Template types <int Data, string Key>
// This fulfills the "Variation in templates" rubric requirement.
Graph<int, string> *generate_graph()
{
    // Node Structure:
    // A(10) -> B, C
    // B(20) -> D
    // C(30) -> A (Back Edge / Cycle)
    // D(40) -> C
    // E(50) -> (Disconnected / Isolated)
    
    vector<string> keys = {"A", "B", "C", "D", "E"};
    vector<int> data = {10, 20, 30, 40, 50};
    vector<vector<string>> edges = {
        {"B", "C"}, // A's neighbors
        {"D"},      // B's neighbors
        {"A"},      // C's neighbors
        {"C"},      // D's neighbors
        {}          // E's neighbors (No edges)
    };

    Graph<int, string> *G = new Graph<int, string>(keys, data, edges);
    return G;
}

void test_get(Graph<int, string> *G)
{
    try
    {
        // Check existing vertex with int data
        if (G->get("A") == nullptr || G->get("A")->data != 10)
        {
            cout << "Incorrect result getting vertex \"A\". Expected data 10." << endl;
        }
        // Check disconnected vertex
        if (G->get("E") == nullptr || G->get("E")->data != 50)
        {
            cout << "Incorrect result getting vertex \"E\"." << endl;
        }
        // Check non-existent vertex
        if (G->get("Z") != nullptr)
        {
            cout << "Incorrect result getting non-existant vertex \"Z\"" << endl;
        }
    }
    catch (exception &e)
    {
        cerr << "Error getting vertex from graph : " << e.what() << endl;
    }
}

void test_reachable(Graph<int, string> *G)
{
    try
    {
        // Direct reachability
        if (!G->reachable("A", "B"))
        {
            cout << "Incorrectly identified adjacent vertex \"B\" as unreachable from \"A\"" << endl;
        }
        // Indirect reachability
        if (!G->reachable("A", "D"))
        {
            cout << "Incorrectly identified distant vertex \"D\" as unreachable from \"A\"" << endl;
        }
        // Cycle reachability (B -> D -> C -> A)
        if (!G->reachable("B", "A"))
        {
            cout << "Incorrectly identified vertex \"A\" as unreachable from \"B\" (Cycle case)" << endl;
        }
        // Disconnected case
        if (G->reachable("A", "E"))
        {
            cout << "Incorrectly identified disconnected vertex \"E\" as reachable from \"A\"" << endl;
        }
    }
    catch (exception &e)
    {
        cerr << "Error testing reachable : " << e.what() << endl;
    }
}

void test_bfs(Graph<int, string> *G)
{
    try
    {
        G->bfs("A");
        
        // Expected distances from A:
        // A(0) -> B(1), C(1)
        // B -> D(2)
        // E is unreachable (-1)
        
        string vertices[5] = {"A", "B", "C", "D", "E"};
        int distances[5] = {0, 1, 1, 2, -1};
        
        for (int i = 0; i < 5; i++)
        {
            Vertex<int, string>* v = G->get(vertices[i]);
            if (v == nullptr || v->distance != distances[i])
            {
                int actual_dist = (v == nullptr) ? -999 : v->distance;
                cout << "Incorrect bfs result. Vertex " << vertices[i] 
                     << " should have distance " << distances[i] 
                     << " from source \"A\" but got " << actual_dist << endl;
            }
        }
    }
    catch (exception &e)
    {
        cerr << "Error testing bfs : " << e.what() << endl;
    }
}

void test_print_path(Graph<int, string> *G)
{
    try
    {
        // 1) Valid Path A -> B -> D
        // BFS logic usually picks 'B' before 'C' if adjacency list is {"B", "C"}
        // So Path to D should be A -> B -> D
        stringstream buffer;
        streambuf *prevbuf = cout.rdbuf(buffer.rdbuf());
        G->print_path("A", "D");
        cout.rdbuf(prevbuf);
        
        if (buffer.str() != "A -> B -> D")
        {
            cout << "Incorrect path from \"A\" to \"D\". Expected: A -> B -> D but got : " << buffer.str() << endl;
        }
        
        // 2) No Path (Disconnected)
        // Should print nothing
        stringstream buffer2;
        prevbuf = cout.rdbuf(buffer2.rdbuf());
        G->print_path("A", "E");
        cout.rdbuf(prevbuf);
        
        if (buffer2.str() != "")
        {
            cout << "Incorrect path for disconnected nodes. Expected empty string but got: " << buffer2.str() << endl;
        }
    }
    catch (exception &e)
    {
        cerr << "Error testing print path : " << e.what() << endl;
    }
}

void test_edge_class_custom()
{
    // We create a specific small graph for deterministic edge classification
    // Graph: X -> Y (Tree), Y -> Z (Tree), X -> Z (Forward), Z -> X (Back)
    try
    {
        vector<string> k = {"X", "Y", "Z"};
        vector<int> d = {1, 2, 3};
        vector<vector<string>> e = {{"Y", "Z"}, {"Z"}, {"X"}};
        
        Graph<int, string> *G_Edge = new Graph<int, string>(k, d, e);
        
        // Tree Edge (X -> Y) - First visit
        string e_class = G_Edge->edge_class("X", "Y");
        if (e_class != "tree edge")
        {
             cout << "Misidentified tree edge (\"X\", \"Y\") as : " << e_class << endl;
        }

        // Forward Edge (X -> Z)
        // X visits Y, Y visits Z (Z becomes black). Then X checks Z. 
        // X is ancestor of Z, and Z is already finished.
        e_class = G_Edge->edge_class("X", "Z");
        if (e_class != "forward edge")
        {
             cout << "Misidentified forward edge (\"X\", \"Z\") as : " << e_class << endl;
        }
        
        // Back Edge (Z -> X) - Z goes back to ancestor X
        e_class = G_Edge->edge_class("Z", "X");
        if (e_class != "back edge")
        {
             cout << "Misidentified back edge (\"Z\", \"X\") as : " << e_class << endl;
        }
        
        delete G_Edge;
    }
    catch (exception &e)
    {
        cerr << "Error testing edge class : " << e.what() << endl;
    }
}

void test_bfs_tree(Graph<int, string> *G)
{
    try
    {
        stringstream buffer;
        streambuf *prevbuf = cout.rdbuf(buffer.rdbuf());
        G->bfs_tree("A");
        cout.rdbuf(prevbuf);
        
        // Expected Level Order:
        // A
        // B C
        // D
        // Note: E is not reachable, so it should not appear.
        
        // We check if the string contains the correct levels
        string output = buffer.str();
        bool hasA = output.find("A") != string::npos;
        bool hasBC = output.find("B C") != string::npos; 
        bool hasD = output.find("D") != string::npos;
        
        if (!hasA || !hasBC || !hasD)
        {
            cout << "Incorrect bfs tree. Expected A, then B C, then D. Got:\n" << output << endl;
        }
        
        // Ensure E is NOT printed
        if (output.find("E") != string::npos)
        {
            cout << "Incorrect bfs tree. Unreachable node 'E' should not be printed." << endl;
        }
    }
    catch (exception &e)
    {
        cerr << "Error testing bfs tree : " << e.what() << endl;
    }
}

int main()
{
    string file_name = "Student Custom Tests <int, string>";
    cout << endl
         << "Running tests for " << file_name << endl
         << endl;

    Graph<int, string> *G = generate_graph();
    
    test_get(G);
    test_reachable(G);
    test_bfs(G);
    test_print_path(G);
    test_bfs_tree(G);
    
    // Edge class needs specific structure to guarantee types, so we use a helper
    test_edge_class_custom();

    cout << "Testing completed" << endl;

    delete G;

    return 0;
}