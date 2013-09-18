#ifndef WRITER_H
#define WRITER_H

#include "history.h"

class CWriter: public CHistoryWorker
{
public:
    CWriter(CHistory *history, bool im = false);
	virtual ~CWriter();
	bool im;
	
	virtual void end();
};

#endif // WRITER_H
