#include "../include/main.h"
#include "/home/wrn/projs/wrntui/include/tui.h"
#include <stdlib.h>
#include <string.h>

// local func defs
static void showwin(char*);
static int dep(int, char**);
static int wit(int, char**);
static void maind();
static void mainw();
static void mainn();
static void newsimc();

// global funcs
void tuicreatecmds() {
	cmdprefix = '/';

	cmdnew("dep", dep);
	cmdnew("wit", wit);}

void tuicreatehome() {
	showwin("main");}

// local funcs
static void showwin(char* winname) {
	infofreeall();
	bindfreeall();

	if (strcmp(winname, "main") == 0) {
		int walletw = 32;
		int walleth = 8;
		int graphsw = 32;
		int simulationc = 1 + walletw;
		int simulationw = tuiwidth - walletw - graphsw;
		
		char** walletoptv = 0; int walletoptc = 0;
		int bindd = 0; int bindw = 0;
		if (fraccmp(&walletvolume, &(frac_t){0, 1}) > 0) {
			walletoptv = realloc(walletoptv, sizeof(char*) * ++walletoptc);
			walletoptv[walletoptc - 1] = "deposit";
			bindd = 1;}
		if (fraccmp(&liquidvolume, &(frac_t){0, 1}) > 0) {
			walletoptv = realloc(walletoptv, sizeof(char*) * ++walletoptc);
			walletoptv[walletoptc - 1] = "withdraw";
			bindw = 1;}

		boxdraw(1, 1, walleth, walletw, tuiforev[0], "wallet", walletoptv, walletoptc);
		boxdraw(1, simulationc, walleth, simulationw, tuiforev[1], "simulation", (char*[]){"new"}, 1);
		boxdraw(1 + walleth, 1, tuiheight - walleth - 2, tuiwidth - graphsw, tuiforev[2],"markets",  0, 0);
		boxdraw(1, 1 + tuiwidth - graphsw, tuiheight - 2, graphsw, tuiforev[3], "graphs", 0, 0);
	
		free(walletoptv);

		int infowidth = 30;
		
		infonew(2, 3, infowidth, tuiforev[0], "volume", &walletvolume, INFOFRAC);
		infonew(2, simulationc + 2, infowidth, tuiforev[1], "liquid volume", &liquidvolume, INFOFRAC);
		infonew(3, simulationc + 2, infowidth, tuiforev[1], "active volume", &activevolume, INFOFRAC);
		infonew(4, simulationc + 2, infowidth, tuiforev[1], "liquidity", &liquidity, INFOFRAC);
		infonew(5, simulationc + 2, infowidth, tuiforev[1], "max open markets", &openmarketsthreshold, INFOINT);
		infonew(6, simulationc + 2, infowidth, tuiforev[1], "probability algorithm", &probabilityalgorithm, INFOINT);
		infonew(7, simulationc + 2, infowidth, tuiforev[1], "probability threshold", &probabilitythreshold, INFOFRAC);

		infonew(2, simulationc + 2 + infowidth, infowidth, tuiforev[1], "win volume", &winvolume, INFOFRAC);
		infonew(3, simulationc + 2 + infowidth, infowidth, tuiforev[1], "loss volume", &lossvolume, INFOFRAC);
		infonew(4, simulationc + 2 + infowidth, infowidth, tuiforev[1], "profit volume", &profitvolume, INFOFRAC);
		infonew(5, simulationc + 2 + infowidth, infowidth, tuiforev[1], "wins", &wins, INFOINT);
		infonew(6, simulationc + 2 + infowidth, infowidth, tuiforev[1], "losses", &losses, INFOINT);
		infonew(7, simulationc + 2 + infowidth, infowidth, tuiforev[1], "win ratio", &winratio, INFOFRAC);
		
		if (bindd)
			bindnew('d', maind);

		if (bindw)
			bindnew('w', mainw);

		bindnew('n', mainn);}
	else if (strcmp(winname, "newsim") == 0) {
		int newsimw = 50;
		int newsimh = 10;

		boxdraw(tuiheight / 2 - newsimh / 2 + 1, tuiwidth / 2 - newsimw / 2, newsimh, newsimw, tuiforev[4], "new simulation", (char*[]){"cancel", "new"}, 1);

		bindnew('c', newsimc);}
	else if (strcmp(winname, "dep") == 0) {
		int depw = 50;
		int deph = 3;

		boxdraw(tuiheight / 2 - deph / 2 + 1, tuiwidth / 2 - depw / 2, deph, depw, tuiforev[4], "deposit", (char*[]){"cancel", "deposit"}, 1);
		bindnew('c', newsimc);}
	else if (strcmp(winname, "wit") == 0) {
		int witw = 50;
		int with = 3;

		boxdraw(tuiheight / 2 - with / 2 + 1, tuiwidth / 2 - witw / 2, with, witw, tuiforev[4], "withdraw", (char*[]){"cancel", "withdraw"}, 1);
		bindnew('c', newsimc);}}

static void maind() {
	showwin("dep");}

static void mainw() {
	showwin("wit");}

static void mainn() {
	showwin("newsim");}

static void newsimc() {
	showwin("main");}

static int dep(int argc, char** argv) {
	if (argc == 1) {
		showwin("dep");
		return CMDSUCCESS;}

	if (argc != 2) {
		return CMDINVALIDARGC;}
	
	char* check;
	frac_t amount = fracnew(strtol(argv[1], &check, 10), 1);
						
	if (*check == '\0' && amount.num > 0) {
		fracadd(&liquidvolume, &amount);
		showwin("main");
		return CMDSUCCESS;}
	else 
		return CMDINVALIDARGV;}

static int wit(int argc, char** argv) {
	if (argc == 1) {
		showwin("wit");
		return CMDSUCCESS;}

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
		showwin("main");
		return CMDSUCCESS;}
	else
		return CMDINVALIDARGV;}

