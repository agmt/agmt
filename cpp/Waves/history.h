#ifndef CHISTORY_H
#define CHISTORY_H

#include <complex>
#include <QList>
#include <QString>
#include <QVector>
#include <math.h>
#include "kiss_fft_guts.h"
#include <QDebug>

#ifndef M_PIl
#define M_PIl M_PI
#endif
#ifndef M_PI_2l
#define M_PI_2l M_PI_2
#endif

typedef long double ct;
typedef std::complex<ct> compct;

class CHistory
{
public:
	CHistory(CHistory *history);
	virtual ~CHistory();
	
	virtual void end();
	
	void toInt();
	void updInt();
	
	QVector <kiss_fft_cpx> getInt(); 
	
	void getMinMax(ct &min, ct &max);
	
	QVector <compct> points;
	char asInt;
	char asHisto;
	char asScaled;
	
	QString str;
	
	CHistory *history;
	
	static void initInt(int n);
	static SAMPUNS fixatan(SAMPUNS x);
	static SAMPUNS fixatan2(kiss_fft_scalar y, kiss_fft_scalar x);
	static kiss_fft_scalar fixsin(SAMPUNS x);
	static kiss_fft_scalar fixcos(SAMPUNS x);
	static SAMPUNS fixsqrt(SAMPPROD n);
	static SAMPUNS fixsinc(kiss_fft_scalar high, kiss_fft_scalar low);
	
	static void getHighLow(ct w, ct *high, ct *low);
	
	static int __valBits;
	static kiss_fft_scalar *__fixsin;
	static SAMPUNS *__fixatan;
};

class CTimer: public CHistory
{
public:
	CTimer(ct step, size_t times);
	virtual ~CTimer();
	
	virtual void end();
};

class CHistoryWorker: public CHistory
{
public:
	CHistoryWorker(CHistory *history = 0);
	virtual ~CHistoryWorker();
	
	virtual void end();
};

ct sinc(ct x);
ct ctrand();

#endif // CHISTORY_H
