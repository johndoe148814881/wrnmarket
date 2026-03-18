#ifndef TUI_WIN_H
#define TUI_WIN_H

extern void winnew(char* name, void (*onshow)(void), void (*onupdate)(void));
extern int winshow(char* name);
extern void winfreeall();

#endif
