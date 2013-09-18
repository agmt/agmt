#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include <math.h>

#include "nodes.h"
#include "painter.h"
#include "manager.h"
#include "cff.h"
#include "cf.h"
#include "genarray.h"
#include "modifiers.h"
#include "grapher.h"
#include "writer.h"
#include "old.h"
#include "contin.h"

extern ct curTime, stepTime;
extern CManager manager;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	
	ui->scrollArea->setWidget(new TmpWidget());
	ui->scrollArea->widget()->setLayout(new QVBoxLayout());
	setWindowState(Qt::WindowMaximized);
	
}

MainWindow::~MainWindow()
{
    delete ui;
}

void FullFur(CHistory *hist)
{
	CHistory *fourie = new CF(hist);
	new CAmp(fourie);
	new CPhase(fourie);
}

void FullFFur(CHistory *hist)
{
	CHistory *fourie = new CFF(hist);
	new CAmp(fourie);
	new CPhase(fourie);
}

void MainWindow::on_action_activated()
{
	CHistory *timer = new CTimer(65536.0/1024.0, 1024);
	CHistoryWorker *fourie, *ph, *amp;
	CExp *sin, *sin2, *sin3;
	CHistoryWorker *sum, *fsum;
	CHistoryWorker *fourieMy;
	QList<CHistory*> list;
	
	CContRect contRect(QList<compct>()<<0.25l);
	CContGauss contGauss(QList<compct>()<<1<<0.4l);
	
	CHistory *y1 = new CDiscrMaker(&contGauss, 50);
	CHistory *r = new CDiscrMaker(&contGauss, 8);
	new CDiscrMaker(&contRect, 50);
	CHistory *y2 = new CDiscrMaker(new CContinMaker(r), 50);
	
	CHistory *y = new CSplit(y1, y2);
	y->asHisto = false;
	
/*	sin = new CExp(5, 0.0l, 39.0, 0.1l * 32768.0l);
	sin2 = new CExp(0, 26214.0l, 36.0, 0.0 * 32768.0l);

	sum = new CAcceptor('+', QVector<CHistory*>()<<sin<<sin2);
	
	fourie = new CFF(sum);
	ph = new CInitPhase(fourie, 20, 50);
	
	new CCepstr(sum);
	new CCepstr(fourie);*/


//	new CGrapher(QVector<CGG*>()<<new CGG(QVector<ct>()<<1<<1<<1<<0<<65535<<10000<<32.4<<32.4<<1), new CGW1);
/*	new CMyGrapher(0, 32.1l, 0.1, "");
	new CMyGrapher(0, 32.2l, 0.1, "");
	new CMyGrapher(0, 32.3l, 0.1, "");
	new CMyGrapher(0, 32.4l, 0.1, "");
	new CMyGrapher(0, 32.5l, 0.1, "");
	new CMyGrapher(0, 32.6l, 0.1, "");
	new CMyGrapher(0, 32.7l, 0.1, "");
	new CMyGrapher(0, 32.8l, 0.1, "");
	new CMyGrapher(0, 32.9l, 0.1, "");
	new CMyGrapher(0, 33.0l, 0.1, "");
	/*new CMyGrapher(0, 0.74, 30.3l, "");
	new CMyGrapher(0, 0.74, 30.49l, "");*/
	
	manager.preend();
	foreach(CHistory *history, manager.workers)
	{	
		if(history->points.count())
			ui->scrollArea->widget()->layout()->addWidget(new CPainter(history));
	}
	manager.postend();
}
