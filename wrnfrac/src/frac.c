#include "../include/frac.h"
#include <math.h>
#include <stdlib.h>

/* author: claude */ int gcd(int a, int b) {
	a = llabs(a);
	b = llabs(b);
	while (b) {
		int t = b; b = a % b; a = t;}
	return a;}

/* author: claude */ void fracreduce(frac_t* f) {
	if (f->den < 0) {
		f->num = -f->num; f->den = -f->den;}
	int g = gcd(llabs(f->num), llabs(f->den));
	f->num /= g;
	f->den /= g;}

/* author: claude */ frac_t fracnew(int num, int den) {
	frac_t f = {num, den};
	fracreduce(&f);
	return f;}

void fracset(frac_t* a, frac_t* b) {
	fracreduce(b);
	a->num = b->num;
	a->den = b->den;}

/* author: claude */ int fraccmp(frac_t* a, frac_t* b) { // returns >0 if a>b, 0 if equal, <0 if a<b
	if (!a || !b) 
		return 0;
	int lhs = a->num * b->den;
	int rhs = b->num * a->den;
	return (lhs > rhs) - (lhs < rhs);}

/* author: claude */ void fracadd(frac_t* a, frac_t* b) {
	a->num = a->num * b->den + b->num * a->den;
	a->den = a->den * b->den;
	fracreduce(a);}

/* author: claude */ void fracsub(frac_t* a, frac_t* b) {
	a->num = a->num * b->den - b->num * a->den;
	a->den = a->den * b->den;
	fracreduce(a);}

/* author: claude */ double fractod(frac_t* a) {
	return (double)a->num / a->den;}

/* author: claude */ frac_t dtofrac(double x) {
	static int precision = 1000000;
	return fracnew((int)round(x * precision), precision);}

