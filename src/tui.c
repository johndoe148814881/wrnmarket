#include "../include/main.h"
#include "../wrntui/include/tui.h"
#include <stdlib.h>
#include <string.h>

// local func defs
static int depcmd(int, char**);
static int witcmd(int, char**);

static void mainwin();
static void mainwinupdate();
static void maind();
static void mainw();
static void mainn();

static void newsimwin();
static void newsimc();
static void newsimn();

static void depwin();
static void depc();
static void depd();
static void depwinlistleft(int, void**);
static void depwinlistright(int, void**);

static void witwin();
static void witc();
static void witw();
static void witwinlistleft(int, void**);
static void witwinlistright(int, void**);

// local vars
static int maindcreated = 0;
static int mainwcreated = 0;

static int depwinvolume = 0;

static int witwinvolume = 0;

// global funcs
void tuicreate() {
	cmdprefix = '/';
	cmdnew("dep", depcmd);
	cmdnew("wit", witcmd);
	winnew("main", mainwin, mainwinupdate);
	winnew("newsim", newsimwin, 0);
	winnew("dep", depwin, 0);
	winnew("wit", witwin, 0);}

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
	
	maindcreated = 0;
	mainwcreated = 0;

	boxbindnew("simulation", "new", mainn);
	if ((maindcreated = fraccmp(&walletvolume, &(frac_t){0, 1}) > 0))
		boxbindnew("wallet", "deposit", maind);
	if ((mainwcreated = fraccmp(&liquidvolume, &(frac_t){0, 1}) > 0))
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

	listnew(walleth + 2, 3, tuiheight - walleth - 4, tuiwidth - graphsw - 4, 1);
	listaddfield(infowidth, tuiforev[2], "int", INFOINT);
	listaddfield(infowidth, tuiforev[2], "frac_t", INFOFRAC);
	listaddrecord((void*[]){&openmarketsthreshold, &liquidvolume});
	listaddrecord((void*[]){&registeredmarkets, &activevolume});}

static void mainwinupdate() {
	if (!maindcreated && (maindcreated = fraccmp(&walletvolume, &(frac_t){0, 1}) > 0))
		boxbindnew("wallet", "deposit", maind);
	else if (maindcreated && !(maindcreated = fraccmp(&walletvolume, &(frac_t){0, 1}) > 0))
		boxbindfree("wallet", "deposit", maind);

	if (!mainwcreated && (mainwcreated = fraccmp(&liquidvolume, &(frac_t){0, 1}) > 0))
		boxbindnew("wallet", "withdraw", mainw);
	else if (mainwcreated && !(mainwcreated = fraccmp(&liquidvolume, &(frac_t){0, 1}) > 0))
		boxbindfree("wallet", "withdraw", mainw);}

static void newsimwin() {
	int boxw = 50;
	int boxh = 10;

	boxnew(tuiheight / 2 - boxh / 2 + 1, tuiwidth / 2 - boxw / 2, boxh, boxw, tuiforev[4], "new simulation");
	boxbindnew("new simulation", "new", newsimn);
	boxbindnew("new simulation", "cancel", newsimc);}

static void depwin() {
	depwinvolume = 10;

	int boxw = 50; int boxh = 5;
	int boxx = tuiwidth / 2 - boxw / 2; int boxy = tuiheight / 2 - boxh / 2 + 1;

	boxnew(boxy, boxx, boxh, boxw, tuiforev[4], "deposit");
	boxbindnew("deposit", "deposit", depd);
	boxbindnew("deposit", "cancel", depc);
	
	msgnew(boxy + 2, boxx + 2, 20, "volume: ");
	
	listnew(boxy + 2, boxx + 2 + 20, boxh - 2, boxw - 4 - 20, 0);
	listaddfield(boxw - 4 - 20, tuiforev[4], "volume", INFOINT);
	listaddrecord((void*[]){&depwinvolume});
	listbindleft(depwinlistleft);
	listbindright(depwinlistright);}

static void witwin() {
	witwinvolume = 10;

	int boxw = 50; int boxh = 5;
	int boxx = tuiwidth / 2 - boxw / 2; int boxy = tuiheight / 2 - boxh / 2 + 1;

	boxnew(boxy, boxx, boxh, boxw, tuiforev[4], "withdraw");
	boxbindnew("withdraw", "withdraw", witw);
	boxbindnew("withdraw", "cancel", witc);
	
	msgnew(boxy + 2, boxx + 2, 20, "volume: ");
	
	listnew(boxy + 2, boxx + 2 + 20, boxh - 2, boxw - 4 - 20, 0);
	listaddfield(boxw - 4 - 20, tuiforev[4], "volume", INFOINT);
	listaddrecord((void*[]){&witwinvolume});
	listbindleft(witwinlistleft);
	listbindright(witwinlistright);}

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

static void depc() {
	winshow("main");}

static void depd() {
	frac_t add = fracnew(depwinvolume, 1);
	fracadd(&liquidvolume, &add);
	winshow("main");}

static void witc() {
	winshow("main");}

static void witw() {
	frac_t sub = fracnew(witwinvolume, 1);
	frac_t set = fracnew(0, 1);
	set = liquidvolume;
	fracsub(&set, &sub);
	set = fractod(&set) < 0 ? fracnew(0, 1) : set;
	liquidvolume = set;
	winshow("main");}

static void depwinlistleft(int recordc, void** recordv) {
	(void)recordc; (void)recordv;
	depwinvolume -= 1;
	depwinvolume = depwinvolume < 0 ? 0 : depwinvolume;}

static void depwinlistright(int recordc, void** recordv) {
	(void)recordc; (void)recordv;
	depwinvolume += 1;}

static void witwinlistleft(int recordc, void** recordv) {
	(void)recordc; (void)recordv;
	witwinvolume -= 1;
	witwinvolume = witwinvolume < 0 ? 0 : witwinvolume;}

static void witwinlistright(int recordc, void** recordv) {
	(void)recordc; (void)recordv;
	witwinvolume += 1;}

static int depcmd(int argc, char** argv) {
	if (argc == 1) {
		winshow("dep");
		return CMDSUCCESS;}

	if (argc != 2) {
		return CMDINVALIDARGC;}
	
	char* check;
	frac_t amount = fracnew(strtol(argv[1], &check, 10), 1);
						
	if (*check == 0 && fractod(&amount) > 0) {
		fracadd(&liquidvolume, &amount);
		winshow("main");
		return CMDSUCCESS;}
	else 
		return CMDINVALIDARGV;}

static int witcmd(int argc, char** argv) {
	if (argc == 1) {
		winshow("wit");
		return CMDSUCCESS;}

	if (argc != 2) 
		return CMDINVALIDARGC;
	
	if (strcmp(argv[1], "all") == 0) {
		frac_t set = fracnew(0, 0);
		fracset(&liquidvolume, &set);
		winshow("main");
		return CMDSUCCESS;}
			
	char* check;
	frac_t amount = fracnew(strtol(argv[1], &check, 10), 1);
	amount = fraccmp(&amount, &liquidvolume) > 0 ? liquidvolume : amount; 
				
	if (*check == 0 && fractod(&amount) > 0) {
		fracsub(&liquidvolume, &amount);
		winshow("main");
		return CMDSUCCESS;}
	else
		return CMDINVALIDARGV;}
