#include "../include/main.h"
#include <wrntui/tui.h>
#include <stdlib.h>
#include <string.h>

int q(int, char**);
int dep(int, char**);
int wit(int, char**);

void* (*tuiinit())(void*) {
	cmdnew("q", q);
	cmdnew("dep", dep);
	cmdnew("wit", wit);

	return tui;}

int q(int argc, char** argv) {
	if (argc != 1)
		return CMDINVALIDARGC;

	running = 0;
	return CMDSUCCESS;}

int dep(int argc, char** argv) {
	if (argc != 2) {
		return CMDINVALIDARGC;}
	
	char* check;
	frac_t amount = fracnew(strtol(argv[1], &check, 10), 1);
						
	if (*check == '\0' && amount.num > 0) {
		fracadd(&liquidvolume, &amount);
		return CMDSUCCESS;}
	else 
		return CMDINVALIDARGV;}

int wit(int argc, char** argv) {
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

