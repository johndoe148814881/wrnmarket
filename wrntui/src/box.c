#include "../include/box.h"
#include "../include/tui.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// local typedefs
typedef struct {
	int row;
	int col;
	int rows;
	int cols;
	int optc;
	int ooptc;
	int drawn;
	char* clr;
	char* title;
	char** optv;} box_t;

// local variables
static box_t* boxv = 0; static int boxc = 0;

// local func defs
static void newbox(int, int, int, int, char*, char*);
static void newbind(char*, char*, void (*)(void));
static void delbind(char*, char*, void (*)(void));
static void delallboxes();
static int updatebox(box_t*);
static void drawbox(box_t*);

// global funcs
void boxnew(int row, int col, int rows, int cols, char* clr, char* title) {
	pthread_mutex_lock(&tuiflushmutex);
	newbox(row, col, rows, cols, clr, title);
	pthread_mutex_unlock(&tuiflushmutex);}

void boxbindnew(char* title, char* name, void (*func)(void)) {
	pthread_mutex_lock(&tuiflushmutex);
	newbind(title, name, func);
	pthread_mutex_unlock(&tuiflushmutex);}

void boxbindfree(char* title, char* name, void (*func)(void)) {
	pthread_mutex_lock(&tuiflushmutex);
	delbind(title, name, func);
	pthread_mutex_unlock(&tuiflushmutex);}

void boxdrawall() {
	pthread_mutex_lock(&tuiflushmutex);
	for (int i = 0; i < boxc; ++i)
		if (!updatebox(&boxv[i]))
			drawbox(&boxv[i]);
	pthread_mutex_unlock(&tuiflushmutex);}

void boxfreeall() {
	pthread_mutex_lock(&tuiflushmutex);
	delallboxes();
	pthread_mutex_unlock(&tuiflushmutex);}

// local funcs
static void newbox(int row, int col, int rows, int cols, char* clr, char* title) {
	boxv = realloc(boxv, sizeof(box_t) * ++boxc);
	
	if (!boxv)
		abort();

	boxv[boxc - 1] = (box_t){row, col, rows, cols, 0, 0, 0, clr, title, 0};}

static void newbind(char* title, char* name, void (*func)(void)) {
	for (int i = 0; i < boxc; ++i)
		if (strcmp(boxv[i].title, title) == 0) {
			bindnew(*name, func);
			
			boxv[i].optv = realloc(boxv[i].optv, ++boxv[i].optc * sizeof(char*));
			
			if (!boxv[i].optv)
				abort();

			boxv[i].optv[boxv[i].optc - 1] = malloc(strlen(name) + 1);
			
			if (!boxv[i].optv[boxv[i].optc - 1])
				abort();

			strcpy(boxv[i].optv[boxv[i].optc - 1], name);}}

static void delbind(char* title, char* name, void (*func)(void)){
	for (int i = 0; i < boxc; ++i)
		if (strcmp(boxv[i].title, title) == 0) {
			bindfree(*name, func);

			int opti = -1;
			for (int ii = 0; ii < boxv[i].optc; ++ii) {
				if (strcmp(boxv[i].optv[ii], name) == 0 && opti == -1)
					opti = ii;
				if (ii > opti && opti != -1)
					boxv[i].optv[ii - 1] = boxv[i].optv[ii];}

			if (opti == -1)
				abort();
			
			free(boxv[i].optv[opti]);
			
			if (boxv[i].optc > 1) {
				boxv[i].optv = realloc(boxv[i].optv, --boxv[i].optc * sizeof(char*));

				if (!boxv[i].optv)
					abort();

				return;}
			free(boxv[i].optv);
			boxv[i].optv = 0;
			boxv[i].optc = 0;}}

static void delallboxes() {
	for (int i = 0; i < boxc; ++i) {
		for (int ii = 0; ii < boxv[i].optc; ++ii)
			free(boxv[i].optv[ii]);
		free(boxv[i].optv);}

	free(boxv);
	
	boxv = 0;
	boxc = 0;}

static int updatebox(box_t* box) {
	int changed = box->drawn && box->optc == box->ooptc;
	
	box->ooptc = box->optc;

	return changed;}

static void drawbox(box_t* box) {
	if (!box->drawn) {
		box->drawn = 1;

		// clear area
		for (int i = 0; i < box->rows - 2; ++i)
			printf("%s%*s", MOVECURS(box->row + i + 1, box->col + 1), box->cols - 2, "");}

	// bars
	printf("%s%s", CLRATTRS, box->clr);
	for (int i = 0; i < box->cols - 2; ++i) {
		printf("%s─", MOVECURS(box->row, box->col + i + 1));
		printf("%s─", MOVECURS(box->row + box->rows - 1, box->col + i + 1));}
	
	for (int i = 0; i < box->rows - 2; ++i) {
		printf("%s│", MOVECURS(box->row + i + 1, box->col));
		printf("%s│", MOVECURS(box->row + i + 1, box->col + box->cols - 1));}
	
	// corners
	printf("%s┌", MOVECURS(box->row, box->col));
	printf("%s┐", MOVECURS(box->row, box->col + box->cols - 1));
	printf("%s└", MOVECURS(box->row + box->rows - 1, box->col));
	printf("%s┘", MOVECURS(box->row + box->rows - 1, box->col + box->cols - 1));	
	
	// title
	int elementsize = strlen(box->title) + 4;
	int offset = box->col + 1;
	if (offset + elementsize <= box->cols + box->col - 1) 
		printf("%s┤ %s%s%s ├", MOVECURS(box->row, offset), CLRATTRS, box->title, box->clr);
	offset += elementsize;
	
	// options
	if (box->optc > 0)
		for (int i = 0; i < box->optc; ++i) {
			elementsize = strlen(box->optv[i]) + 4;
			if (offset + elementsize <= box->cols + box->col - 1)
				printf("%s┤ %s%s%c%s%s%s%s%s ├", MOVECURS(box->row, offset), tuiforeerr, BOLD, *box->optv[i], CLRATTRS, BOLD, box->optv[i] + 1, CLRATTRS, box->clr);
			offset += elementsize;}

	printf("%s", CLRATTRS);}
	


