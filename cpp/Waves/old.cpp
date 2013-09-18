#include "old.h"
#include <stdio.h>
#include "QObject"
#include "cf.h"
#include "cff.h"
#include "nodes.h"
#include "modifiers.h"

CInitPhase_2::CInitPhase_2(CHistory *history, int Wmin, int Wmax, int acc): CHistoryWorker(history)
{
	this->Wmin = Wmin;
	this->Wmax = Wmax;
	this->acc = acc;
	str = QObject::tr("InitPhase_2{%1}").arg(history->str);
}

CInitPhase_2::~CInitPhase_2()
{
}
	
void CInitPhase_2::end()
{
	CAmp *amp = new CAmp(history);
	CUnFourieNull *unFourie = new CUnFourieNull(amp, Wmin, Wmax, acc);
	CFF *fourie = new CFF(unFourie);
	CPhase *phase = new CPhase(history), *phase2 = new CPhase(fourie);
	amp->end();
	unFourie->end();
	fourie->end();
	phase->end();
	phase2->end();
	
	ct w = unFourie->getFreqAmp().real();
	
	int w_int = round(w);
	
	ct res = phase->points[w_int].real() - phase2->points[w_int].real();
	
	if(res>M_PI_2l)
		res -= M_PIl;
		
	if(res<-M_PI_2l)
		res += M_PIl;
	printf("Added_2: %.20llf\n", res);
	points.append(compct(res, w));
	
//	delete phase2;
//	delete phase;
//	delete fourie;
//	delete unFourie;
//	delete amp;
}

CUnFourieNull_2::CUnFourieNull_2(CHistory *history, int Wmin, int Wmax, int acc): CHistoryWorker(history)
{
	this->Wmin = Wmin;
	this->Wmax = Wmax;
	this->acc = acc;
	str = QObject::tr("CosWith0Phase_2{%1}").arg(history->str);
}

CUnFourieNull_2::~CUnFourieNull_2()
{

}

void CUnFourieNull_2::end()
{
	compct fa = getFreqAmp();
	
	str = QString("%1*PreCos_2(%2*i + %3)").arg((double)fa.imag()).arg((double)fa.real()).arg(0);
	
	fa.real() *= 2 * M_PIl / history->points.size();
	for(int i = 0; i < history->points.size(); i++)
	{
		compct cur = compct(fa.imag()*cos(fa.real()*ct(i)), fa.imag()*sin(fa.real()*ct(i)));
		points.append(cur);
	}
}

compct CUnFourieNull_2::getFreqAmp()
{
	int i;
	Wmin = 1;
	Wmax = history->points.size() - 2;
	
	int Wf = Wmin;
	
	for(i = Wmin+1; (i <= Wmax) && (i < history->points.size()); i++)
	{
		if(history->points[i].real() > history->points[Wf].real())
			Wf = i;
	}
	
	ct c2, a, sign;
	c2 = history->points[Wf].real();
	if(history->points[Wf-1].real() < history->points[Wf+1].real())
	{
		sign = 1;
	}else 
	{
		sign = -1;
	}
	
	a = 0;
	foreach(compct val, history->history->points)
	{
		a += val.real();
	}
	
	c2 /= a;
	
	
	ct cleft, cleftval, cright, crightval, cmid, cmidval;
	
	cleft = 0.0l;
	cleftval = sinc(cleft);
	cright = 1.0l;
	crightval = sinc(cright);
	
	for(int i = 0; i < acc; i++)
	{
		cmid = (cleft+cright)/2.0l;
		cmidval = sinc(cmid);
		
	//	printf("%i %llf %llf %llf %llf %llf %llf %llf\n", i, c2, cleft, cleftval, cright, crightval, cmid, cmidval);
		
		if(cmidval < c2)
		{
			cright = cmid;
			crightval = cmidval;
		}else
		{
			cleft = cmid;
			cleftval = cmidval;
		}
	}
	
	ct w = ct(Wf) + sign * cmid;
	
	return compct(w, a/ct(history->points.size()));
}


CUnFourieNull_3::CUnFourieNull_3(CHistory *history, int Wmin, int Wmax, int acc): CHistoryWorker(history)
{
	this->Wmin = Wmin;
	this->Wmax = Wmax;
	this->acc = acc;
	str = QObject::tr("CosWith0Phase_3{%1}").arg(history->str);
}

CUnFourieNull_3::~CUnFourieNull_3()
{

}

void CUnFourieNull_3::end()
{
	compct fa = getFreqAmp();
	
	str = QString("%1*PreCos_3(%2*i + %3)").arg((double)fa.imag()).arg((double)fa.real()).arg(0);
	
	fa.real() *= 2 * M_PIl / history->points.size();
	for(int i = 0; i < history->points.size(); i++)
	{
		compct cur = compct(fa.imag()*cos(fa.real()*ct(i)), fa.imag()*sin(fa.real()*ct(i)));
		points.append(cur);
	}
}

compct CUnFourieNull_3::getFreqAmp()
{
	int i;
	Wmin = 1;
	Wmax = history->points.size() - 2;
	
	int Wf = Wmin;
	
	for(i = Wmin+1; (i <= Wmax) && (i < history->points.size()); i++)
	{
		if(history->points[i].real() > history->points[Wf].real())
			Wf = i;
	}
	
	ct c2, a, sign;
	c2 = history->points[Wf].real();
	if(history->points[Wf-1].real() < history->points[Wf+1].real())
	{
		sign = 1;
	}else 
	{
		sign = -1;
	}
	
	a = 0;
	foreach(compct val, history->history->history->points)
	{
		a += abs(val.real());
	}
	a *= M_PI_2l;
	c2 /= a;
	
	
	ct cleft, cleftval, cright, crightval, cmid, cmidval;
	
	cleft = 0.0l;
	cleftval = sinc(cleft);
	cright = 1.0l;
	crightval = sinc(cright);
	
	for(int i = 0; i < acc; i++)
	{
		cmid = (cleft+cright)/2.0l;
		cmidval = sinc(cmid);
		
	//	printf("%i %llf %llf %llf %llf %llf %llf %llf\n", i, c2, cleft, cleftval, cright, crightval, cmid, cmidval);
		
		if(cmidval < c2)
		{
			cright = cmid;
			crightval = cmidval;
		}else
		{
			cleft = cmid;
			cleftval = cmidval;
		}
	}
	
	ct w = ct(Wf) + sign * cmid;
	
	return compct(w, a/ct(history->points.size()));
}

CUnFourieNull_4::CUnFourieNull_4(CHistory *history, int Wmin, int Wmax, int acc): CHistoryWorker(history)
{
	this->Wmin = Wmin;
	this->Wmax = Wmax;
	this->acc = acc;
	str = QObject::tr("CosWith0Phase_4{%1}").arg(history->str);
}

CUnFourieNull_4::~CUnFourieNull_4()
{

}

void CUnFourieNull_4::end()
{
	compct fa = getFreqAmp();
	
	str = QString("%1*PreCos_4(%2*i + %3)").arg((double)fa.imag()).arg((double)fa.real()).arg(0);
	
	fa.real() *= 2 * M_PIl / history->points.size();
	for(int i = 0; i < history->points.size(); i++)
	{
		compct cur = compct(fa.imag()*cos(fa.real()*ct(i)), fa.imag()*sin(fa.real()*ct(i)));
		points.append(cur);
	}
}

compct CUnFourieNull_4::getFreqAmp()
{
	int i;
	QVector<compct> array = history->points;
	
	Wmin = 1;
	Wmax = array.size() - 2;
	
	int Wf = Wmin;
	
	for(i = Wmin+1; (i <= Wmax) && (i < array.size()); i++)
	{
		if(array[i].real() > array[Wf].real())
			Wf = i;
	}
	
	ct c1, c2, c3, a, sign;
	c2 = array[Wf].real();
	if(array[Wf-1].real() < array[Wf+1].real())
	{
		c1 = array[Wf-1].real();
		c3 = array[Wf+1].real();
		sign = 1;
	}else 
	{
		c3 = array[Wf-1].real();
		c1 = array[Wf+1].real();
		sign = -1;
	}
	
	/*for(ct d = 0; d < 0.6l; d += 0.005l)
	{
		ct val;
		val = getSinc(c1, c2, c3, a, d);
		printf("%llf %llf %llf %llf %llf %llf\n", d, c1, c2, c3, a, val);
	}
	printf("End_4!\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	*/
	
	#define count 4
	
	ct *cs = new ct[count + 2], *csval = new ct[count + 2];
	
	cs[1] = 0;
	csval[1] = getSinc(c1, c2, c3, a, cs[1]);
	cs[count] = 0.5l;
	csval[count] = getSinc(c1, c2, c3, a, cs[count]);
	//printf("\n\n\nINIT: %llf %llf %llf %llf\n", c1, c2, c3, sign);
	for(int i = 0; i < acc; i++)
	{
		cs[0] = cs[1];
		csval[0] = csval[1];
		cs[count+1] = cs[count];
		csval[count+1] = csval[count];
		int min = 1;
		for(int j = 2; j < count; j++)
		{
			cs[j] = cs[1] + (cs[count]-cs[1])*(j-1)/(count-1);
			csval[j] = getSinc(c1, c2, c3, a, cs[j]);
		//	printf("%llf %llf ", cs[j], csval[j]);
			if(csval[j] < csval[min])
				min = j;
		}
		if(csval[count] < csval[min])
			min = count;
	//	printf("\n %i %llf %llf %llf %llf %i\n", i, cs[1], csval[1], cs[count], csval[count], min);
		cs[1] = cs[min - 1];
		csval[1] = csval[min - 1];
		cs[count] = cs[min + 1];
		csval[count] = csval[min + 1];
	
	}
	getSinc(c1, c2, c3, a, cs[1]);
	ct w = ct(Wf) + sign * (cs[1] + cs[count]) / 2.0l;
	
	delete[] cs;
	delete[] csval;
	
	#undef count
	
	return compct(w, a/ct(history->points.size()));
}

ct CUnFourieNull_4::getSinc(ct c1, ct c2, ct c3, ct &a, ct c)
{
//	printf("Init: %llf %llf %llf %llf\n", c1, c2, c3, c);
	ct sum = c1 - 2*c2 + c3;
	a = -sum*(1-c*c)/((2-2*c-2*c*c)*sinc(c));
	ct b = c2 - a*sinc(c);
	c1 -= b;
	c2 -= b;
	c3 -= b;
//	printf("Next: %llf %llf %llf %llf %llf\n", c1, c2, c3, a, b);
	ct toc1 = abs(a*sinc(c+1));
	ct toc3 = abs(a*sinc(c-1));
	ct err = (c1 - toc1)*(c1 - toc1) + (c3 - toc3)*(c3 - toc3);
//	printf("Err: %llf\n", err);
	return err;
}

CInitPhaseWithNoize_3::CInitPhaseWithNoize_3(CHistory *history, ct NW, int Wmin, int Wmax, int acc): CHistoryWorker(history)
{
	this->Wmin = Wmin;
	this->Wmax = Wmax;
	this->NW = NW;
	this->acc = acc;
	str = QObject::tr("InitPhaseWithNoize_3{%1}").arg(history->str);
}

CInitPhaseWithNoize_3::~CInitPhaseWithNoize_3()
{
}
	
void CInitPhaseWithNoize_3::end()
{
	CHistory *noize = new CExp(1, 0, round(NW), (round(NW) - NW)*32768.0l);
	CHistory *delta = new CAcceptor('*', QVector<CHistory*>()<<history<<noize);
	CHistory *fourie = new CFF(delta);
	noize->end();
	delta->end();
	fourie->end();
	
	CInitPhase *initPhase = new CInitPhase(fourie, floor(NW) + Wmin, floor(NW) + Wmax + 1, acc);
	initPhase->end();
	points.append(compct(initPhase->points[0].real() - M_PI_4, initPhase->points[0].imag() - NW));
	
	delete fourie;
	delete initPhase;
	delete noize;
	delete delta;
}

CInitPhaseWithNoize_4::CInitPhaseWithNoize_4(CHistory *history, ct NW, int Wmin, int Wmax, int acc): CHistoryWorker(history)
{
	this->Wmin = Wmin;
	this->Wmax = Wmax;
	this->NW = NW;
	this->acc = acc;
	str = QObject::tr("InitPhaseWithNoize_4{%1}").arg(history->str);
}

CInitPhaseWithNoize_4::~CInitPhaseWithNoize_4()
{
}
	
void CInitPhaseWithNoize_4::end()
{
	CHistory *noize = new CExp(1, 0, round(NW), (round(NW)-NW)*32768.0l);
	CHistory *delta = new CAcceptor('*', QVector<CHistory*>()<<history<<noize);
	CHistory *fourie = new CFF(delta);
	noize->end();
	delta->points.resize(history->points.count());
	for(int i = 0; i < delta->points.count(); i++)
	{
		delta->points[i] = history->points[i].real()*noize->points[i].real();
	}
	fourie->end();
	
	CInitPhase *initPhase = new CInitPhase(fourie, floor(NW) + Wmin, floor(NW) + Wmax + 1, acc);
	initPhase->end();
	
	points.append(compct(initPhase->points[0].real(), initPhase->points[0].imag() - NW));
	
	delete fourie;
	delete initPhase;
	delete noize;
	delete delta;
}
