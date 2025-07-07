#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Qlabel>
#include <QlineEdit>

#include "approximator.h"
#include "appmenuitem.h"

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(Approximator* pAppr, QWidget* parent = nullptr);
	~MainWindow();

private slots:
	void onInputSubmitted();

private:
	QMenuBar* pHeaderMenu = nullptr;
	QLineEdit* pInput = nullptr;
	QLabel* pOutput = nullptr;

	QString savedOutput;

	ProgramInterface* pSelectedProgram = nullptr;
	Approximator* pApproximator = nullptr;

	ProgramOutput programOutput;

	void createMenuActions(QMenu* item, int connectedProgramIndex);
	void cacheSelectedProgram(QAction* triggeredAction);
	void startSelectedProgram(ProgramOutput* pProgramOutput);

	void showCodeAct();
	void runProgram();
	void advanceSelectedProgram(ProgramOutput* pProgramOutput, const ProgramInput& input);
};

#endif