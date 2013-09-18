#ifndef CCONTIN_H
#define CCONTIN_H

#include <QList>
#include <QString>
#include "history.h"

class CContin: public CHistoryWorker
{
public:
	CContin(QList<compct> args, QString str, CHistory *history = 0);
	virtual ~CContin();

	QList<compct> args;
	virtual compct get(ct t) = 0;
};

class CDiscrMaker : public CHistoryWorker
{
public:
	CDiscrMaker(CContin *func, int step);
	virtual ~CDiscrMaker();

	virtual void end();

	int step;
	CContin *func;
};

class CContRect: public CContin
{
public:
	CContRect(QList<compct> args);
	virtual compct get(ct t);
};

class CContGauss: public CContin
{
public:
	CContGauss(QList<compct> args);
	virtual compct get(ct t);
};

class CContinMaker: public CContin
{
public:
	CContinMaker(CHistory *history);
	virtual ~CContinMaker();

	virtual compct get(ct t);
	
	virtual void end();
};



#endif // CCONTIN_H
