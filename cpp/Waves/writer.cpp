#include "writer.h"
#include <QFile>
#include <QTextStream>
#include <QString>

CWriter::CWriter(CHistory *history, bool im): CHistoryWorker(history)
{
	this->str = this->history->str;
	this->im = im;
}
 
CWriter::~CWriter()
{

}

void CWriter::end()
{
	QFile file(str+QString(".html"));
	file.open(QIODevice::WriteOnly | QIODevice::Truncate);
	QTextStream stream(&file);
	
	stream<<"<html><body><table border=1>";
	
	for(int i = 0; i < history->points.size(); i++)
	{
		points.append(history->points[i]);
		stream<<"<tr>";
		stream<<"<td>"<<i<<"</td>"<<"<td>"<<QString("%1").arg((double)history->points[i].real(), 20, 'f', 20)<<"</td>";
		if(im)
			stream<<"<td>"<<QString("%1").arg((double)history->points[i].imag(), 20, 'f', 20)<<"</td>";
		stream<<"</tr>\n";
	}
	
	stream<<"</table></body></html>";
	
	file.close();
}
