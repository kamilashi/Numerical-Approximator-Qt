#ifndef APPMENUITEM_H
#define APPMENUITEM_H

#include <QListWidgetItem>

class AppMenuItem : public QListWidgetItem {

public:
	AppMenuItem(const QString& text, int idx, QListWidget* listview = nullptr);
	~AppMenuItem();

private:
	int index;
};

#endif