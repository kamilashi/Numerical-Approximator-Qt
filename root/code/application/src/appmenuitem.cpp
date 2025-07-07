#include "appmenuitem.h"

AppMenuItem::AppMenuItem(int idx, const QString& title, QObject* parent) : QAction(title, parent)
{
	index = idx;
}

AppMenuItem::~AppMenuItem() {}