#include "../include/main.h"
#include "../wrntui/include/tui.h"
#include <stdlib.h>
#include <string.h>

// local func defs
static int dep(int, char**);
static int wit(int, char**);
static void maind();
static void mainw();
static void mainn();
static void newsimc();
static void newsimn();
static void mainwin();
static void newsimwin();
static void depwin();
static void witwin();

// global funcs
void tuicreate() {
	cmdprefix = '/';
	cmdnew("dep", dep);
	cmdnew("wit", wit);
	winnew("main", mainwin);
	winnew("newsim", newsimwin);
	winnew("dep", depwin);
	winnew("wit", witwin);}

// local funcs
static void mainwin() {
	int walletw = 64;
	int walleth = 8;
	int graphsw = 32;
	int simulationc = 1 + walletw;
	int simulationw = tuiwidth - walletw - graphsw;
	int infowidth = 30;

	boxnew(1, 1, walleth, walletw, tuiforev[0], "wallet");
	boxnew(1, simulationc, walleth, simulationw, tuiforev[1], "simulation");
	boxnew(1 + walleth, 1, tuiheight - walleth - 2, tuiwidth - graphsw, tuiforev[2], "markets");
	boxnew(1, 1 + tuiwidth - graphsw, tuiheight - 2, graphsw, tuiforev[3], "graphs");
	
	boxbindnew("simulation", "new", mainn);
	if (fraccmp(&walletvolume, &(frac_t){0, 1}) > 0)
		boxbindnew("wallet", "deposit", maind);
	if (fraccmp(&liquidvolume, &(frac_t){0, 1}) > 0)
		boxbindnew("wallet", "withdraw", mainw);

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

	infonew(2, simulationc + 2 + infowidth * 2, infowidth, tuiforev[1], "registered markets", &registeredmarkets, INFOINT);
	infonew(3, simulationc + 2 + infowidth * 2, infowidth, tuiforev[1], "active markets", &activemarkets, INFOINT);
	infonew(4, simulationc + 2 + infowidth * 2, infowidth, tuiforev[1], "open markets", &openmarkets, INFOINT);
	infonew(5, simulationc + 2 + infowidth * 2, infowidth, tuiforev[1], "closed markets", &closedmarkets, INFOINT);

	listnew(walleth + 2, 3, tuiheight - walleth - 4, tuiwidth - graphsw - 4);
	listaddfield(infowidth, tuiforev[2], "INFOINT", INFOINT);
	listaddfield(infowidth, tuiforev[2], "INFOFRAC", INFOFRAC);
	listaddrecord((void*[]){&openmarketsthreshold, &liquidvolume});
	listaddrecord((void*[]){&registeredmarkets, &activevolume});}

static void newsimwin() {
	int newsimw = 50;
	int newsimh = 10;

	boxnew(tuiheight / 2 - newsimh / 2 + 1, tuiwidth / 2 - newsimw / 2, newsimh, newsimw, tuiforev[4], "new simulation");
	boxbindnew("new simulation", "new", newsimn);
	boxbindnew("new simulation", "cancel", newsimc);}

static void depwin() {
	int depw = 50;
	int deph = 3;

	boxnew(tuiheight / 2 - deph / 2 + 1, tuiwidth / 2 - depw / 2, deph, depw, tuiforev[4], "deposit");
	boxbindnew("deposit", "deposit", newsimn);
	boxbindnew("deposit", "cancel", newsimc);}

static void witwin() {
	int witw = 50;
	int with = 3;

	boxnew(tuiheight / 2 - with / 2 + 1, tuiwidth / 2 - witw / 2, with, witw, tuiforev[4], "withdraw");
	boxbindnew("withdraw", "withdraw", newsimn);
	boxbindnew("withdraw", "cancel", newsimc);}

static void maind() {
	winshow("dep");}

static void mainw() {
	winshow("wit");}

static void mainn() {
	winshow("newsim");}

static void newsimc() {
	winshow("main");}

static void newsimn() {
	}

static int dep(int argc, char** argv) {
	if (argc == 1) {
		winshow("dep");
		return CMDSUCCESS;}

	if (argc != 2) {
		return CMDINVALIDARGC;}
	
	char* check;
	frac_t amount = fracnew(strtol(argv[1], &check, 10), 1);
						
	if (*check == '\0' && amount.num > 0) {
		fracadd(&liquidvolume, &amount);
		winshow("main");
		return CMDSUCCESS;}
	else 
		return CMDINVALIDARGV;}

static int wit(int argc, char** argv) {
	if (argc == 1) {
		winshow("wit");
		return CMDSUCCESS;}

	if (argc != 2) 
		return CMDINVALIDARGC;
	
	if (strcmp(argv[1], "all") == 0) {
		frac_t set = fracnew(0, 1);
		fracset(&liquidvolume, &set);
		winshow("main");
		return CMDSUCCESS;}
			
	char* check;
	frac_t amount = fracnew(strtol(argv[1], &check, 10), 1);
				
	if (*check == '\0' && amount.num > 0) {
		fracsub(&liquidvolume, &amount);
		winshow("main");
		return CMDSUCCESS;}
	else
		return CMDINVALIDARGV;}
