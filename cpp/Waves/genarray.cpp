#include "genarray.h"

#include <QObject>
#include <QVector>

CGenArray::CGenArray(QList<CPoint> array, CHistory *history): CHistoryWorker(history)
{	
	this->array = array;
}

CGenArray::~CGenArray()
{
}
	
void CGenArray::end()
{
	str = QObject::tr("Массив");
	for(int i = 0; i < history->points.size(); i++)
		points.append(compct(0, 0));
	
	foreach(CPoint point, array)
	{
		points[point.x] = point.val;
	}
}

CPoint::CPoint(int x, compct val)
{
	this->x = x;
	this->val = val;
}
