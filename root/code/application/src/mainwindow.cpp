#include "mainwindow.h"
#include "listwidget.h"
#include "appmenuitem.h"

#include <stdio.h>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QWidget>
#include <QMenu>
#include <QScrollArea>
#include <QApplication>

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

	//pHeader->setStyleSheet("background-color: red;");
	//pFooter->setStyleSheet("background-color: green;");

	pCentralLayout->addWidget(pHeader);
	pCentralLayout->addWidget(pBody);
	pCentralLayout->addWidget(pFooter);

	QHBoxLayout* pHeaderLayout = new QHBoxLayout();
	pHeaderLayout->setAlignment(Qt::AlignVCenter);
	pHeader->setLayout(pHeaderLayout);

	pHeaderMenu = new QListWidget();
	//pHeaderMenu->setStyleSheet("background-color: blue;");

	pHeaderMenu->setFlow(QListView::LeftToRight);
	pHeaderMenu->setWrapping(false);
	pHeaderMenu->setSpacing(dim.topMenuHorizontalSpacing); 
	pHeaderMenu->setItemAlignment(Qt::AlignVCenter);

	for (int i = 0; i< pApproximator->programCount; i++)
	{
		AppMenuItem* pMenu = new AppMenuItem(pApproximator->programInterfaces[i].pName, pApproximator->programInterfaces[i].index);
		pMenu->setTextAlignment(Qt::AlignVCenter);
		pHeaderMenu->addItem(pMenu);
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

	// Connect signal to slot
	connect(pHeaderMenu, &QListWidget::itemClicked, this, &MainWindow::onItemClicked);

	connect(pInput, &QLineEdit::returnPressed, this, &MainWindow::onInputSubmitted);

}

MainWindow::~MainWindow() {}

void MainWindow::onItemClicked(QListWidgetItem* item) 
{
	QString itemText = item->text();

	AppMenuItem* pAppMenuItem = static_cast<AppMenuItem*>(item);

	if (pSelectedProgram != nullptr)
	{
		pApproximator->resetProgram(pSelectedProgram->index);
	}
	pSelectedProgram = &pApproximator->programInterfaces[pAppMenuItem->index];

	// Create menu
	QMenu* menu = new QMenu(pHeaderMenu);
	QAction* actionA = menu->addAction("Show Code");
	QAction* actionB = menu->addAction("Run Program");

	connect(actionA, &QAction::triggered, this, &MainWindow::showCode);
	connect(actionB, &QAction::triggered, this, &MainWindow::runProgram);

	QRect itemRect = pHeaderMenu->visualItemRect(item);
	QPoint globalPos = pHeaderMenu->viewport()->mapToGlobal(itemRect.bottomLeft());
	menu->exec(globalPos);
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

void MainWindow::showCode()
{
	pOutput->setText("Here Should Be Code");
}

void MainWindow::runProgram()
{
	startSelectedProgram(&programOutput);
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