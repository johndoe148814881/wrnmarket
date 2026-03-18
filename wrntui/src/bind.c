#include "../include/bind.h"
#include <stdlib.h>

// local type defs
typedef struct {
	int key;
	void (*func)(void);} bind_t;

// local vars
bind_t* bindv = 0; int bindc = 0;

// global funcs
void bindnew(int key, void (*func)(void)) {
	bindv = realloc(bindv, ++bindc * sizeof(bind_t));
	if (!bindv)
		abort();
	bindv[bindc - 1] = (bind_t){key, func};}

void bindexecute(int key) {
	void (**funcs)(void) = 0;
	int funcc = 0;

	for (int i = 0; i < bindc; ++i)
		if (key == bindv[i].key) {
			funcs = realloc(funcs, sizeof(void (*)(void)) * ++funcc);
			if (!funcs)
				abort();
			funcs[funcc - 1] = bindv[i].func;}

	for (int i = 0; i < funcc; ++i)
		funcs[i]();

	free(funcs);}

void bindfreeall() {
	free(bindv);
	bindv = 0;
	bindc = 0;}
