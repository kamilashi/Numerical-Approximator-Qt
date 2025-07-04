#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "listwidget.h"

struct ProgramItem
{
	int index;
	const char* pName;

	ProgramItem(const char* pNameString)
	{
		static int count = 0;

		index = count;
		pName = pNameString;

		count++;
	}

	~ProgramItem() {}
};


class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private slots:
	void onItemClicked(QListWidgetItem* item);

private:
	QListWidget* pHeaderMenu;
	ProgramItem* pSelectedMenuEntry;

	void showCode();
	void runProgram();
};

#endif