#include "../include/graph.h"
#include "../include/tui.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// local type defs
typedef struct {
	int row;
	int col;
	int rows;
	int cols;
	int typeid;
	void* value;
	void** valuehistory;} graph_t;

// local vars
static graph_t* graphv = 0; static int graphc = 0;
static struct timespec ots, ts = {0};

// local func defs
static void newgraph(int, int, int, int, void*, int);
static void delallgraphs();
static int updategraph(graph_t*);
static void drawgraph(graph_t*);

// global funcs
void graphnew(int row, int col, int rows, int cols, void* value, int typeid) {
	pthread_mutex_lock(&tuiflushmutex);
	newgraph(row, col, rows, cols, value, typeid);
	pthread_mutex_unlock(&tuiflushmutex);}

void graphdrawall() {
	pthread_mutex_lock(&tuiflushmutex);
	for (int i = 0; i < graphc; ++i)
		if (!updategraph(&graphv[i]))
			drawgraph(&graphv[i]);
	pthread_mutex_unlock(&tuiflushmutex);}

void graphfreeall() {
	pthread_mutex_lock(&tuiflushmutex);
	delallgraphs();
	pthread_mutex_unlock(&tuiflushmutex);}

// local funcs
static void newgraph(int row, int col, int rows, int cols, void* value, int typeid) {
	char* odraw = malloc(cols + 1);
	graphv = realloc(graphv, ++graphc * sizeof(graph_t));
	
	if (!odraw || !graphv) 
		abort();
	
	memset(odraw, 0, cols + 1);
	graphv[graphc - 1] = (graph_t){row, col, rows, cols, typeid, value, malloc(sizeof(void*) * cols)};}

static void delallgraphs() {
	for (int i = 0; i < graphc; ++i) 
		free(graphv[i].valuehistory);
	free(graphv);
	graphv = 0;
	graphc = 0;}

static int updategraph(graph_t* graph) {
	clock_gettime(CLOCK_MONOTONIC, &ts);
	double elapsed = (ts.tv_sec - ots.tv_sec) + (ts.tv_nsec - ots.tv_nsec) / 1e9;

	if (elapsed < (1 / tuiframerate))
		return 1;

	ots = ts;
	
	free(graph->valuehistory[0]);

	for (int i = 1; i < graph->cols; ++i)
		graph->valuehistory[i - 1] = graph->valuehistory[i];
	
	switch (graph->typeid) {
	case GRAPHINT: {
		graph->valuehistory[graph->cols - 1] = malloc(sizeof(int));
		*(int*)graph->valuehistory[graph->cols - 1] = *(int*)graph->value;
		break;}
	case GRAPHFRAC: {
		graph->valuehistory[graph->cols - 1] = malloc(sizeof(frac_t));
		*(frac_t*)graph->valuehistory[graph->cols - 1] = *(frac_t*)graph->value;
		break;}}

	return 0;}

static void drawgraph(graph_t* graph) {
	(void)graph;}
