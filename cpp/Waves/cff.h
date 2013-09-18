#ifndef CFF_H
#define CFF_H

#include "kiss_fft_guts.h"
#include "history.h"

class CFF: public CHistoryWorker
{
public:
	CFF(CHistory *history, bool fun = false);
	virtual ~CFF();
	virtual void end();
	
	bool fun;

};

typedef compct Complex;
typedef compct ShortComplex;

/*
  Fast Fourier Transformation
  x: x - array of items
  N: N - number of items in array
  complement: false - normal (direct) transformation, true - reverse transformation
*/
void universal_fft(ShortComplex *x, int N, bool complement);

#endif // CFF_H
