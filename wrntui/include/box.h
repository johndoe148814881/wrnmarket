#ifndef TUI_BOX_H
#define TUI_BOX_H

extern void boxnew(int row, int column, int rows, int columns, char* colour, char* title);
extern void boxbindnew(char* boxtitle, char* name, void (*function)(void));
extern void boxbindfree(char* boxtitle, char* name, void (*function)(void));
extern void boxdrawall();
extern void boxfreeall();

#endif

