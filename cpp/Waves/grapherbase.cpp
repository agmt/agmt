#include "grapherbase.h"
#include "nodes.h"

CGG::CGG(QVector<ct> args)
{
	this->args = args;
}

CGG::~CGG()
{

}

void CGG::init()
{
	for(int i = 0; i < args.count()/3; i++)
	{
		curargs.append(args[i*3]);
	}
}

int CGG::next()
{
	int i = 0;
	for(;;)
	{
		curargs[i] += args[i*3 + 2];
		if(curargs[i] > args[i*3 + 1])
		{
			curargs[i] = args[i*3];
			i++;
			if(i >= curargs.count())
				return 1;
		}else
		{
			break;
		}
	}
	return 0;
}

void CGG::step(CHistory *wave)
{
	wave->~CHistory();
	wave = new(wave) CExp(curargs[0], curargs[1], curargs[2], 0);
	wave->end();
}

CGW::CGW()
{

}

CGW::~CGW()
{

}

CGrapher::CGrapher(QVector<CGG *> gens, CGW *work, char asHisto, QString str): CHistoryWorker()
{
	this->gens = gens;
	this->work = work;
	this->str = str;
	this->asHisto = asHisto;
}

CGrapher::~CGrapher()
{
}

void CGrapher::end()
{
	QVector < QVector < ct > > args;
	QVector<CHistory*> waves;
	int i;
	waves.resize(gens.count());
	args.resize(gens.count());
	for(i = 0; i < gens.count(); i++)
	{
		gens[i]->init();
		args[i] = gens[i]->curargs;
		waves[i] = new CExp(0, 0, 0, 0);
		gens[i]->step(waves[i]);
	}
	for(;;)
	{
		points.append(work->step(args, waves));	
		for(i = 0; i < gens.count(); i++)
		{
			int res = gens[i]->next();
			if((res) && (i == gens.count() - 1))
			{
				i++;
				break;
			}
			args[i] = gens[i]->curargs;
			gens[i]->step(waves[i]);
			if(!res)
				break;
		}
		if(i >= gens.count())
			break;	
	}
	for(i = 0; i < gens.count(); i++)
		delete gens[i];
	gens.clear();
	delete work;
}
