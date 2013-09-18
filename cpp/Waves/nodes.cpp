#include "nodes.h"
#include <QObject>

extern ct curTime, stepTime;

CSin::CSin(ct A, ct B, ct W, CHistory *history): CHistoryWorker(history)
{
	this->A = A;
	this->B = B;
	this->W = W;
}

CSin::~CSin()
{
}

void CSin::end()
{
	str = QObject::tr("%1 * cos(%2*%3 + %4)").arg(double(A)).arg(double(W)).arg(this->history->str).arg(double(B));
	foreach(compct cur, history->points)
	{
		compct val;
		ct arg = W * cur.real() + B;
		
		val = compct(A*cos(arg), A*sin(arg));
		points.append(val);
	}
}

CExp::CExp(ct A, ct B, ct WHigh, ct WLow, CHistory *history): CHistoryWorker(history)
{
	this->A = A;
	this->B = B;
	ct W = WHigh + WLow/32768.0l;
	getHighLow(W, &this->WHigh, &this->WLow);
}

CExp::~CExp()
{

}

void CExp::end()
{
	str = QObject::tr("%1 * exp(%2*%3 + %4<%5>)").arg(double(A)).arg(double(WHigh + WLow/32768.0l)).arg(this->history->str).arg(double(B)).arg(double(B*M_PIl/32768.0));
	points.reserve(history->points.count());
	foreach(compct cur, history->points)
	{
		if(asInt)
		{
			SAMPUNS amp = round(A);
			kiss_fft_scalar high = round(WHigh), low = round(WLow);
			SAMPUNS time = round(cur.real());
			SAMPUNS argh = ((((SAMPPROD)time)*(SAMPPROD)high));
			SAMPUNS argl1 = S_MUL(time, low);
			SAMPUNS argl = argl1 + ((SAMPUNS)round(B));
			SAMPUNS arg = ((SAMPUNS)argl) + argh;
			points.append(compct(S_MUL(fixcos(arg), amp), S_MUL(fixsin(arg), amp)));
		}else
		{
			compct val;
			ct arg = ((WHigh + WLow/32768.0l) * cur.real() + B)*M_PIl/32768.0l;
			
			std::complex<long double> arg2 = 0;
			arg2.imag(arg);
			
			val = exp(arg2);
			val *= A;
			
			points.append(val);
		}
	}
}

CConst::CConst(compct C, CHistory *history): CHistoryWorker(history)
{
	this->C = C;
}

CConst::~CConst()
{
}

void CConst::end()
{
	str = QObject::tr("%1+i%2").arg(double(C.real())).arg(double(C.imag()));
	for(size_t i = 0; i < history->points.size(); i++)
	{
		points.append(C);
	}
}

CLn::CLn(CHistory *history): CHistoryWorker(history)
{
}

CLn::~CLn()
{
}

void CLn::end()
{
	str = QObject::tr("Ln(%1)").arg(history->str);
	points.reserve(history->points.count());
	for(size_t i = 0; i < history->points.count(); i++)
	{
		points.append(log(history->points[i]));
	}
}


