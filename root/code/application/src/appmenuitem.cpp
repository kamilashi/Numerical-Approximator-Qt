#include "appmenuitem.h"

AppMenuItem::AppMenuItem(const QString& text, int idx, QListWidget* listview) : QListWidgetItem(text, listview)
{
	index = idx;
}

AppMenuItem::~AppMenuItem() {}