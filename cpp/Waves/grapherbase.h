#ifndef GRAPHERBASE_H
#define GRAPHERBASE_H

#include "history.h"

class CGG
{
public:
	CGG(QVector<ct> args);
	virtual ~CGG();

	virtual void init();
	virtual int next();
	virtual void step(CHistory *wave);
	
	QVector<ct> args;
	QVector<ct> curargs;
};

class CGW
{
public:
	CGW();
	virtual ~CGW();
	
	virtual compct step(QVector<QVector<ct> > args, QVector<CHistory*> waves) = 0;
};

class CGrapher: public CHistoryWorker
{
public:
	CGrapher(QVector<CGG*> gens, CGW *work, char asHisto = 0, QString str = QString("Grapher"));
	virtual ~CGrapher();

	virtual void end();
	
	QVector<CGG*> gens;
	CGW *work;
	QVector<ct> args;
};

#endif // GRAPHERBASE_H
