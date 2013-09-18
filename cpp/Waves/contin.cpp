#include "contin.h"
#include "cf.h"
#include <stdio.h>
#include <QDebug>

CContin::CContin(QList<compct> args, QString str, CHistory *history): CHistoryWorker(history)
{
	this->args = args;
	this->str = str;
}

CContin::~CContin()
{
}

CDiscrMaker::CDiscrMaker(CContin *func, int step): CHistoryWorker()
{
	this->func = func;
	this->step = step;
}

CDiscrMaker::~CDiscrMaker()
{

}

void CDiscrMaker::end()
{
	str = QString("[%1,%2]").arg(step).arg(func->str);
	
	for(int i = 0; i < step; i++)
	{
		this->points.append(func->get(ct(i+0.5l)/ct(step)));
	}
}

CContRect::CContRect(QList<compct> args): CContin(args, QString("Прямоуг"))
{

}

compct CContRect::get(ct t)
{
	if((t < args[0].real()) || (t > (1.0l-args[0].real())))
	{
		return 0;
	}
	return 1;
}

CContGauss::CContGauss(QList<compct> args): CContin(args, QString("Гаусс"))
{

}

compct CContGauss::get(ct t)
{
	t -= 0.5l;
	return args[0] * exp(-t*t/(args[1]*args[1]));
}

CContinMaker::CContinMaker(CHistory *history): CContin(QList<compct>(), QString("Непр{%1}").arg(history->str), history)
{

}

CContinMaker::~CContinMaker()
{

}

compct CContinMaker::get(ct t)
{
	ct res = 0;
	ct mul1, mul2;
	for(int i = 0; i < history->points.count(); i++)
	{
		mul1 = history->points[i].real();
		mul2 = sinc(t*history->points.count() - i - 0.5l);
		res +=  mul1*mul2;
	}

	return res;
}

void CContinMaker::end()
{
	this->str = QString("Непр{%1}").arg(history->str);
}
