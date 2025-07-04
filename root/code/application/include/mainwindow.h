#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Qlabel>
#include <QlineEdit>

#include "listwidget.h"
#include "approximator.h"

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(Approximator* pAppr, QWidget* parent = nullptr);
	~MainWindow();

private slots:
	void onItemClicked(QListWidgetItem* item);
	void onInputSubmitted();

private:
	QListWidget* pHeaderMenu = nullptr;
	QLineEdit* pInput = nullptr;
	QLabel* pOutput = nullptr;

	QString savedOutput;

	ProgramItem* pSelectedProgram = nullptr;
	Approximator* pApproximator = nullptr;

	InputType requestedInputType = InputType::TypesCount;

	void showCode();
	void runProgram();
};

#endif