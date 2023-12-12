/*
------------------------------------------------------------------
*
* Author: Adham Ahmed Ali
* ID: 900223243
* Assignment: CSCE 2211 / assignment 6
* Due Date: 12/12/2023
* Description: Graphs.cpp
*
------------------------------------------------------------------
*/
#include "Graphs.hpp"

// FILE: graphs.cpp
// Graph Library Implementation File for Asn 6

#include <fstream>        // For reading file from disk
#include <iostream>
#include <string>        // For file name
#include <iomanip>
#include "Edge.h"        // Deinition of an Edge
#include <limits>
#include <stack>


ofstream out("output.txt");
using namespace std;
// Constructor
Graphs::Graphs()
{
    E = V = order = 0;
}

// Destructor
Graphs::~Graphs()
{
}


// Map vertex number (0,1,2,..) to (A,B,C,..)
char Graphs::Vname(const int s) const
{
    return char(s+65);
}

// Get Graph from text File (file name is stored in string fname)
// Graph is stored in adjacency matrix
void Graphs::getGraph(string fname)
{
   // Local data ...
    weightType wi;
    ifstream source;

    source.open(fname.c_str());
    source >> V; // Read no. of verices (V)
    // Input weights from the file into a 2-D
    // Adjacency Matrix of V rows and V columns
    for (int r = 0; r < V; r++)
    {
        for(int j=0;j<V;j++)
        {
            int z;
            source>>z;
            AdjMatrix[r][j]=z;
        }
        // get V weights for row [r] from file
        // put V weights in adjacency matrix at row [r]
    }
    // close file
    source.close();
    getEdges();
    
}


// Get Non-Zero edges from adjacency matrix
// and store them in array edges[].
void Graphs::getEdges()
{
    int r , c;
    int i = 0;
    weightType weight;

    // Only examine weights above the diagonal
    for (r = 0; r <= V-2; r++)
        for (c = r+1; c <= V-1; c++)
        {
            weight = AdjMatrix[r][c];
            if (weight > 0)
            {
                // save (r,c,weight) in edges[i]
                edges[i].u = r;
                edges[i].v = c;
                edges[i].w = weight;
                i++;
            }
        }

    E = i;        // Number of non-zero edges
    
}

// Get number of vertices (V)
int Graphs::No_of_Vertices() const
{
    return V;
}

// Get Number of Non-zero edges (E)
int Graphs::No_of_Edges() const
{
    return E;
}
//to display the number of vertices and edges in the begining of the file like the sample file
void Graphs::display_sentences()
{
    out<<"Note: The DFS traverse is in the output of the int main , as I want to make the output file like the Test file "<<endl<<endl;
    out << "Number of Vertices = " <<No_of_Vertices() << endl;
    out << "Number of Edges = " << No_of_Edges() << endl;
    out << endl;
}
// Display Adjacency Matrix
void Graphs::dispGraph() const
{
    int i,j;
    out<<"Adjacency Matrix\n";
    for(i=0; i<V; i++)
    {
        for(j=0; j<V; j++)
            out<< setw(3)<< AdjMatrix[i][j] << " ";
        out<< endl;
    }
}


// Output an edge (e): Vertex names and weight
void Graphs::printEdge(Edge e) const
{
    out << Vname(e.u) << " " << Vname(e.v) << " " << e.w << endl;
}

// Display Graph Edges
void Graphs::dispEdges() const
{
    out<<endl;
    out<<"Graph Edges\n";
    for (int i = 0; i < E; i++)
        printEdge(edges[i]);
}


// Find the vertex with the minimum distance value among the vertices not yet included in the shortest path tree
int Graphs::minimum_path(vector<pair<int, string>> distance, bool flag[])
{
    int minimum_value = INT_MAX; // Initialize the minimum distance to a large value
    int minimum_index;

    // Iterate through all vertices
    for (int v = 0; v < V; v++)
    {
        // Check if the vertex 'v' is not yet included in the shortest path tree and its distance is less than or equal to the current minimum value
        if (flag[v] == false && distance[v].first <= minimum_value)
        {
            minimum_value = distance[v].first; // Update the minimum distance value
            minimum_index = v; // Update the index of the vertex with the minimum distance
        }
    }

    return minimum_index; // Return the index of the vertex with the minimum distance
}


// Shortest paths from node s
// uses Dijkstra's Algorithm
void Graphs::shPath(int s)
{
    // Vector to store pairs of distances and paths from the source 's' to each vertex 'i'
    vector<pair<int, string>> distance(V); // distance will save the shortest path from 's' to 'i'
    
    // Boolean array to mark whether a vertex 'i' is part of the shortest path tree or if its shortest distance is determined
    bool flag[V]; // Here, flag will be true when vertex 'i' is part of the shortest path tree or when the shortest distance from the source to 'i' is determined.

    // Initialize distance and flag arrays
    for (int i = 0; i < V; i++)
    {
        distance[i].first = INT_MAX; // Initialize all distances to infinity
        flag[i] = false; // Initialize flag as false, indicating the vertex is not yet included in the shortest path tree
    }

    // Set the distance from the source 's' to itself as 0 and append the source vertex to the path
    distance[s].first = 0;
    char c = s + 65; // Convert vertex index to character representation (assuming 0-based indices and ASCII values for characters)
    distance[s].second.push_back(c);

    // Loop to find the shortest path for all vertices
    for (int count = 0; count < V - 1; count++)
    {
        // Find the vertex with the minimum distance value
        int z = minimum_path(distance, flag);

        // Mark the selected vertex as true in the flag array, indicating it is included in the shortest path tree
        flag[z] = true;

        // Update the distance values of adjacent vertices
        for (int v = 0; v < V; v++)
        {
            string s;

            // Check if the vertex 'v' is not in the shortest path tree, there is an edge from 'z' to 'v',
            // the total distance from 's' to 'v' through 'z' is smaller than the current known distance to 'v'
            if (!flag[v] && AdjMatrix[z][v] && distance[z].first != INT_MAX && distance[z].first + AdjMatrix[z][v] < distance[v].first)
            {
                // Update the distance and record the previous vertex in the path
                distance[v].first = distance[z].first + AdjMatrix[z][v];
                via[v] = z; // 'via' array keeps track of the path by storing the previous vertex for each vertex in the shortest path
            }
        }
    }
    
    // Print distance array and paths
    print_function(distance, s);
}


void Graphs::print_function(vector<pair<int, string>> distance, int s)
{
    out << endl; // Move to a new line to match the sample file
    out << "Shortest Paths from Node " << Vname(s) << endl;

    // Iterate through all vertices to print distance and paths
    for (int i = 0; i < V; i++)
    {
        out << "  " << distance[i].first << "  "; // Print the shortest distance to vertex 'i'

        // Call the printPath function to print the path from 's' to 'i'
        printPath(s, i);

        out << endl;
    }
}


// Print path (vertex names) from source (s) to destination (i)
void Graphs::printPath(int s, int i) const
{
    // Base case: If the destination 'i' is the source 's', print the source vertex
    if (i == s)
    {
        out << Vname(s);
    }
    else
    {
        printPath(s, via[i]);

        // Print the current vertex 'i' after the recursive call
        out << Vname(i);
    }
}


// Node Visit Function
// Helper function for Depth-First Search (DFS) to visit vertices and set their visit order
void Graphs::visit(int k)
{
    val[k] = order++; // Set the visit order for vertex 'k' and increment the order counter
    for (int i = 0; i < V; i++)
    {
        // Check if there is an edge from 'k' to 'i'
        if (AdjMatrix[k][i] != 0)
        {
            // If vertex 'i' is not visited (-2), recursively visit it
            if (val[i] == -2)
            {
                visit(i);
            }
        }
    }
}

// Depth First Search Traversal
void Graphs::DFS()
{
    int i;
    
    // Initialize visit order values for all vertices to -2 (indicating not visited)
    for (i = 0; i < V; i++)
    {
        val[i] = -2;
    }

    // Perform DFS traversal for unvisited nodes
    for (i = 0; i < V; i++)
    {
        if (val[i] == -2)
        {
            visit(i); // Call the visit function to initiate DFS traversal from vertex 'i'
        }
    }
    cout<<"The vistied vertex by their order "<<endl;
    for (int j = 0; j < V; j++)
    {
        char c=65+j;
        cout<<c<<" is the "<<val[j]+1<<"th"<<endl;
    }
    
}




