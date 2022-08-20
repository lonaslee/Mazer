#include "graph.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Graph *generate_graph(int width, int height) {
    Node **graph = calloc(width, sizeof(Node *));
    for (int i = 0; i < width; i++) {
        Node *column = calloc(height, sizeof(Node));
        graph[i] = column;
    }

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            puts("");
            if (graph[x][y].upper == NULL) {
                if (y != height - 1) {
                    if (graph[x][y + 1].lower == NULL) {
                        Edge *edge = calloc(1, sizeof(Edge));
                        graph[x][y].upper = edge;
                        graph[x][y + 1].lower = edge;
                        edge->node2 = &graph[x][y];
                        edge->node1 = &graph[x][y + 1];
                    } else {
                        graph[x][y].upper = graph[x][y + 1].lower;
                    }
                } else {
                    Edge *edge = calloc(1, sizeof(Edge));
                    graph[x][y].upper = edge;
                    edge->node2 = &graph[x][y];
                }
                printf("upper\n");
            }
            if (graph[x][y].lower == NULL) {
                if (y != 0) {
                    if (graph[x][y - 1].upper == NULL) {
                        Edge *edge = calloc(1, sizeof(Edge));
                        graph[x][y].lower = edge;
                        graph[x][y - 1].upper = edge;
                        edge->node1 = &graph[x][y - 1];
                        edge->node2 = &graph[x][y];
                    } else {
                        graph[x][y].lower = graph[x][y - 1].upper;
                    }
                } else {
                    Edge *edge = calloc(1, sizeof(edge));
                    graph[x][y].lower = edge;
                    edge->node1 = &graph[x][y];
                }
                printf("lower\n");
            }
            if (graph[x][y].left_ == NULL) {
                if (x != 0) {
                    if (graph[x - 1][y].right == NULL) {
                        Edge *edge = calloc(1, sizeof(Edge));
                        graph[x][y].left_ = edge;
                        graph[x - 1][y].right = edge;
                        edge->node1 = &graph[x - 1][y];
                        edge->node2 = &graph[x][y];
                    } else {
                        graph[x][y].left_ = graph[x - 1][y].right;
                    }
                } else {
                    Edge *edge = calloc(1, sizeof(Edge));
                    graph[x][y].left_ = edge;
                    edge->node2 = &graph[x][y];
                }
                printf("left\n");
            }
            if (graph[x][y].right == NULL) {
                if (x != width - 1) {
                    if (graph[x + 1][y].left_ == NULL) {
                        Edge *edge = calloc(1, sizeof(Edge));
                        graph[x][y].right = edge;
                        graph[x + 1][y].left_ = edge;
                        edge->node1 = &graph[x][y];
                        edge->node2 = &graph[x + 1][y];
                    } else {
                        graph[x][y].right = graph[x + 1][y].left_;
                    }
                } else {
                    Edge *edge = calloc(1, sizeof(edge));
                    graph[x][y].right = edge;
                    edge->node1 = &graph[x][y];
                }
                printf("right\n");
            }
        }
    }

    Graph *graphobj = calloc(1, sizeof(Graph));
    graphobj->width = width;
    graphobj->height = height;
    graphobj->nodes = graph;
    return graphobj;
}

void clear_graph(Graph *graph) {
    if (graph == NULL) return;
    for (int x = 0; x < graph->width; x++) {
        for (int y = 0; y < graph->height; y++) {
            graph->nodes[x][y].data = 0;
            if (graph->nodes[x][y].upper != NULL) graph->nodes[x][y].upper->exists = graph->nodes[x][y].upper->data = 0;
            if (graph->nodes[x][y].lower != NULL) graph->nodes[x][y].lower->exists = graph->nodes[x][y].lower->data = 0;
            if (graph->nodes[x][y].left_ != NULL) graph->nodes[x][y].left_->exists = graph->nodes[x][y].left_->data = 0;
            if (graph->nodes[x][y].right != NULL) graph->nodes[x][y].right->exists = graph->nodes[x][y].right->data = 0;
        }
    }
}

int free_graph(Graph *graph) {
    if (graph == NULL) return 0;
    int num = 0;
    for (int x = 0; x < graph->width; x++) {
        for (int y = 0; y < graph->height; y++) {
            if (graph->nodes[x][y].upper != NULL) {
                num++;
                free(graph->nodes[x][y].upper);
                graph->nodes[x][y].upper = NULL;
                if (y != graph->height - 1) graph->nodes[x][y + 1].lower = NULL;
            }
            if (graph->nodes[x][y].lower != NULL) {
                num++;
                free(graph->nodes[x][y].lower);
                graph->nodes[x][y].lower = NULL;
                if (y != 0) graph->nodes[x][y - 1].upper = NULL;
            }
            if (graph->nodes[x][y].left_ != NULL) {
                num++;
                free(graph->nodes[x][y].left_);
                graph->nodes[x][y].left_ = NULL;
                if (x != 0) graph->nodes[x - 1][y].right = NULL;
            }
            if (graph->nodes[x][y].right != NULL) {
                num++;
                free(graph->nodes[x][y].right);
                graph->nodes[x][y].right = NULL;
                if (x != graph->width - 1) graph->nodes[x + 1][y].left_ = NULL;
            }
        }
    }
    free(graph);
    return num;
}