#include <stdio.h>
#include <conio.h>

#define n               3   // Length/Breadth of the square arena
#define u               0   // Up
#define ur              1   // Up Right
#define r               2   // Right
#define dr              3   // Down Right
#define d               4   // Down
#define dl              5   // Down Left
#define l               6   // Left
#define ul              7   // Up Left
#define threshold_val  10   // If sensor value is beyond this threshold, we consider that there is no obstacle/wall
#define north           0   // Directions with some value for updating graph and movement based on robot orientation
#define northeast       1
#define east            2
#define southeast       3
#define south           4
#define southwest       5
#define west            6
#define northwest       7

int graph[n*n+1][8];        // Array of n*n+1 with 0th entry being empty. Inner array consist of the 8 different directions in which node can be connected
char print_str[n+3][n*n];   // String to represent Graph
int visited_nodes[n*n] = {0};   // Array to keep track of the nodes that are visited
int stack[n*n] = {0};       // Stack for back tracing in case of a dead end
int stack_pointer = 0;      // Stack pointer to keep tract of last inserted or deleted element from Stack

#include "graph.h"          // Header file containing necessary functions to perform on Graph
#include "algorithm.h"      // Header file containing necessary algorithms (DFS)

int main()
{
    initialize_graph();
    connect_node(1,r,5,north);
    connect_node(1,dr,5,north);
    connect_node(1,d,5,north);
    connect_node(2,r,5,north);
    connect_node(2,dr,5,north);
    connect_node(2,d,5,north);
    connect_node(3,dl,5,north);
    connect_node(3,d,5,north);
    connect_node(4,r,5,north);
    connect_node(4,dr,5,north);
    connect_node(4,d,5,north);
    connect_node(5,r,5,north);
    connect_node(5,d,5,north);
    connect_node(5,dl,5,north);
    connect_node(6,d,5,north);
    connect_node(6,dl,5,north);
    connect_node(7,r,5,north);
    connect_node(8,r,5,north);
    //print_graph();
    //pretty_print_graph();
    generate_graph();
    print_graph();
    pretty_print_graph();
}
