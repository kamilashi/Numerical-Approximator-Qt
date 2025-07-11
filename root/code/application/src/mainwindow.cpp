#include "mainwindow.h"
#include "appmenuitem.h"

#include <QMessageBox>
#include <QVBoxLayout>
#include <QWidget>
#include <QScrollArea>
#include <QScrollBar>
#include <QApplication>
#include <QMenuBar>

struct Parameters
{
	int mainWindowWidth = 800;
	int mainWindowHeight = 600;
	int topMenuHorizontalSpacing = 0;
	int containersSpacing = 0;
	int headerHeight = 50;
	QString containerSS = "padding: 0px;";
	QString borderedChildSS = "#Bordered {border-radius: 10px; border: 1px solid palette(button); background-color: palette(base);}";
	QString paddedChildSS = "#Padded {padding: 10px; border: none; }";
	QString menuBarSS = "#MenuBar {padding: 0px; background-color: palette(dark);}";
};

static const Parameters params;

MainWindow::MainWindow(Approximator* pAppr, QWidget* parent) : QMainWindow(parent)
{
	pApproximator = pAppr;
	programOutput = ProgramOutput();
	pSelectedProgram = nullptr;
	savedOutput = "";
	QString welcomeText = QString("Choose a program to inspect or run from the menu bar above. \n\nUse the edit line at the bottom to send input to the program.");

	resize(params.mainWindowWidth, params.mainWindowHeight);

	setWindowTitle("Numerical Approximator");

	QPalette palette = QApplication::palette();
	QColor defaultColor = palette.color(QPalette::Base);

	QWidget* pCentralWidget = new QWidget(this);
	setCentralWidget(pCentralWidget);

	QVBoxLayout* pCentralLayout = new QVBoxLayout();
	pCentralLayout->setSpacing(params.containersSpacing);
	pCentralWidget->setLayout(pCentralLayout);

	QWidget* pHeader = new QWidget();
	QWidget* pBody = new QWidget();
	QWidget* pFooter = new QWidget();

	pHeader->setFixedHeight(params.headerHeight);
	pFooter->setFixedHeight(params.headerHeight);

	pCentralLayout->addWidget(pHeader);
	pCentralLayout->addWidget(pBody);
	pCentralLayout->addWidget(pFooter);

	QHBoxLayout* pHeaderLayout = new QHBoxLayout();
	pHeaderLayout->setAlignment(Qt::AlignVCenter);
	pHeader->setLayout(pHeaderLayout);
	pHeader->setStyleSheet(params.containerSS);

	pHeaderMenu = new QMenuBar();
	pHeaderMenu->setObjectName("MenuBar");

	for (int i = 0; i< pApproximator->programCount; i++)
	{
		QMenu* pMenu = new QMenu(QString(pApproximator->programInterfaces[i].pName));
		createMenuActions(pMenu, pApproximator->programInterfaces[i].index);

		if (pApproximator->programInterfaces[i].pCategoryName == nullptr) 
		{
			pHeaderMenu->addMenu(pMenu);
			pHeaderMenu->setStyleSheet(params.menuBarSS);
		}
		else 
		{
			createOrUpdateCategory(pMenu, pApproximator->programInterfaces[i].pCategoryName);
		}
	}

	pHeaderLayout->addWidget(pHeaderMenu);

	QVBoxLayout* pBodyLayout = new QVBoxLayout();
	pBodyLayout->setAlignment(Qt::AlignTop);
	pBody->setLayout(pBodyLayout);
	pBody->setStyleSheet(params.containerSS);

	pOutput = new QLabel(welcomeText);
	pOutput->setAlignment(Qt::AlignTop);
	pOutput->setObjectName("Padded");
	pOutput->setStyleSheet(params.paddedChildSS);

	QScrollArea* pScrollArea = new QScrollArea();
	pScrollArea->setObjectName("Bordered");
	pScrollArea->setAlignment(Qt::AlignTop);
	pScrollArea->setWidget(pOutput);
	pScrollArea->setWidgetResizable(true);
	pScrollArea->setStyleSheet(params.borderedChildSS);

	pScrollArea->verticalScrollBar()->setStyleSheet(params.borderedChildSS);

	pBodyLayout->addWidget(pScrollArea);

	QHBoxLayout* pFooterLayout = new QHBoxLayout();
	pFooterLayout->setAlignment(Qt::AlignVCenter);
	pFooter->setLayout(pFooterLayout);
	pFooter->setStyleSheet(params.containerSS);

	pInput = new QLineEdit();
	pInput->setAlignment(Qt::AlignTop);
	pInput->setObjectName("Bordered");
	pInput->setStyleSheet(params.borderedChildSS);
	pInput->clear();

	pFooterLayout->addWidget(pInput);

	connect(pInput, &QLineEdit::returnPressed, this, &MainWindow::onInputSubmitted);
}

MainWindow::~MainWindow() 
{
}

void MainWindow::onInputSubmitted()
{
	ProgramInput input;
	bool isInputValid = false;
	QString error;

	switch (programOutput.requestedInputType)
	{
	case InputType::TypesCount:
	{
		error = "Choose a program first!\n";
		break;
	}
	case InputType::Int:
	{
		int digit = pInput->text().toInt(&isInputValid);
		input.inputInt = digit;
		error = "Please enter an integer number\n";
	}
	break;
	case InputType::Float:
	{
		float digit = pInput->text().toFloat(&isInputValid);
		input.inputFloat = digit;
		error = "Please enter a floating point number\n";
	}
	break;
	case InputType::Any:
	{
		isInputValid = true;
	}
	break;
	}

	if (isInputValid)
	{
		advanceSelectedProgram(&programOutput, input);
	}
	else
	{
		pOutput->setText(savedOutput + "\n\n" + "Invalid input - " + error);
	}

	pInput->clear();
}

void MainWindow::createMenuActions(QMenu* pMenu, int connectedProgramIndex)
{
	AppMenuItem* showCodeAct = new AppMenuItem(connectedProgramIndex, "Show Code");
	AppMenuItem* runProgramAct = new AppMenuItem(connectedProgramIndex, "Run Program");
	pMenu->addAction(showCodeAct);
	pMenu->addAction(runProgramAct);

	connect(showCodeAct, &QAction::triggered, this, &MainWindow::showProgramCode);
	connect(runProgramAct, &QAction::triggered, this, &MainWindow::runProgram);
}

void MainWindow::createOrUpdateCategory(QMenu* pMenu, const char* pCategoryName)
{
	QString categoryName = QString(pCategoryName);

	if (!categoryMenus.contains(categoryName))
	{
		QMenu* categoryMenu = new QMenu(categoryName);
		pHeaderMenu->addMenu(categoryMenu);
		categoryMenus.insert(categoryName, categoryMenu);
	}

	categoryMenus[categoryName]->addMenu(pMenu);
}

void MainWindow::cacheSelectedProgram(QAction* triggeredAction)
{
	AppMenuItem* pAppMenuItem = static_cast<AppMenuItem*>(triggeredAction);
	if (pSelectedProgram != nullptr)
	{
		pApproximator->resetProgram(pSelectedProgram->index);
	}
	pSelectedProgram = &pApproximator->programInterfaces[pAppMenuItem->index];
}

void MainWindow::startSelectedProgram(ProgramOutput* pProgramOutput)
{
	pOutput->setText("");
	savedOutput = "";

	pApproximator->startProgram(pSelectedProgram->index, pProgramOutput);

	QString outputString = savedOutput.length() == 0 ? pProgramOutput->pOutput : savedOutput + "\n" + pProgramOutput->pOutput;

	pOutput->setText(outputString);

	if (!pProgramOutput->outputIsError)
	{
		savedOutput = outputString;
	}
}

void MainWindow::showProgramCode()
{
	pOutput->setText("Here will be program code.");

	AppMenuItem* pAction = qobject_cast<AppMenuItem*>(sender());
	if (pAction)
	{
		cacheSelectedProgram(pAction);

		pApproximator->getProgramCode(pSelectedProgram->index, &programOutput);

		pOutput->setText(programOutput.pOutput);
	}
}

void MainWindow::runProgram()
{
	AppMenuItem* pAction = qobject_cast<AppMenuItem*>(sender());
	if (pAction)
	{
		cacheSelectedProgram(pAction);
		startSelectedProgram(&programOutput);
	}
}

void MainWindow::advanceSelectedProgram(ProgramOutput* pProgramOutput, const ProgramInput& input)
{
	pApproximator->advanceProgram(pSelectedProgram->index, pProgramOutput, input);

	pOutput->setText(savedOutput + "\n" + pProgramOutput->pOutput);

	if (!pProgramOutput->outputIsError)
	{
		savedOutput = savedOutput + "\n" + pProgramOutput->pOutput;
	}
}
