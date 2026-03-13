#ifndef MAIN_H
#define MAIN_H
#include "/home/wrn/projs/wrnfrac/include/frac.h" 
#define POLYMARKETFIGURE 0
#define TOPHOLDERSPERFORMANCE 1
#define DEFAULTOPENMARKETSTHRESHOLD 10
#define DEFAULTPROBABILITYALGORITHM POLYMARKETFIGURE
#define DEFAULTPROBABILITYTHRESHOLD fracnew(1, 2)

extern int running;
extern int simulationloaded;

extern frac_t walletvolume;

extern frac_t liquidvolume;
extern frac_t activevolume;
extern frac_t liquidity;
extern int openmarketsthreshold;
extern int probabilityalgorithm;
extern frac_t probabilitythreshold;
extern frac_t winvolume;
extern frac_t lossvolume;
extern frac_t profitvolume;
extern int wins;
extern int losses;
extern frac_t winratio;
extern int registeredmarkets;
extern int activemarkets;
extern int openmarkets;
extern int closedmarkets;

#endif

