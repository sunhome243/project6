//
//  test_graph_example.cpp
//  CS 271 Graph Project: Example Test File
//
//  Created by Dr. Stacey Truex
//

#include <fstream>
#include <sstream>
#include "graph.cpp"

Graph<string, string> *generate_graph(string fname)
{
    string line;
    ifstream infile(fname);
    vector<string> keys = {};
    vector<string> data = {};
    vector<vector<string>> adjs = {};
    if (infile.is_open())
    {
        while (getline(infile, line))
        {
            string file_name = "40";
            size_t delim = line.find(":");
            string key = line.substr(0, delim);
            string adj = line.substr(delim + 1);

            keys.push_back(key);
            data.push_back(key + " data");
            delim = adj.find(",");
            vector<string> adj_lst = {};
            while (delim != string::npos)
            {
                adj_lst.push_back(adj.substr(0, delim));
                adj = adj.substr(delim + 1);
                delim = adj.find(",");
            }
            adj_lst.push_back(adj);
            adjs.push_back(adj_lst);
        }
        infile.close();
    }
    Graph<string, string> *G = new Graph<string, string>(keys, data, adjs);
    return G;
}

void test_get(Graph<string, string> *G)
{
    try
    {
        if (G->get("S") == nullptr || G->get("S")->data != "S data")
        {
            cout << "Incorrect result getting vertex \"s\"" << endl;
        }
        if (G->get("a") != nullptr)
        {
            cout << "Incorrect result getting non-existant vertex \"a\"" << endl;
        }
    }
    catch (exception &e)
    {
        cerr << "Error getting vertex from graph : " << e.what() << endl;
    }
}

void test_reachable(Graph<string, string> *G)
{
    try
    {
        if (!G->reachable("R", "V"))
        {
            cout << "Incorrectly identified adjacent vertex \"V\" as unreachable from \"R\"" << endl;
        }
        if (!G->reachable("X", "W"))
        {
            cout << "Incorrectly identified \"W\" as unreachable from \"X\"" << endl;
        }
        if (G->reachable("S", "A"))
        {
            cout << "Incorrectly identified non-existant vetex \"A\" as reachable from \"S\"" << endl;
        }
    }
    catch (exception &e)
    {
        cerr << "Error testing reachable : " << e.what() << endl;
    }
}

void test_bfs(Graph<string, string> *G)
{
    try
    {
        G->bfs("T");
        string vertices[8] = {"V", "R", "S", "W", "T", "X", "U", "Y"};
        int distances[8] = {3, 2, 1, 1, 0, 2, 1, 2};
        for (int i = 0; i < 8; i++)
        {
            if (G->get(vertices[i]) == nullptr || G->get(vertices[i])->distance != distances[i])
            {
                cout << "Incorrect bfs result. Vertex " << vertices[i] << " should have distance " << distances[i] << " from source vertex \"t\"" << endl;
            }
        }
    }
    catch (exception &e)
    {
        cerr << "Error testing bfs : " << e.what() << endl;
    }
}

void test_print_path(Graph<string, string> *G)
{
    try
    {
        stringstream buffer;
        streambuf *prevbuf = cout.rdbuf(buffer.rdbuf());
        G->print_path("T", "V");
        cout.rdbuf(prevbuf);
        if (buffer.str() != "T -> S -> R -> V")
        {
            cout << "Incorrect path from vertex \"T\" to vertex \"V\". Expected: T -> S -> R -> V but got : " << buffer.str() << endl;
        }
    }
    catch (exception &e)
    {
        cerr << "Error testing print path : " << e.what() << endl;
    }
}

void test_edge_class(Graph<string, string> *G)
{
    try
    {
        string e_class = G->edge_class("R", "V"); // tree edge
        if (e_class != "tree edge")
        {
            cout << "Misidentified tree edge (\"R\", \"V\") as : " << e_class << endl;
        }
        e_class = G->edge_class("X", "U"); // back edge
        if (e_class != "back edge")
        {
            cout << "Misidentified back edge (\"X\", \"U\") as : " << e_class << endl;
        }
        e_class = G->edge_class("R", "U"); // no edge
        if (e_class != "no edge")
        {
            cout << "Misidentified non-existant edge (\"R\", \"U\") as : " << e_class << endl;
        }
        e_class = G->edge_class("T", "W"); // forward edge
        if (e_class != "forward edge")
        {
            cout << "Misidentified forward edge (\"T\", \"W\") as : " << e_class << endl;
        }
        e_class = G->edge_class("T", "S"); // cross edge
        if (e_class != "cross edge")
        {
            cout << "Misidentified forward edge (\"T\", \"S\") as : " << e_class << endl;
        }
    }
    catch (exception &e)
    {
        cerr << "Error testing edge class : " << e.what() << endl;
    }
}

void test_bfs_tree(Graph<string, string> *G)
{
    try
    {
        stringstream buffer;
        streambuf *prevbuf = cout.rdbuf(buffer.rdbuf());
        G->bfs_tree("T");
        cout.rdbuf(prevbuf);
        if (buffer.str() != "T\nS U W\nR Y X\nV")
        {
            cout << "Incorrect bfs tree. Expected : \nT\nS U W\nR Y X\nV \nbut got :\n"
                 << buffer.str() << endl;
        }
    }
    catch (exception &e)
    {
        cerr << "Error testing bfs tree : " << e.what() << endl;
    }
}

int main()
{
    string file_name = "solution";
    cout << endl
         << "Running tests for " << file_name << endl
         << endl;

    Graph<string, string> *G = generate_graph("graph_description.txt");
    test_get(G);
    test_reachable(G);
    test_bfs(G);
    test_print_path(G);
    test_edge_class(G);
    test_bfs_tree(G);

    cout << "Testing completed" << endl;

    delete G;

    return 0;
}