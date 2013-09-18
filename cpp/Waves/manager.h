#ifndef CMANAGER_H
#define CMANAGER_H

#include <QList>
#include "history.h"
#include "painter.h"

class CManager
{
public:
	~CManager();
	QList<CHistory*> workers;
	QList<CPainter*> painters;
	
	void clear();
	void end();
	void preend();
	void postend();
};

#endif // CMANAGER_H
