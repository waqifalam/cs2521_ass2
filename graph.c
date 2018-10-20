#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"

#define FALSE -1
#define TRUE   1

Vertex newVertex(int URL) {
  Vertex new = malloc(sizeof(struct VertexRep));
  new->ID = URL;
  new->n = 1;
  new->next = NULL;

  return new;
}

Vertex findVertex(Graph g, int URL_ID) {
  int i = 0;
  for (i = 0; i < g->nV; i++) {
    if (g->URL[i]->ID == URL_ID) return g->URL[i];
  }

  return NULL;
}

int findIndex(Graph g, int URL_ID) {
  int i = 0;
  for (i = 0; i < g->nV; i++) {
    if (g->URL[i]->ID == URL_ID) return i;
  }

  return FALSE;
}

// Creates new graph from collection.txt
Graph newGraph(void) {
  Graph new = malloc(sizeof(struct GraphRep));
  char buffer[256];
  FILE *f = fopen("collection.txt", "r");
  int x = 0;
  while (fscanf(f, "%s", buffer) == 1) x++;

  new->nV = x;
  new->URL = malloc(x * sizeof(Vertex));

  rewind(f);
  for (x = 0; x < new->nV; x++) {
    fscanf(f, "%s", buffer);
    new->URL[x] = newVertex(atoi(&buffer[3]));
    addEdges(new, atoi(&buffer[3]));
  }
  fclose(f);

  return new;
}

static int ParallelEdge(Vertex curr, int URL_ID) {
  while (curr != NULL) {
    if (curr->ID == URL_ID) {
      curr->n = curr->n + 1;
      return TRUE;
    }
    curr = curr->next;
  }

  return FALSE;
}

void addEdges(Graph g, int URL) {
  char buffer[256] = "URL";
  sprintf(&buffer[3], "%d.txt", URL);   // x[3] is null character... so append at NULL
  FILE *f = fopen(buffer, "r");
  while (fscanf(f, "%s", buffer) && strcmp("Section-1", buffer) != 0);

  Vertex head = findVertex(g, URL);
  Vertex curr = head;
  Vertex tmp = NULL;

  while (fscanf(f, "%s", buffer) && strcmp("#end", buffer) != 0) {
    if (ParallelEdge(head->next, atoi(&buffer[3])) == FALSE) {
      tmp = newVertex(atoi(&buffer[3]));
      curr->next = tmp;
      curr = curr->next;
    }
  }

  fclose(f);
}

void showGraph(Graph g) {
  int i = 0;
  for (i = 0; i < g->nV; i++) {
    printf("Outbound links for %d: ", g->URL[i]->ID);
    Vertex curr = NULL;
    for (Vertex curr = g->URL[i]->next; curr != NULL; curr = curr->next) {
      printf("%d ", curr->ID);
    }
    printf("\n");
  }
}
