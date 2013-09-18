#ifndef MODIFIERS_H
#define MODIFIERS_H

#include "history.h"

class CAcceptor: public CHistoryWorker
{
public:
	CAcceptor(char op, QVector<CHistory*> historys);
	virtual ~CAcceptor();
	
	QVector<CHistory*> nodes;
	
	virtual void end();
	
	char op;
};

class CSubMid: public CHistoryWorker
{
public:
	CSubMid(CHistory *history);
	virtual ~CSubMid();
	
	virtual void end();
};

class CReverse: public CHistoryWorker
{
public:
	CReverse(CHistory *history);
	virtual ~CReverse();
	
	virtual void end();
};

class CNuller: public CHistoryWorker
{
public:
	CNuller(CHistory *history, int len, int st = 0);
	virtual ~CNuller();
	
	virtual void end();
	
	int len, st;
};

class CDelim: public CHistoryWorker
{
public:
	CDelim(CHistory *history, int delim);
	virtual ~CDelim();
	
	virtual void end();
	
	int delim;
};

class CShift: public CHistoryWorker
{
public:
	CShift(CHistory *history, QVector<compct> muls);
	virtual ~CShift();
	
	virtual void end();
	
	QVector<compct> muls;
};

class CSplit: public CHistoryWorker
{
public:
	CSplit(CHistory *history, CHistory *history2);
	virtual ~CSplit();
	
	virtual void end();
	
	CHistory *history2;
};

#endif // MODIFIERS_H
