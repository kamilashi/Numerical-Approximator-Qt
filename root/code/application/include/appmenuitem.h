#ifndef APPMENUITEM_H
#define APPMENUITEM_H

#include <QAction>

class AppMenuItem : public QAction {
	Q_OBJECT
public:
	AppMenuItem(int idx, const QString& title, QObject* parent = nullptr);
	~AppMenuItem();

	int index;
};

#endif