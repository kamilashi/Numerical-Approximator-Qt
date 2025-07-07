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

struct Dimensions
{
	int mainWindowWidth = 800;
	int mainWindowHeight = 600;
	int topMenuSpacing = 10;
	int windowPartsSpacing = 0;
	int headerHeight = 50;
};

static const Dimensions dim;

MainWindow::MainWindow(Approximator* pAppr, QWidget* parent) : QMainWindow(parent)
{
	pApproximator = pAppr;

	resize(dim.mainWindowWidth, dim.mainWindowHeight);

	setWindowTitle("Numerical Approximator Qt");

	QWidget* central = new QWidget(this);
	setCentralWidget(central);

	QVBoxLayout* centralLayout = new QVBoxLayout();
	centralLayout->setSpacing(dim.windowPartsSpacing);
	central->setLayout(centralLayout);

	QWidget* header = new QWidget();
	QWidget* body = new QWidget();
	QWidget* footer = new QWidget();

	header->setMaximumHeight(dim.headerHeight);
	footer->setMaximumHeight(dim.headerHeight);

	centralLayout->addWidget(header);
	centralLayout->addWidget(body);
	centralLayout->addWidget(footer);

	QHBoxLayout* headerLayout = new QHBoxLayout();
	headerLayout->setAlignment(Qt::AlignVCenter);
	header->setLayout(headerLayout);

	pHeaderMenu = new QListWidget();

	pHeaderMenu->setFlow(QListView::LeftToRight);
	pHeaderMenu->setWrapping(true);
	pHeaderMenu->setResizeMode(QListView::Adjust);
	pHeaderMenu->setSpacing(dim.topMenuSpacing);

	for (int i = 0; i< pApproximator->programCount; i++)
	{
		AppMenuItem* menu = new AppMenuItem(pApproximator->programInterfaces[i].pName, pApproximator->programInterfaces[i].index);
		pHeaderMenu->addItem(menu);
	}

	headerLayout->addWidget(pHeaderMenu);

	QVBoxLayout* bodyLayout = new QVBoxLayout();
	bodyLayout->setAlignment(Qt::AlignTop);
	body->setLayout(bodyLayout);
	body->setStyleSheet("padding: 10px;");

	pOutput = new QLabel();

	QScrollArea* scrollArea = new QScrollArea();
	scrollArea->setAlignment(Qt::AlignTop);
	scrollArea->setWidget(pOutput);
	scrollArea->setWidgetResizable(true);

	bodyLayout->addWidget(scrollArea);

	QHBoxLayout* footerLayout = new QHBoxLayout();
	footerLayout->setAlignment(Qt::AlignVCenter);
	footer->setLayout(footerLayout);

	pInput = new QLineEdit();
	pInput->clear();

	footerLayout->addWidget(pInput);

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