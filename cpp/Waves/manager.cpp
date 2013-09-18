#include "manager.h"
#include <stdio.h>

CManager manager;

CManager::~CManager()
{
	clear();
}

void CManager::clear()
{
	foreach(CHistory *history, workers)
		delete history;
}

void CManager::end()
{
	preend();
	postend();
}

void CManager::preend()
{
	foreach(CHistory *history, workers)
		history->end();
}

void CManager::postend()
{
	foreach(CPainter *history, painters)
		history->end();
}
