#include "../include/main.h"
#include "../include/tui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// global vars
int running = 1;
int simulationloaded = 0;

frac_t walletvolume;

frac_t liquidvolume;
frac_t activevolume;
frac_t liquidity;
int openmarketsthreshold = DEFAULTOPENMARKETSTHRESHOLD;
int probabilityalgorithm = DEFAULTPROBABILITYALGORITHM;
frac_t probabilitythreshold;
frac_t winvolume;
frac_t lossvolume;
frac_t profitvolume;
int wins = 0;
int losses = 0;
frac_t winratio;
int registeredmarkets = 0;
int activemarkets = 0;
int openmarkets = 0;
int closedmarkets = 0;

// local func defs
int parseargs(int, char**);

// main
int main(int argc, char** argv) {
	parseargs(argc, argv);
	
	// initialize frac_t type global vars
	walletvolume = fracnew(0, 1);
	
	liquidvolume = fracnew(0, 1);
	activevolume = fracnew(0, 1);
	liquidity = fracnew(0, 1);
	probabilitythreshold = DEFAULTPROBABILITYTHRESHOLD;
	winvolume = fracnew(0, 1);
	lossvolume = fracnew(0, 1);
	profitvolume = fracnew(0, 1);
	winratio = fracnew(0, 1);

	// create threads
	pthread_t tuithread;
	pthread_create(&tuithread, 0, tuiinit(&running), 0);
	
	// tui related
	tuicreatecmds();
	tuicreatehome();

	// join threads
	pthread_join(tuithread, 0);

	return 0;}

// local funcs
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
