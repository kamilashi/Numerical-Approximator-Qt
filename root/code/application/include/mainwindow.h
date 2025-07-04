#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "listwidget.h"
#include "approximator.h"

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(Approximator* pAppr, QWidget* parent = nullptr);
	~MainWindow();

private slots:
	void onItemClicked(QListWidgetItem* item);

private:
	QListWidget* pHeaderMenu = nullptr;
	ProgramItem* pSelectedProgram = nullptr;
	Approximator* pApproximator = nullptr;

	void showCode();
	void runProgram();
};

#endif