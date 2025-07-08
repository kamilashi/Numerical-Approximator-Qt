#include "mainwindow.h"
#include "appmenuitem.h"

#include <QMessageBox>
#include <QVBoxLayout>
#include <QWidget>
#include <QScrollArea>
#include <QApplication>
#include <QMenuBar>

struct Dimensions
{
	int mainWindowWidth = 800;
	int mainWindowHeight = 600;
	int topMenuHorizontalSpacing = 0;
	int windowPartsSpacing = 0;
	int headerHeight = 50;
	QString bodySS = "padding: 10px;";
	QString headerMenuSS = "left-padding: 10px; right-padding: 10px;";
};

static const Dimensions dim;

MainWindow::MainWindow(Approximator* pAppr, QWidget* parent) : QMainWindow(parent)
{
	pApproximator = pAppr;
	programOutput = ProgramOutput();
	pSelectedProgram = nullptr;
	savedOutput = "";

	resize(dim.mainWindowWidth, dim.mainWindowHeight);

	setWindowTitle("Numerical Approximator Qt");

	QPalette palette = QApplication::palette();
	QColor defaultColor = palette.color(QPalette::Base);

	QWidget* pCentralWidget = new QWidget(this);
	setCentralWidget(pCentralWidget);

	QVBoxLayout* pCentralLayout = new QVBoxLayout();
	pCentralLayout->setSpacing(dim.windowPartsSpacing);
	pCentralWidget->setLayout(pCentralLayout);

	QWidget* pHeader = new QWidget();
	QWidget* pBody = new QWidget();
	QWidget* pFooter = new QWidget();

	pHeader->setFixedHeight(dim.headerHeight);
	pFooter->setFixedHeight(dim.headerHeight);

	pCentralLayout->addWidget(pHeader);
	pCentralLayout->addWidget(pBody);
	pCentralLayout->addWidget(pFooter);

	QHBoxLayout* pHeaderLayout = new QHBoxLayout();
	pHeaderLayout->setAlignment(Qt::AlignVCenter);
	pHeader->setLayout(pHeaderLayout);

	pHeaderMenu = new QMenuBar();

	for (int i = 0; i< pApproximator->programCount; i++)
	{
		QMenu* pMenu = new QMenu(QString(pApproximator->programInterfaces[i].pName));
		createMenuActions(pMenu, pApproximator->programInterfaces[i].index);
		pHeaderMenu->addMenu(pMenu);
	}

	pHeaderLayout->addWidget(pHeaderMenu);

	QVBoxLayout* pBodyLayout = new QVBoxLayout();
	pBodyLayout->setAlignment(Qt::AlignTop);
	pBody->setLayout(pBodyLayout);
	pBody->setStyleSheet(dim.bodySS);

	pOutput = new QLabel();
	pOutput->setStyleSheet(QString("background-color: %1;").arg(defaultColor.name()));
	pOutput->setAlignment(Qt::AlignTop);

	QScrollArea* pScrollArea = new QScrollArea();
	pScrollArea->setAlignment(Qt::AlignTop);
	pScrollArea->setWidget(pOutput);
	pScrollArea->setWidgetResizable(true);

	pBodyLayout->addWidget(pScrollArea);

	QHBoxLayout* pFooterLayout = new QHBoxLayout();
	pFooterLayout->setAlignment(Qt::AlignVCenter);
	pFooter->setLayout(pFooterLayout);

	pInput = new QLineEdit();
	pInput->setAlignment(Qt::AlignTop);
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

	connect(showCodeAct, &QAction::triggered, this, &MainWindow::showCodeAct);
	connect(runProgramAct, &QAction::triggered, this, &MainWindow::runProgram);
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

	pOutput->setText(savedOutput + "\n" + pProgramOutput->pOutput);

	if (!pProgramOutput->outputIsError)
	{
		savedOutput = savedOutput + "\n" + pProgramOutput->pOutput;
	}
}

void MainWindow::showCodeAct()
{
	AppMenuItem* pAction = qobject_cast<AppMenuItem*>(sender());
	if (pAction)
	{
		cacheSelectedProgram(pAction);
		pOutput->setText("Here Should Be Code");
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
