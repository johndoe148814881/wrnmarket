#ifndef TUI_BIND_H
#define TUI_BIND_H

extern void bindnew(int character, void (*function)(void));
extern void bindexecute(int character);
extern void bindfreeall();

#endif
