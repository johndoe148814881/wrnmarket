#ifndef FRAC_H
#define FRAC_H

typedef struct {
	int num;
	int den;} frac_t;

extern frac_t fracnew(int, int);
extern void fracset(frac_t*, frac_t*);
extern int fraccmp(frac_t*, frac_t*);
extern void fracadd(frac_t*, frac_t*);
extern void fracsub(frac_t*, frac_t*);
extern double fractod(frac_t*);
extern frac_t dtofrac(double);

#endif

