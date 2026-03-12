#include "../include/main.h"
#include "../include/tui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int running = 1;
int simulationloaded = 0;
frac_t liquidvolume;

int parseargs(int, char**);

int main(int argc, char** argv) {
	parseargs(argc, argv);

	liquidvolume = fracnew(0, 1);

	// create threads
	pthread_t tuithread;
	pthread_create(&tuithread, 0, tuiinit(&running), 0);
	
	// tui related
	tuicreatecmds();
	tuicreatebinds();
	tuicreatehome();

	// join threads
	pthread_join(tuithread, 0);

	return 0;}

int parseargs(int argc, char** argv) {
	// parse args
//	for (int i = 1; i < argc; ++i)
//		if (strcmp(argv[i], "--notui") == 0) {
//			disableout = 1;
//			argv[i] = NULL;}

	for (int i = 1; i < argc; ++i) // if invalid args, error and exit
		if (argv[i] != NULL) {
			char* generalmsg = "invalid arguments: ";
			char* err;
			int errlen = strlen(generalmsg);
			
			for (int i = 1; i < argc; ++i)
				if (argv[i] != NULL)
					errlen += strlen(argv[i]) + 1;

			err = malloc(errlen + 1);
			memset(err, ' ', errlen);
			err[errlen] = '\0';
			strcpy(err, generalmsg);
			
			int concati = strlen(generalmsg);
			
			for (int i = 1; i < argc; ++i)
				if (argv[i] != NULL) {
					memcpy(err + concati, argv[i], strlen(argv[i]));
					concati += strlen(argv[i]) + 1;}

			fprintf(stderr, "%s: %s\n", argv[0], err);
			free(err);
			return 1;}

	return 0;}
