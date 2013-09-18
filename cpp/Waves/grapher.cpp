#include "grapher.h"
#include "nodes.h"
#include <QObject>
#include <QFile>
#include "cf.h"
#include "modifiers.h"
#include "cff.h"
#include <stdio.h>


compct CGW1::step(QVector<QVector<ct> > args, QVector<CHistory *> waves)
{
	qDebug()<<waves[0]->str;
	CFF fourie(waves[0]);
	fourie.end();
	return fourie.points[32];
}
