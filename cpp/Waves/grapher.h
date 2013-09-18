#ifndef GRAPHER_H
#define GRAPHER_H

#include "grapherbase.h"

class CGW1: public CGW
{
public:
	virtual compct step(QVector<QVector<ct> > args, QVector<CHistory*> waves);
};

#endif // GRAPHER_H
