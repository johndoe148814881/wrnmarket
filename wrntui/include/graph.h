#ifndef TUI_GRAPH_H
#define TUI_GRAPH_H
#define GRAPHINT 0
#define GRAPHFRAC 1

extern void graphnew(int row, int column, int rows, int columns, void* value, int typeidentifier);
extern void graphdrawall();
extern void graphfreeall();

#endif

