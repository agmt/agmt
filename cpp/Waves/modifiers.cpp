#include "modifiers.h"

#include <QObject>
#include <QVector>

CAcceptor::CAcceptor(char op, QVector<CHistory*> nodes): CHistoryWorker(0)
{
	this->op = op;
	this->nodes = nodes;
}

CAcceptor::~CAcceptor()
{

}

void CAcceptor::end()
{
	str = QObject::tr("(");
	
	foreach(CHistory *node, nodes)
	{
		if(node != nodes[0])
		{
			str += QObject::tr(" %1 ").arg(op);	
		}
		str += node->str;
	}
	str += QObject::tr(")");
	
	points.reserve(history->points.count());
	for(int i = 0; i < nodes[0]->points.size(); i++)
	{
		compct val;
		if(op == '+')
		{
			val = nodes[0]->points[i];
			for(int j = 1; j < nodes.count(); j++)
				val += nodes[j]->points[i];
		}
		else if(op == '-')
		{
			val = nodes[0]->points[i];
			for(int j = 1; j < nodes.count(); j++)
				val -= nodes[j]->points[i];
		}else if(op == '*')
		{
			val = compct(1, 1);
			foreach(CHistory *node, nodes)
			{
				val *= node->points[i];
			//	val.real() *= node->points[i].real();
			//	val.imag() *= node->points[i].imag();
			}
		}	
		points.append(val);
	}
	if(asInt)
		updInt();
}


CReverse::CReverse(CHistory *history): CHistoryWorker(history)
{
}

CReverse::~CReverse()
{
}
	
void CReverse::end()
{
	this->str = QObject::tr("Reverse{%1}").arg(history->str);
	points.resize(history->points.count());
	for(int i = 0; i < history->points.count(); i++)
	{
		points[i] = history->points[history->points.count() - i - 1];
	}
}


CSubMid::CSubMid(CHistory *history): CHistoryWorker(history)
{
}

CSubMid::~CSubMid()
{
}
	
void CSubMid::end()
{
	this->str = QObject::tr("SubMid{%1}").arg(history->str);
	compct mid(0, 0);
	foreach(compct numb, history->points)
	{
		mid += numb;
	}
	mid /= history->points.size();
	
	points.reserve(history->points.count());
	foreach(compct numb, history->points)
	{
		points.append(numb - mid);
	}
}


CNuller::CNuller(CHistory *history, int len, int st): CHistoryWorker(history)
{
	this->len = len;
	this->st = st;
}

CNuller::~CNuller()
{
}
	
void CNuller::end()
{
	this->str = QObject::tr("Nuller{%1}").arg(history->str);
	points.reserve(len);
	
	if((st + len) > history->points.count())
	{
		for(int i = st; i < history->points.count(); i++)
		{
			points.append(history->points[i]);
		}
		
		for(int i = history->points.count(); i < (st + len); i++)
		{
			points.append(compct(0, 0));
		}
	}else
	{
		for(int i = st; i < (st + len); i++)
		{
			points.append(history->points[i]);
		}
	}
}



CDelim::CDelim(CHistory *history, int delim): CHistoryWorker(history)
{
	this->delim = delim;
}

CDelim::~CDelim()
{

}
	
void CDelim::end()
{
	str = QObject::tr("Delim{%1, %2}").arg(delim).arg(history->str);
	compct mid;
	for(int i = 0; i < history->points.count()/delim; i++)
	{
		mid = compct(0, 0);
		for(int j = 0; j < delim; j++)
		{
			mid += history->points[i*delim + j];
		}
		points.append(mid/compct(delim));
	}
}

CShift::CShift(CHistory *history, QVector<compct> muls): CHistoryWorker(history)
{
	this->muls = muls;
}

CShift::~CShift()
{

}
	
void CShift::end()
{
	str = QObject::tr("Shift{%1}").arg(history->str);
	points.reserve(history->points.count());
	for(int i = 0; i < history->points.count(); i++)
	{
		compct res = compct(0, 0);
		for(int j = 0; j < muls.count(); j++)
		{
			if(i + muls.count()-1 - j < history->points.count())
			{
				res = res + muls[j]*history->points[i + muls.count()-1 - j];
			}
		}
		points.append(res);
	}
}

CSplit::CSplit(CHistory *history, CHistory *history2): CHistoryWorker(history)
{
	this->history2 = history2;
}

CSplit::~CSplit()
{

}
	
void CSplit::end()
{
	this->str = QString("(%1) + i(%2)").arg(history->str).arg(history2->str);

	points.resize(qMax(history->points.count(), history2->points.count()));
	for(int i = 0; i < history->points.count(); i++)
	{
		points[i].real(history->points[i].real());
	}
	
	for(int i = 0; i < history2->points.count(); i++)
	{
		points[i].imag(history2->points[i].real());
	}
}
