#ifndef GRAPH_H
#define GRAPH_H

typedef struct VertexRep *Vertex;
typedef struct GraphRep *Graph;

struct VertexRep {
  int ID;        // URL ID
  int n;         // No. of parallel edges
  Vertex next;   // Outbound links to other URLs
};

struct GraphRep {
  int nV;        // no. of vertex
  Vertex *URL;   // adjacency list representation
};

Vertex newVertex(int URL);
Vertex findVertex(Graph g, int URL_ID);
int findIndex(Graph g, int URL_ID);
Graph newGraph(void);
void addEdges(Graph g, int URL);
void showGraph(Graph g);

#endif
