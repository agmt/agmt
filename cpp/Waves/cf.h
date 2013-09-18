#ifndef CF_H
#define CF_H

#include "history.h"

class CF: public CHistoryWorker
{
public:
	CF(CHistory *history, bool fun = false, int Wmin = 1, int Wmax = INT_MAX);
	virtual ~CF();
	bool fun;
	virtual void end();

	int Wmin, Wmax;
};

class CReal: public CHistoryWorker
{
public:
	CReal(CHistory *history);
	virtual ~CReal();
	
	virtual void end();
};

class CAmp: public CHistoryWorker
{
public:
	CAmp(CHistory *history);
	virtual ~CAmp();
	
	virtual void end();
	
	ct getAmp(ct high, ct low);
};

class CPhase: public CHistoryWorker
{
public:
	CPhase(CHistory *history);
	virtual ~CPhase();
	
	virtual void end();
	
	ct getPhase(ct high, ct low);
};

class CInitPhase: public CHistoryWorker
{
public:
	CInitPhase(CHistory *history, int Wmin = 1, int Wmax = INT_MAX/2, int acc = 40);
	virtual ~CInitPhase();
	
	virtual void end();
	
	int Wmin, Wmax;
	int acc;
};

class CInitPhaseWithNoize: public CHistoryWorker
{
public:
	CInitPhaseWithNoize(CHistory *history, ct NWhigh, ct NWlow, int Wmin = 1, int Wmax = INT_MAX/2, int acc = 40);
	virtual ~CInitPhaseWithNoize();
	
	virtual void end();
	
	int Wmin, Wmax;
	ct NWhigh, NWlow;
	int acc;
};

class CInitPhaseWithNoize_2: public CHistoryWorker
{
public:
	CInitPhaseWithNoize_2(CHistory *history, ct NWhigh, ct NWlow, int Wmin = 1, int Wmax = INT_MAX/2, int acc = 40);
	virtual ~CInitPhaseWithNoize_2();
	
	virtual void end();
	
	int Wmin, Wmax;
	ct NWhigh, NWlow;
	int acc;
};

class CInitPhaseWithNoizeFull: public CHistoryWorker
{
public:
	CInitPhaseWithNoizeFull(CHistory *history, ct NWhigh, ct NWlow, ct a, ct b, int Wmin = 1, int Wmax = INT_MAX/2, int acc = 40);
	virtual ~CInitPhaseWithNoizeFull();
	
	virtual void end();
	
	int Wmin, Wmax;
	ct NWhigh, NWlow;
	ct a, b;
	int acc;
};

class CUnFourieNull: public CHistoryWorker
{
public:
	CUnFourieNull(CHistory *history, int Wmin = 1, int Wmax = INT_MAX/2, int acc = 40);
	virtual ~CUnFourieNull();
	
	virtual void end();
	
	compct getFreqAmp();
	ct getSinc(ct c1, ct c2, ct c3, ct &a, ct c);
	SAMPPROD fixGetSinc(kiss_fft_scalar c1, kiss_fft_scalar c2, kiss_fft_scalar c3, SAMPUNS &a, kiss_fft_scalar c);
	
	int Wmin, Wmax;
	int acc;
};

class CCepstr: public CHistoryWorker
{
public:
	CCepstr(CHistory *history, bool fun = false, int Wmin = 1, int Wmax = INT_MAX);
	virtual ~CCepstr();
	bool fun;
	virtual void end();

	int Wmin, Wmax;
};

#endif // CF_H
