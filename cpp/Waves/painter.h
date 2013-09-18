#ifndef CPAINTER_H
#define CPAINTER_H

#include <QWidget>
#include <QtGui>
#include "history.h"

class CPainter: public QWidget
{
public:
	CPainter(CHistory *history);
	virtual void paintEvent(QPaintEvent *event);
	
	virtual void end();
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
	
	void setLabelText();
	
	CHistory *history;
	QLabel *label;
	
	ct min, max;
	int arg;
};

class TmpWidget: public QWidget
{
Q_OBJECT 
public:
	TmpWidget();
	
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void paintEvent(QPaintEvent *event);

	int x;
};


#endif // CPAINTER_H
