#ifndef CNODES_H
#define CNODES_H

#include "history.h"

class CSin: public CHistoryWorker
{
public:
	CSin(ct A, ct B, ct W, CHistory *history = 0);
	virtual ~CSin();
	
	virtual void end();
	
	ct A, B, W;
};

class CExp: public CHistoryWorker
{
public:
	CExp(ct A, ct B, ct WHigh, ct WLow, CHistory *history = 0);
	virtual ~CExp();
	
	virtual void end();
	
	ct A, B, WHigh, WLow;
};

class CConst: public CHistoryWorker
{
public:
	CConst(compct C, CHistory *history = 0);
	virtual ~CConst();
	
	virtual void end();
	
	compct C;
};

class CLn: public CHistoryWorker
{
public:
	CLn(CHistory *history = 0);
	virtual ~CLn();

	virtual void end();
};


#endif // CNODES_H
