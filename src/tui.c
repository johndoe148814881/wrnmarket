#include "../include/main.h"
#include <wrntui/tui.h>
#include <stdlib.h>
#include <string.h>

// local func defs
static void showwin(char*);
static int dep(int, char**);
static int wit(int, char**);

// global funcs
void tuicreatecmds() {
	cmdsetprefix('/');

	cmdnew("dep", dep);
	cmdnew("wit", wit);}

void tuicreatebinds() {
	}

void tuicreatehome() {
	showwin("main");}

// local funcs
static void showwin(char* winname) {
	if (strcmp(winname, "main") == 0) {
		int walletw = 50;
		int walleth = 10;
		int graphsw = 50;

		boxdraw(1, 1, walleth, walletw, FORE1, "wallet", 0, 1, (char*[]){"deposit"});
		boxdraw(1, 1 + walletw, walleth, width - walletw - graphsw, FORE2, "simulation", 0, 1, (char*[]){"new"});
		boxdraw(1 + walleth, 1, height - walleth - 2, width - graphsw, FORE3, "markets", 0, 0, 0);
		//boxdraw(1, width - graphsw, height - 2, graphsw, FORE4, "graphs", 0, 0, 0);}
	}else if (strcmp(winname, "newsim") == 0) {
		}}

static int dep(int argc, char** argv) {
	if (argc != 2) {
		return CMDINVALIDARGC;}
	
	char* check;
	frac_t amount = fracnew(strtol(argv[1], &check, 10), 1);
						
	if (*check == '\0' && amount.num > 0) {
		fracadd(&liquidvolume, &amount);
		return CMDSUCCESS;}
	else 
		return CMDINVALIDARGV;}

static int wit(int argc, char** argv) {
	if (argc != 2) 
		return CMDINVALIDARGC;
	
	if (strcmp(argv[1], "all") == 0) {
		frac_t set = fracnew(0, 1);
		fracset(&liquidvolume, &set);
		return 0;}
			
	char* check;
	frac_t amount = fracnew(strtol(argv[1], &check, 10), 1);
				
	if (*check == '\0' && amount.num > 0) {
		fracsub(&liquidvolume, &amount);
		return CMDSUCCESS;}
	else
		return CMDINVALIDARGV;}

