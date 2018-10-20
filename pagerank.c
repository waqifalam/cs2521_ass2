#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "graph.h"

double calculate_PR(Graph g, int x, double prevPR[]);
int inlink(Graph g, int x);
double outlink (Graph g, int x);
double w_in(Graph g, int u, int v);
double w_out(Graph g, int u, int v);
double calculate_diff(int n, double PR[], double prevPR[]);
void swap(double x[][2], int m, int n);
int partition(double x[][2], int l, int h);
void quickSort(double x[][2], int l, int h);

int main(int argc, char const *argv[]) {
  if (argc < 4) {
    fprintf(stderr, "Usage: pagerank.exe d diffPR maxIterations\n");
    return 1;
  }

  double d = atof(argv[1]);
  double diffPR = atof(argv[2]);
  double maxIterations = atof(argv[3]);
  Graph g = newGraph();
  int n = g->nV;
  double PR[n];
  double prevPR[n];

  int i = 0;
  for (i = 0; i < n; i++) PR[i] = 1.0 / n;

  i = 0;
  double diff = diffPR;

  while (i < maxIterations && diff >= diffPR) {
    int j = 0;
    for (j = 0; j < n; j++) prevPR[j] = PR[j];
    for (j = 0; j < n; j++) {
      PR[j] = (1.0 - d) / n + d * calculate_PR(g, j, prevPR);
      // printf("prev[%d] = %lf , curr[%d] = %lf\n", j, prevPR[j], j, PR[j]);
    }
    diff = calculate_diff(n, PR, prevPR);
    i++;
  }

  double result[n][2];
  for (i = 0; i < n; i++) {
    result[i][0] = PR[i];
    result[i][1] = g->URL[i]->ID;
  }
  quickSort(result, 0, n - 1);

  FILE *f = fopen("pagerankList.txt", "w");
    for (i = n - 1; i > -1; i--) {
      fprintf(f, "url%.0lf, ", result[i][1]);
      double outDegrees = outlink(g, findIndex(g, result[i][1]));
      if (outDegrees == 0.5) outDegrees = 0;
      fprintf(f, "%.0lf, %.7lf\n", outDegrees, result[i][0]);
    }
  fclose(f);
  return 0;
}

double calculate_PR(Graph g, int x, double prevPR[]){
  double result = 0;
  int i = 0;

  for (i = 0; i < g->nV; i++) {
    if (i == x) continue; // URL pointing to itself
    for (Vertex curr = g->URL[i]->next; curr != NULL; curr = curr->next) {
      if (curr->ID == g->URL[x]->ID) {
        result = result + prevPR[i] * w_in(g, i, x) * w_out(g, i, x);
        // printf("Win[%d][%d] %lf , ",i ,x, w_in(g, i, x));
        // printf("Wout[%d][%d] %lf\n",i ,x, w_out(g, i, x));
        break;
      }
    }
  }

  return result;
}

int inlink(Graph g, int x) {
  int i = 0;
  int result = 0;
  Vertex curr = NULL;
  for (i = 0; i < g->nV; i++) {
    if (i == x) continue; // URL pointing to itself
    for (curr = g->URL[i]->next; curr != NULL; curr = curr->next) {
      if (curr->ID == g->URL[x]->ID) {
        result++;
        break;
      }
    }
  }

  return result;
}

double outlink(Graph g, int x) {
  double result = 0;
  Vertex curr = NULL;
  for (curr = g->URL[x]->next; curr != NULL; curr = curr->next) {
    if (findIndex(g, curr->ID) == x) continue; // URL pointing to itself
    result++;
  }

  if (result == 0) return 0.5;
  return result;
}

double w_in(Graph g, int v, int u) {
  double sum = 0;
  Vertex curr = NULL;
  for (curr = g->URL[v]->next; curr != NULL; curr = curr->next) {
    if (curr->ID == g->URL[v]->ID) continue; // URL pointing to itself
    sum = sum + inlink(g, findIndex(g, curr->ID));
  }
  return inlink(g, u) / sum;
}

double w_out(Graph g, int v, int u) {
  double sum = 0;
  Vertex curr = NULL;
  for (curr = g->URL[v]->next; curr != NULL; curr = curr->next) {
    if (curr->ID == g->URL[v]->ID) continue; // URL pointing to itself
    sum = sum + outlink(g, findIndex(g, curr->ID));
  }
  return outlink(g, u) / sum;
}

double calculate_diff(int n, double PR[], double prevPR[]) {
  int i = 0;
  double diff = 0;
  for (i = 0; i < n; i++) diff = diff + fabs(PR[i] - prevPR[i]);
  return diff;
}

void swap(double x[][2], int m, int n) {
  double tmp[2];
  tmp[0] = x[n][0];
  tmp[1] = x[n][1];
  x[n][0] = x[m][0];
  x[n][1] = x[m][1];
  x[m][0] = tmp[0];
  x[m][1] = tmp[1];
}

int partition(double x[][2], int l, int h) {
  double threshold = x[l][0];
  int i = l + 1;
  int j = h;

  while (1) {
    while (x[i][0] < threshold && i <= j) i++;
    while (x[j][0] > threshold && i < j) j--;
    if (i >= j) break;
    swap(x, i, j);
  }

  swap(x, l, i - 1);
  return i - 1;
}

void quickSort(double x[][2], int l, int h) {
  if (l >= h) return;
  int pivot = partition(x, l, h);
  quickSort(x, l, pivot - 1);
  quickSort(x, pivot + 1, h);
}
