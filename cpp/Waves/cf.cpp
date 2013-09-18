#include "cf.h"
#include <QObject>
#include <QVector>
#include <stdio.h>
#include "cff.h"
#include "modifiers.h"
#include "nodes.h"

CF::CF(CHistory *history, bool fun, int Wmin, int Wmax): CHistoryWorker(history)
{
	this->fun = fun;
	this->Wmin = Wmin;
	this->Wmax = Wmax;
}
 
CF::~CF()
{

}

void CF::end()
{
	str = QObject::tr("Фурье{%1}").arg(history->str);
	compct arg;
	
	if(fun)
	{
		arg = compct(2 * M_PIl) * compct(0, 1) / compct(history->points.size());
	}else
	{
		arg = compct(-2 * M_PIl) * compct(0, 1) / compct(history->points.size());
	}
	
	points.resize(history->points.size());

	int maxN = Wmax;
	if(maxN >= history->points.size())
		maxN = history->points.size() - 1;

	for(int i = Wmin - 1; i <= maxN; i++)
	{
		compct cur = 0;
		for(int j = 0; j < history->points.size(); j++)
		{
			compct arg2 = arg * compct(i * j);
			cur += history->points[j] * compct(exp(arg2));
		}
		if(fun)
			cur /= compct(history->points.size());
		
		points[i] = cur;
	}
}

CReal::CReal(CHistory *history): CHistoryWorker(history)
{
}

CReal::~CReal()
{
}
	
void CReal::end()
{
	str = QObject::tr("Re{%1}").arg(history->str);
	foreach(compct numb, history->points)
	{
		points.append(compct(numb.real(), 0));
	}
}

CAmp::CAmp(CHistory *history): CHistoryWorker(history)
{
}

CAmp::~CAmp()
{
}
	
void CAmp::end()
{
	str = QObject::tr("A{%1}").arg(history->str);
	points.reserve(history->points.count());
	foreach(compct numb, history->points)
	{
		if(asInt)
		{
			SAMPPROD r = numb.real(), i = numb.imag();
			SAMPPROD sq = fixsqrt(r*r + i*i);
			if(sq < 0)
				qDebug()<<"isqrt<0";
			points.append(compct(sq, 0));
		}else
		{
			points.append(compct(sqrt(numb.real()*numb.real() + numb.imag()*numb.imag()), 0));
		}
	}
}

ct CAmp::getAmp(ct high, ct low)
{
	int w_int = round(high);
	compct numb = history->points[w_int];
	if(asInt)
	{
		SAMPPROD r = numb.real(), i = numb.imag();
		SAMPPROD sq = fixsqrt(r*r + i*i);
		if(sq < 0)
			qDebug()<<"isqrt<0";
		SAMPPROD a = (sq<<15) / fixsinc(0, round(low));
		if(abs(a) > 32767)
			qDebug()<<"abs()>32767";
		return a;
	}else
	{
		ct a = sqrt(numb.real()*numb.real() + numb.imag()*numb.imag()) / sinc(low/32768.0);
		return a;
	}
}

CPhase::CPhase(CHistory *history): CHistoryWorker(history)
{
}

CPhase::~CPhase()
{
}
	
void CPhase::end()
{
	str = QObject::tr("Ф{%1}").arg(history->str);
	foreach(compct numb, history->points)
	{
		if(asInt)
		{
			points.append(fixatan2(numb.imag(), numb.real()));
		}else
		{
			points.append(atan2l(numb.imag(), numb.real()));
		}
	}
}

ct CPhase::getPhase(ct high, ct low)
{
	int w_int = round(high);
	if(asInt)
	{
		SAMPPROD sclow = round(low);
		SAMPUNS curPhase = CHistory::fixatan2(history->points[w_int].imag(), history->points[w_int].real());
		SAMPUNS sum = S_MUL(sclow, 32768 - 32768/history->points.count());
		
		ct res = (SAMPUNS)(curPhase - sum);
		return res;
	}else
	{
		ct curPhase = atan2l(history->points[w_int].imag(), history->points[w_int].real());
		
		ct sum = ((low / 32768.0) * M_PIl * (1.0 - 1.0/ct(history->points.count())));
		ct res = curPhase - sum;
		
		if(res < 0)
			res += 2.0l * M_PIl;
		if(res >= 2.0l * M_PIl)
			res -= 2.0l * M_PIl;
		res = res * 32768.0l / M_PIl;
		return res;
	}
}

CInitPhase::CInitPhase(CHistory *history, int Wmin, int Wmax, int acc): CHistoryWorker(history)
{
	this->Wmin = Wmin;
	this->Wmax = Wmax;
	this->acc = acc;
}

CInitPhase::~CInitPhase()
{
}
	
void CInitPhase::end()
{
	str = QObject::tr("Нач.Ф{%1}").arg(history->str);
	CAmp *amp = new CAmp(history);
	CUnFourieNull *unFourie = new CUnFourieNull(amp, Wmin, Wmax, acc);
	CPhase *phase = new CPhase(history);
	amp->end();
	ct w = unFourie->getFreqAmp().real();
	ct high, low;
	getHighLow(w, &high, &low);
	ct res = phase->getPhase(high, low);
	
	points.append(compct(res, w));
	
	delete phase;
	delete unFourie;
	delete amp;
}

CInitPhaseWithNoize::CInitPhaseWithNoize(CHistory *history, ct NWhigh, ct NWlow, int Wmin, int Wmax, int acc): CHistoryWorker(history)
{
	this->Wmin = Wmin;
	this->Wmax = Wmax;
	this->NWhigh = NWhigh;
	this->NWlow = NWlow;
	this->acc = acc;
}

CInitPhaseWithNoize::~CInitPhaseWithNoize()
{
}
	
void CInitPhaseWithNoize::end()
{
	CPhase *phase = new CPhase(history);
	CAmp *amp = new CAmp(history);
	ct a = amp->getAmp(NWhigh, NWlow);
	CExp *node = new CExp(a, phase->getPhase(NWhigh, NWlow), NWhigh, NWlow);
	node->asInt = asInt;
	CAcceptor *delta = new CAcceptor('-', QVector<CHistory*>()<<history->history<<node);
	delta->asInt = asInt;
	CFF *fourie = new CFF(delta);
	node->end();
	delta->end();
	fourie->end();
	
	CInitPhase *initPhase = new CInitPhase(fourie, Wmin, Wmax, acc);
	initPhase->end();
	
	points.append(initPhase->points[0]);
	
	str = QObject::tr("InitPhaseWithNoize{%1}{%2;%3}").arg(history->str).arg((double)((double)NWhigh + (double)NWlow/32768.0)).arg((double)a);
	
	delete fourie;
	delete initPhase;
	delete phase;
	delete node;
	delete amp;
	delete delta;
}

CInitPhaseWithNoize_2::CInitPhaseWithNoize_2(CHistory *history, ct NWhigh, ct NWlow, int Wmin, int Wmax, int acc): CHistoryWorker(history)
{
	this->Wmin = Wmin;
	this->Wmax = Wmax;
	this->NWhigh = NWhigh;
	this->NWlow = NWlow;
	this->acc = acc;
}

CInitPhaseWithNoize_2::~CInitPhaseWithNoize_2()
{
}
	
void CInitPhaseWithNoize_2::end()
{
	CHistory *fourie = new CHistoryWorker(0);
	fourie->asInt = asInt;
	fourie->points = history->points;
	CAmp *amp = new CAmp(history);
	ct a = amp->getAmp(NWhigh, NWlow);
	int iWL = round(NWhigh) - 1, iWR = round(NWhigh) + 1;
	
	if(asInt)
	{
		ct iPhaseL = atan2l(fourie->points[iWL].imag(), fourie->points[iWL].real());
		ct iPhaseR = atan2l(fourie->points[iWR].imag(), fourie->points[iWR].real());
		
		int maxN = Wmax + 1;
		if(maxN >= history->points.size())
			maxN = history->points.size() - 1;
		for(int i = Wmin - 1; i <= maxN; i++)
		{
			compct sub;
			ct curAmp = a * abs(sinc((ct(i) - NWhigh) + NWlow/32768.0));
			int iW;
			
			if(i <= floor(NWhigh))
			{
				sub.real(curAmp * cosl(iPhaseL + (ct(i - iWL))*M_PIl/fourie->points.count()));
				sub.imag(curAmp * sinl(iPhaseL + (ct(i - iWL))*M_PIl/fourie->points.count()));
			}else
			{
				sub.real(curAmp * cosl(iPhaseR + (ct(i - iWR))*M_PIl/fourie->points.count()));
				sub.imag(curAmp * sinl(iPhaseR + (ct(i - iWR))*M_PIl/fourie->points.count()));
			}
			fourie->points[i] -= sub;
		}
		str = QObject::tr("FixedInitPhaseWithNoize_2{%1}{%2;%3}").arg(history->str).arg((double)(NWhigh + NWlow/32768.0)).arg((double)a);
		CInitPhase *initPhase = new CInitPhase(fourie, Wmin, Wmax, acc);
		initPhase->end();
		points.append(initPhase->points[0]);
		delete initPhase;
	}else
	{		
		ct iPhaseL = atan2l(fourie->points[iWL].imag(), fourie->points[iWL].real());
		ct iPhaseR = atan2l(fourie->points[iWR].imag(), fourie->points[iWR].real());
		
		int maxN = Wmax + 1;
		if(maxN >= history->points.size())
			maxN = history->points.size() - 1;
		for(int i = Wmin - 1; i <= maxN; i++)
		{
			compct sub;
			ct curAmp = a * abs(sinc((ct(i) - NWhigh) + NWlow/32768.0));
			int iW;
			
			if(i <= floor(NWhigh))
			{
				sub.real(curAmp * cosl(iPhaseL + (ct(i - iWL))*M_PIl/fourie->points.count()));
				sub.imag(curAmp * sinl(iPhaseL + (ct(i - iWL))*M_PIl/fourie->points.count()));
			}else
			{
				sub.real(curAmp * cosl(iPhaseR + (ct(i - iWR))*M_PIl/fourie->points.count()));
				sub.imag(curAmp * sinl(iPhaseR + (ct(i - iWR))*M_PIl/fourie->points.count()));
			}
			fourie->points[i] -= sub;
		}
		str = QObject::tr("InitPhaseWithNoize_2{%1}{%2;%3}").arg(history->str).arg((double)(NWhigh + NWlow/32768.0)).arg((double)a);
		CInitPhase *initPhase = new CInitPhase(fourie, Wmin, Wmax, acc);
		initPhase->end();
		points.append(initPhase->points[0]);
		delete initPhase;
	}

	delete amp;
	delete fourie;
}

CInitPhaseWithNoizeFull::CInitPhaseWithNoizeFull(CHistory *history, ct NWhigh, ct NWlow, ct a, ct b, int Wmin, int Wmax, int acc): CHistoryWorker(history)
{
	this->Wmin = Wmin;
	this->Wmax = Wmax;
	this->NWhigh = NWhigh;
	this->NWlow = NWlow;
	this->acc = acc;
	this->a = a;
	this->b = b;
}

CInitPhaseWithNoizeFull::~CInitPhaseWithNoizeFull()
{
}
	
void CInitPhaseWithNoizeFull::end()
{
	CExp *node = new CExp(a, b, NWhigh, NWlow);
	node->asInt = asInt;
	CAcceptor *delta = new CAcceptor('-', QVector<CHistory*>()<<history->history<<node);
	delta->asInt = asInt;
	CFF *fourie = new CFF(delta);
	node->end();
	delta->end();
	fourie->end();
	
	CInitPhase *initPhase = new CInitPhase(fourie, Wmin, Wmax, acc);
	initPhase->end();
	
	points.append(initPhase->points[0]);
	
	str = QObject::tr("InitPhaseWithNoizeFull{%1}{%2;%3}").arg(history->str).arg((double)((double)NWhigh + (double)NWlow/32768.0)).arg((double)a);
	
	delete fourie;
	delete initPhase;
	delete node;
	delete delta;
}

CUnFourieNull::CUnFourieNull(CHistory *history, int Wmin, int Wmax, int acc): CHistoryWorker(history)
{
	this->Wmin = Wmin;
	this->Wmax = Wmax;
	this->acc = acc;
}

CUnFourieNull::~CUnFourieNull()
{

}

void CUnFourieNull::end()
{
	compct fa = getFreqAmp();
	
	str = QString("%1*PreCos(%2*i + %3)").arg((double)fa.imag()).arg((double)fa.real()).arg(0);
	
	fa.real() *= 2 * M_PIl / history->points.size();
	for(int i = 0; i < history->points.size(); i++)
	{
		compct cur = compct(fa.imag()*cos(fa.real()*ct(i)), fa.imag()*sin(fa.real()*ct(i)));
		points.append(cur);
	}
}

compct CUnFourieNull::getFreqAmp()
{
	int i;
	ct w, a;
	int Wf = Wmin;
	int maxN = Wmax + 1;
	if(maxN >= history->points.size())
		maxN = history->points.size() - 1;

	
	for(i = Wmin+1; i < maxN; i++)
	{
		if(history->points[i].real() > history->points[Wf].real())
			Wf = i;
	}
	
	
	if(asInt)
	{
		kiss_fft_scalar c1, c2, c3;
		SAMPUNS fixA;
		bool sign = false;
		c2 = history->points[Wf].real();
		if(history->points[Wf-1].real() < history->points[Wf+1].real())
		{
			c1 = round(history->points[Wf-1].real());
			c3 = round(history->points[Wf+1].real());
			sign = false;
		}else 
		{
			c3 = round(history->points[Wf-1].real());
			c1 = round(history->points[Wf+1].real());
			sign = true;
		}
		
		#define count 4
		
		kiss_fft_scalar cs[count + 2];
		SAMPPROD csval[count + 2];
		
		cs[1] = 0;
		csval[1] = fixGetSinc(c1, c2, c3, fixA, cs[1]);
		cs[count] = 16384;
		csval[count] = fixGetSinc(c1, c2, c3, fixA, cs[count]);
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
				csval[j] = fixGetSinc(c1, c2, c3, fixA, cs[j]);
				if(csval[j] < csval[min])
					min = j;
			}
			if(csval[count] < csval[min])
				min = count;
			cs[1] = cs[min - 1];
			csval[1] = csval[min - 1];
			cs[count] = cs[min + 1];
			csval[count] = csval[min + 1];
		
		}
		fixGetSinc(c1, c2, c3, fixA, cs[2]);
		kiss_fft_scalar low = ((SAMPUNS)cs[1] + (SAMPUNS)cs[count])>>1;
		if(sign)
			low = -low;
		
		w = ct(Wf) + ct(low)/32768.0l;
		a = fixA;
		
		#undef count
	}else
	{
		ct c1, c2, c3, sign;
		c2 = history->points[Wf].real();
		if(history->points[Wf-1].real() < history->points[Wf+1].real())
		{
			c1 = history->points[Wf-1].real();
			c3 = history->points[Wf+1].real();
			sign = 1;
		}else 
		{
			c3 = history->points[Wf-1].real();
			c1 = history->points[Wf+1].real();
			sign = -1;
		}
		
		/*for(ct d = 0; d < 0.6l; d += 0.005l)
		{
			ct val;
			val = getSinc(c1, c2, c3, a, d);
			printf("%llf %llf %llf %llf %llf %llf\n", d, c1, c2, c3, a, val);
		}
		printf("End_0!\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
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
		//		printf("%llf %llf ", cs[j], csval[j]);
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
		getSinc(c1, c2, c3, a, cs[2]);
		w = ct(Wf) + sign * (cs[1] + cs[count]) / 2.0l;
		
		delete[] cs;
		delete[] csval;
		
		#undef count
	}
	
	return compct(w, a);
}

ct CUnFourieNull::getSinc(ct c1, ct c2, ct c3, ct &a, ct c)
{
	a = c2 / sinc(c);
	ct toc1 = abs(a*sinc(c+1));
	ct toc3 = abs(a*sinc(c-1));
	ct err = (c1 - toc1)*(c1 - toc1) + (c3 - toc3)*(c3 - toc3);
	return err;
}

SAMPPROD CUnFourieNull::fixGetSinc(kiss_fft_scalar c1, kiss_fft_scalar c2, kiss_fft_scalar c3, SAMPUNS &a, kiss_fft_scalar c)
{
	SAMPPROD curA = (c2<<15) / fixsinc(0, c);
	if(curA & 0xFFFF0000)
	{
		printf("fixGetSinc Amp: %i     c: %i\n", curA, c);
		return 0x40000000 + (curA>>3);
	}
	a = curA & 0xFFFF;
	kiss_fft_scalar toc1 = S_MUL(a, fixsinc(1, c));
	kiss_fft_scalar toc3 = S_MUL(a, fixsinc(-1, c));
	SAMPPROD err = ((SAMPPROD)(c1 - toc1))*((SAMPPROD)(c1 - toc1)) + ((SAMPPROD)(c3 - toc3))*((SAMPPROD)(c3 - toc3));
	return err;
}

CCepstr::CCepstr(CHistory *history, bool fun, int Wmin, int Wmax): CHistoryWorker(history)
{
	this->fun = fun;
	this->Wmin = Wmin;
	this->Wmax = Wmax;
}
 
CCepstr::~CCepstr()
{

}

void CCepstr::end()
{
	str = QObject::tr("Кепстр{%1}").arg(history->str);
	compct arg;
	
	if(fun)
	{
		arg = compct(2 * M_PIl) * compct(0, 1) / compct(history->points.size());
	}else
	{
		arg = compct(2 * M_PIl) * compct(0, 1) / compct(history->points.size());
	}
	
	points.resize(history->points.size());

	int maxN = Wmax;
	if(maxN >= history->points.size())
		maxN = history->points.size() - 1;
	for(int i = Wmin - 1; i <= maxN; i++)
	{
		compct cur = 0;
		for(int j = 0; j < history->points.size(); j++)
		{
			compct arg2 = arg * compct(i * j);
			ct re = history->points[j].real();
			ct im = history->points[j].imag();
			ct val = history->points[j].real()*history->points[j].real();
			cur += compct(log(val)) * compct(exp(arg2));
		//	qDebug()<<QString("numbs %1 %2").arg((double)cur.real()).arg((double)cur.imag());
		}
		if(fun)
			cur /= compct(history->points.size());
		
		points[i] = cur;
	}
}
