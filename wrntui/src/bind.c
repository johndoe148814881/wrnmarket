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
	for (int i = 0; i < bindc; ++i)
		if (key == bindv[i].key)
			bindv[i].func();}

void bindfree(int key, void (*func)(void)) {
	int bindi = -1;
	for (int i = 0; i < bindc; ++i) {
		if (bindv[i].key == key && bindv[i].func == func && bindi == -1)
			bindi = i;
		if (i > bindi && bindi != -1)
			bindv[i - 1] = bindv[i];}

	if (bindi == -1)
		abort();

	if (bindc > 1) {
		bindv = realloc(bindv, --bindc * sizeof(bind_t));
		
		if (!bindv)
			abort();

		return;}
	free(bindv);
	bindv = 0;
	bindc = 0;}

void bindfreeall() {
	free(bindv);
	bindv = 0;
	bindc = 0;}

