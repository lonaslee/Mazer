/**
 * @file node.c
 * @brief Functions and structs that create a graph which a maze can be generated from
 *        are declared here. This differs from grid in that it uses nodes and edges.
 */

#pragma once

typedef struct Node {
    struct Edge *upper;
    struct Edge *lower;
    struct Edge *left_;
    struct Edge *right;
    int data;
} Node;

typedef struct Edge {
    struct Node *node1;
    struct Node *node2;
    int exists;
    int data;
} Edge;

/**
 * @brief A graph of nodes and edges.
 */
typedef struct {
    int width;
    int height;
    int type;
    Node **nodes;
} Graph;

/**
 * @brief Generate a Graph of nodes with connected edges that can have a maze generated in.
 *
 * @param width width of the graph
 * @param height height of the graph
 * @return the node graph that was created.
 */
Graph *generate_graph(int width, int height);

/**
 * @brief Clear a graph, setting all value fields to 0.
 *
 * @param graph graph to clear
 */
void clear_graph(Graph *graph);

/**
 * @brief Free all nodes of a graph and all associated edges. It is safe to pass NULL to this function.
 *
 * @param graph graph to free
 * @return int - Number of edges that was freed.
 */
int free_graph(Graph *graph);
