#include <math.h>
#include "history.h"
#include "manager.h"

extern CManager manager;
int accdis = 16;

int CHistory::__valBits;
kiss_fft_scalar *CHistory::__fixsin;
SAMPUNS *CHistory::__fixatan;

CHistory::CHistory(CHistory *history)
{
	this->history = history;
	manager.workers.append(this);
	asHisto = true;
	asScaled = true;
	if(history)
	{
		asInt = history->asInt;
	}else
	{
		asInt = false;
	}
}

CHistory::~CHistory()
{
	manager.workers.removeOne(this);
}

void CHistory::end()
{

}

void CHistory::toInt()
{
	ct min, max;
	getMinMax(min, max);
	
	ct maxVal = ct((1 << (accdis-1)));
	
	for(int i = 0; i < points.count(); i++)
	{
		points[i].real(round((points[i].real() - min) * maxVal / (max-min)) - (1 << (accdis-2)));
		points[i].imag(round((points[i].imag() - min) * maxVal / (max-min)) - (1 << (accdis-2)));
	}
}

void CHistory::updInt()
{
	int bits;
	ct min, max;
	getMinMax(min, max);
	kiss_fft_scalar fmax = round(qMax(qAbs(min), qAbs(max)));
	if(fmax == 0)
	{
		return;
	}
	if(fmax > 16384)
	{
		bits = -1;
	}else if(fmax <= 8192)
	{
		bits = 1;
		while((fmax << bits) <= 8192)
			bits++;
	}else
	{
		return;
	}
	
	
	kiss_fft_scalar val;
	for(int i = 0; i < points.count(); i++)
	{
		val = round(points[i].real());
		if(bits > 0)
		{
			val = val << bits;
		}else
		{
			val = val >> -bits;
		}
		points[i].real(val);
		
		val = round(points[i].imag());
		if(bits > 0)
		{
			val = val << bits;
		}else
		{
			val = val >> -bits;
		}
		points[i].imag(val);
	}
}

QVector <kiss_fft_cpx> CHistory::getInt()
{
	QVector <kiss_fft_cpx> res;
	
	for(int i = 0; i < points.count(); i++)
	{
		kiss_fft_cpx cur;
		cur.real(round(points[i].real()));
		cur.imag(round(points[i].imag()));
		res.append(cur);
	}
	return res;
}

void CHistory::getMinMax(ct &min, ct &max)
{
	if(points.size()>0)
		min = max = points[0].real();
	else
		min = max = 0;
	foreach(compct val, points)
	{
		if(val.real() < min)
			min = val.real();
		if(val.imag() < min)
			min = val.imag();
		if(val.real() > max)
			max = val.real();
		if(val.imag() > max)
			max = val.imag();
	}
}

CTimer::CTimer(ct step, size_t times): CHistory(0)
{
	str = QObject::tr("t");
	
	for(size_t i = 0; i < times; i++)
	{
		points.append(compct(step*ct(i)));
	}
	
}

CTimer::~CTimer()
{

}
	
void CTimer::end()
{
	if(asInt)
		toInt();
}

CHistoryWorker::CHistoryWorker(CHistory *history): CHistory(history)
{
	if(history)
	{
		this->history = history;
	}else
	{
		this->history = manager.workers[0];
	}
}

CHistoryWorker::~CHistoryWorker()
{
}

void CHistoryWorker::end()
{

}

void CHistory::initInt(int n)
{
	__valBits = n;
	__fixsin = new kiss_fft_scalar[1 << __valBits + 1];
	__fixatan = new SAMPUNS[1 << __valBits + 1];
	int t;
	for(int i = 0; i <= (1 << __valBits); i++)
	{
		t = round((SAMP_MAX)*sin(2 * M_PIl * (double)i / (double)(1 << __valBits)));
		__fixsin[i] = t;
		t = round(((long double)((1<<(VALBITS+1))))*atan(((long double)i) / (long double)(1 << __valBits)) / M_PIl);
		if(t < 0)
			t = 65536 + t;
		__fixatan[i] = t;
	}
}

SAMPUNS CHistory::fixatan(SAMPUNS x)
{
	SAMPPROD val1 = (SAMPUNS)__fixatan[x>>(VALBITS - __valBits)], val2 = (SAMPUNS)__fixatan[(x>>(VALBITS - __valBits)) + 1];
	SAMPUNS delta = ((x<<__valBits)&0xFFFF)>>__valBits; // हिन्दू कोड
	SAMPUNS all = 1<<(VALBITS - __valBits);
	return (val1*(all-delta) + val2*delta)>>(VALBITS - __valBits+1);
}

SAMPUNS CHistory::fixatan2(kiss_fft_scalar y, kiss_fft_scalar x)
{
	if(y < 0)
		return 65536 - fixatan2(-y, x);
	if(x < 0)
		return (1<<(VALBITS-1)) - fixatan2(y, -x);
	
	if(y == 0)
		return 0;
	if(x == 0)
		return 1<<(VALBITS-2);
	
	if(x == y)
		return 1<<(VALBITS-3);
	if(y > x)
	{
		return (1<<(VALBITS-2)) - fixatan2(x, y);
	}
	return ((SAMPUNS)fixatan(S_DIV(y, x)))>>1;
}

kiss_fft_scalar CHistory::fixsin(SAMPUNS x)
{
	SAMPPROD val1 = __fixsin[x>>(VALBITS - __valBits)], val2 = __fixsin[(x>>(VALBITS - __valBits)) + 1];
	kiss_fft_scalar delta = ((x<<__valBits)&0xFFFF)>>__valBits; // हिन्दू कोड
	kiss_fft_scalar all = 1<<(VALBITS - __valBits);
	return (val1*(all-delta) + val2*delta)>>(VALBITS - __valBits);
}

kiss_fft_scalar CHistory::fixcos(SAMPUNS x)
{
	SAMPUNS arg = x + (1<<(VALBITS-2));
	return fixsin(arg);
}

SAMPUNS CHistory::fixsqrt(SAMPPROD l)
{
	long temp, div;
	unsigned  rslt = (unsigned)l;
	if (l <=  0)
		return 0;
	else if (l & 0xFFFF0000L)
		if  (l & 0xFF000000L)
			div  = 0x3FFF;
		else
			div  = 0x3FF;
	else if  (l & 0x0FF00L)
			div  = 0x3F;
		else
			div  = (l > 4) ? 0x7 : l;
	for(;;)
	{
		temp = l  / div + div;
		div =  temp >>  1;
		div += temp  & 1;
		if  (rslt > div)
			rslt = (unsigned)div;
		else
		{
			if (l / rslt == rslt - 1 && l % rslt == 0)
				rslt--;
			return rslt;
		}
	}

}

SAMPUNS CHistory::fixsinc(kiss_fft_scalar high, kiss_fft_scalar low)
{
	if(low < 0)
	{
		high--;
		low = 0x8000 + low;
	}
	if(high < 0)
	{
		high = -high;
		low = -low;
		if(low < 0)
		{
			high--;
			low = 0x8000 + low;
		}
	}
	if(!(high || low))
	{
		return 0x8000;
	}
	kiss_fft_scalar r = fixsin(low);
	SAMPPROD r2 = (SAMPPROD)r * (SAMPPROD)10431; // $/Pi
	SAMPUNS res = r2 / ((((SAMPPROD)high)<<15) + (SAMPPROD)low);
	return res;
}

void CHistory::getHighLow(ct w, ct *high, ct *low)
{
	*high = round(w);
	*low = (w - (ct)(*high))*32768.0;
}

ct abs(ct x)
{
	if(x > 0)
		return x;
	return -x;
}

ct sinc(ct x)
{
	if(abs(x) < 2)
	{
		ct res = 0;
		ct x2 = -x*x*M_PIl*M_PIl;
		ct cur = 1;
		
		for(int i = 2; i < 100; i += 2)
		{
			res += cur;
			
			cur *= x2 / (((ct)(i)) * ((ct)(i+1))); 
		}
		
		return res;
	}
	x = abs(x);
	ct nx = x;
	while(nx > 2)
		nx -= 2;
	return sinl(M_PIl*nx)/(M_PIl*x);
}

ct ctrand()
{
	ct sum = 0, cur, div = 1;
	for(int i = 0; i < sizeof(ct); i++)
	{
		cur = rand()&0xFF;
		div *= 256;
		cur /= div;
		sum += cur;
	}
	return sum;
}
