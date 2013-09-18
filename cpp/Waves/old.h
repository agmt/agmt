#ifndef OLD_H
#define OLD_H

#include "history.h"

class CInitPhase_2: public CHistoryWorker
{
public:
	CInitPhase_2(CHistory *history, int Wmin = 0, int Wmax = INT_MAX, int acc = 100);
	virtual ~CInitPhase_2();
	
	virtual void end();
	
	int Wmin, Wmax;
	int acc;
};

class CUnFourieNull_2: public CHistoryWorker
{
public:
	CUnFourieNull_2(CHistory *history, int Wmin = 0, int Wmax = INT_MAX, int acc = 100);
	virtual ~CUnFourieNull_2();
	
	virtual void end();
	
	compct getFreqAmp();
	
	int Wmin, Wmax;
	int acc;
};

class CUnFourieNull_3: public CHistoryWorker
{
public:
	CUnFourieNull_3(CHistory *history, int Wmin = 0, int Wmax = INT_MAX, int acc = 100);
	virtual ~CUnFourieNull_3();
	
	virtual void end();
	
	compct getFreqAmp();
	
	int Wmin, Wmax;
	int acc;
};

class CUnFourieNull_4: public CHistoryWorker
{
public:
	CUnFourieNull_4(CHistory *history, int Wmin = 0, int Wmax = INT_MAX, int acc = 100);
	virtual ~CUnFourieNull_4();
	
	virtual void end();
	
	compct getFreqAmp();
	ct getSinc(ct c1, ct c2, ct c3, ct &a, ct c);
	
	int Wmin, Wmax;
	int acc;
};

class CInitPhaseWithNoize_3: public CHistoryWorker
{
public:
	CInitPhaseWithNoize_3(CHistory *history, ct NW, int Wmin = 1, int Wmax = INT_MAX/2, int acc = 40);
	virtual ~CInitPhaseWithNoize_3();
	
	virtual void end();
	
	int Wmin, Wmax;
	ct NW;
	int acc;
};

class CInitPhaseWithNoize_4: public CHistoryWorker
{
public:
	CInitPhaseWithNoize_4(CHistory *history, ct NW, int Wmin = 1, int Wmax = INT_MAX/2, int acc = 40);
	virtual ~CInitPhaseWithNoize_4();
	
	virtual void end();
	
	int Wmin, Wmax;
	ct NW;
	int acc;
};


#endif // OLD_H
