#ifndef GENARRAY_H
#define GENARRAY_H

#include "history.h"

struct CPoint
{
	CPoint(int x, compct val);
	int x;
	compct val;
};



class CGenArray: public CHistoryWorker
{
public:
	CGenArray(QList<CPoint> array, CHistory *history = 0);
	virtual ~CGenArray();
	
	virtual void end();
	
	QList<CPoint> array;
};

#endif // GENARRAY_H
