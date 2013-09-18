#include "painter.h"
#include "manager.h"
#include <stdio.h>
#include <QLayout>
#include <QGraphicsScene>
#include "mainwindow.h"
#include "writer.h"
#include "nodes.h"

extern CManager manager;

CPainter::CPainter(CHistory *history)
{
	this->history = history;
	manager.painters.append(this);
	QVBoxLayout *layout = new QVBoxLayout();
	this->setLayout(layout);
	layout->addStretch(1);
	
	label = new QLabel();
	label->setMinimumHeight(40);
	label->setMaximumHeight(40);
//	label->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	layout->addWidget(label);
	this->setFocusPolicy(Qt::ClickFocus);
}

void CPainter::paintEvent(QPaintEvent *event)
{
	QPainter p(this);
	QBrush brush(QColor::fromRgb(255, 255, 255));
	int scaleFactor = 1;
	if(history->asScaled)
	{
		scaleFactor = this->width() / history->points.count();
		if(!scaleFactor)
			scaleFactor = 1;
	}
	
	ct rs = (this->height() - label->height())/(max - min);
	arg = ((TmpWidget*)this->parentWidget())->x;
	arg -= this->pos().x();
	if(arg >= 0)
	{
		arg /= scaleFactor;
	}
	if(history->points.size() > 0)
	{
		if(arg < 0)
			arg = 0;
		if(arg > (history->points.size()-1))
			arg = (history->points.size()-1);
	}
	
	p.setPen(QColor::fromRgb(0, 255, 0));
	
	if(max*min <= 0)
	{
		p.drawLine(0, max*rs, width(), max*rs);
	}
	
	p.setPen(QColor::fromRgb(0, 0, 255));
	brush.setColor(p.pen().color());
	if(history->asHisto)
	{
		for(size_t i = 0; i < history->points.size(); i++)
		{
			p.drawRect(i*scaleFactor, max*rs, scaleFactor, -history->points[i].imag()*rs);
			p.fillRect(i*scaleFactor, max*rs, scaleFactor, -history->points[i].imag()*rs, brush);
			//p.drawLine(i, (max)*rs, i, (max - history->points[i].imag())*rs);
		}
	}else
	{
		for(size_t i = 1; i < history->points.size(); i++)
		{
			p.drawLine((i-1)*scaleFactor, (max - history->points[i-1].imag())*rs, i*scaleFactor, (max - history->points[i].imag())*rs);
		}
	}
	p.setPen(QColor::fromRgb(255, 0, 0));
	brush.setColor(p.pen().color());
	if(history->asHisto)
	{
		for(size_t i = 0; i < history->points.size(); i++)
		{
			p.drawRect(i*scaleFactor, max*rs, scaleFactor, -history->points[i].real()*rs);
			p.fillRect(i*scaleFactor, max*rs, scaleFactor, -history->points[i].real()*rs, brush);
		//	p.drawLine(i, (max)*rs, i, (max - history->points[i].real())*rs);
		}
	}else
	{
		for(size_t i = 1; i < history->points.size(); i++)
		{
			p.drawLine((i-1)*scaleFactor, (max - history->points[i-1].real())*rs, i*scaleFactor, (max - history->points[i].real())*rs);
		}
	}
	
	p.setPen(QColor::fromRgb(0, 255, 0));
	p.drawLine(arg*scaleFactor, 0, arg*scaleFactor, height());
	
	setLabelText();
	p.end();
	event->ignore();
}

void CPainter::mousePressEvent(QMouseEvent *event)
{
	if((event->buttons() & Qt::RightButton))
	{
		QPixmap::grabWidget(this).save(history->str + QString(".png"));
		CWriter *writer = new CWriter(history, true);
		writer->end();
		delete writer;
	}
	event->ignore();
}


void CPainter::mouseMoveEvent(QMouseEvent *event)
{
	((TmpWidget*)parentWidget())->mousePressEvent(event);
}

void CPainter::keyPressEvent(QKeyEvent *event)
{
	qDebug()<<QString("Key pressed: '%1' '%2' '%3'").arg(event->key()).arg(event->modifiers()).arg(event->text());
}

void CPainter::end()
{
	history->getMinMax(min, max);
	this->setMinimumWidth(history->points.count());
	this->setMinimumHeight(200);
//	this->setMinimumSize(history->points.size(), 125);
//	this->setMaximumSize(this->minimumSize());
	this->repaint();
}


void CPainter::setLabelText()
{
	if(history->points.size() > 0)
	{
		label->setText(history->str + QString("  Max: (%1 ; %2)  Val: (%3 ; %4 + i%5)").arg(double(min)).arg(double(max)).arg(arg).arg(double(history->points[arg].real()), 0, 'g', 20).arg(double(history->points[arg].imag()), 0, 'g', 20));
	}else
	{
		label->setText(history->str + QString("  Max: (%1 ; %2)").arg(double(min)).arg(double(max)));
	}
	
	if(this->hasFocus())
	{
		label->setText(QString("_%1_").arg(label->text()));
	}
	
	label->adjustSize();
}

TmpWidget::TmpWidget(): QWidget()
{
	x = -1;
}

void TmpWidget::mousePressEvent(QMouseEvent *event)
{
	x = event->pos().x();
	update();
	if(event->buttons() & Qt::MidButton)
	{
		QPixmap pixmap(this->width() - 0, this->layout()->count() * 200);
		pixmap = QPixmap::grabWidget(this);
	/*	QPainter painter(&pixmap);
		for(int i = 0; i < this->layout()->count(); i++)
		{
			painter.drawPixmap(this->layout()->itemAt(i)->widget()->x(), this->layout()->itemAt(i)->widget()->y(), QPixmap::grabWidget(this->layout()->itemAt(i)->widget()));
		}
		painter.end();*/
		
		
		pixmap.save(QString("window.png"));
	}
	event->ignore();
}

void TmpWidget::mouseReleaseEvent(QMouseEvent *event)
{
	x = -1;
	update();
}

void TmpWidget::paintEvent(QPaintEvent *event)
{
	QPainter p(this);
	p.setPen(Qt::green);
	p.drawLine(x, 0, x, height());
	p.end();
	event->ignore();
}
