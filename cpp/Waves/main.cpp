#include <QtGui/QApplication>
#include <QTextCodec>
#include "mainwindow.h"
#include <stdio.h>
#include <math.h>
#include <cmath>
#include <complex>
#include "cf.h"
#include "cff.h"
#include <iostream>
#include <QDebug>
#include "nodes.h"

using namespace std;

int main(int argc, char *argv[])
{
	CHistory::initInt(8);

	
/*	for(int i = 0; i <= 16; i++)
	{
		printf("%i %i %llf\n", i, CHistory::fixsinc(i>>2, (i&0x3)<<13), fabs(sinc(((double)i)/4.0l)) * 32768);
	}*/
	
/*	for(int i = 0; i < 100; i++)
	{
		int vall = CHistory::fixsin(i*65536/100);
		printf("Sin: %i %llf\n", vall, (long double)sin(i*M_PIl/50.0l));
	}*/
	
/*	for(int i = 0; i < 10; i++)
	{
		int vall = CHistory::fixatan((i*65536+6)/10);
		printf("Atan: %i %llf\n", vall, (long double)65536.0l*atan(((long double)i)/10.0)/M_PIl);
	}*/
	
	/*for(int i = 32760; i < 32768; i++)
	{
		for(int j = 32760; j < 32768; j++)
		{
			int vall = CHistory::fixatan2(i, j);
			printf("Atan: %i %i %i %llf %llf\n", i, j, vall, (long double)atan2(i, j), (long double)atan2(i, j)*32768.0/M_PIl);
		}
	}*/

	QApplication app(argc, argv);
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
	
	MainWindow w;
	w.show();
	
	return app.exec();
}
