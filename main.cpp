/*
------------------------------------------------------------------
*
* Author: Adham Ahmed Ali
* ID: 900223243
* Assignment: CSCE 2211 / assignment 6
* Due Date: 12/12/2023
* Description: main.cpp
*
------------------------------------------------------------------
*/

#include <iostream>
#include "Graphs.hpp"
#include "Edge.h"
#include <fstream>
#include <string>
using namespace std;

int main() 
{
    // Create an object of the Graphs class
    Graphs G;

    // Load the graph from the "Cities.txt" file
    G.getGraph("Cities.txt");
    
    // Print the number of vertices and edges in the graph
    G.display_sentences();

    // Display the graph
    G.dispGraph();

    // Display the edges of the graph
    G.dispEdges();

    // Find and display the shortest path from A
    G.shPath(0);
    
    G.DFS();// printing the function in the int main to presrve the shape of the output file
    // calling it as it is a requirment in the assignment
    
    
    return 0;
}
