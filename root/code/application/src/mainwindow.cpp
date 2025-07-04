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

	QVBoxLayout* centralLayout = new QVBoxLayout;
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

	QHBoxLayout* headerLayout = new QHBoxLayout;
	headerLayout->setAlignment(Qt::AlignVCenter);
	header->setLayout(headerLayout);

	pHeaderMenu = new QListWidget();

	pHeaderMenu->setFlow(QListView::LeftToRight);
	pHeaderMenu->setWrapping(true);
	pHeaderMenu->setResizeMode(QListView::Adjust);
	pHeaderMenu->setSpacing(dim.topMenuSpacing);

	for (int i = 0; i< pApproximator->programCount; i++)
	{
		AppMenuItem* menu = new AppMenuItem(pApproximator->programItems[i].pName, pApproximator->programItems[i].index);
		pHeaderMenu->addItem(menu);
	}

	headerLayout->addWidget(pHeaderMenu);

	QVBoxLayout* bodyLayout = new QVBoxLayout;
	bodyLayout->setAlignment(Qt::AlignTop);
	body->setLayout(bodyLayout);

	pOutput = new QLabel("output");

	QScrollArea* scrollArea = new QScrollArea;
	scrollArea->setAlignment(Qt::AlignTop);
	scrollArea->setWidget(pOutput);
	scrollArea->setWidgetResizable(true);

	bodyLayout->addWidget(scrollArea);

	QHBoxLayout* footerLayout = new QHBoxLayout;
	footerLayout->setAlignment(Qt::AlignVCenter);
	footer->setLayout(footerLayout);

	pInput = new QLineEdit();

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

	pSelectedProgram = &pApproximator->programItems[pAppMenuItem->index];

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
	bool isInputValid = false;
	QString input;
	QString error;

	switch (requestedInputType)
	{
		case InputType::TypesCount:
		{
			error = "choose a program first!";
			break;
		}
		case InputType::Int:
		{
			int digit = pInput->text().toInt(&isInputValid);
			input = QString::number(digit);
			error = "please enter an integer number";
		} 
		break;
		case InputType::Float:
		{
			float digit = pInput->text().toFloat(&isInputValid);
			input = QString::number(digit);
			error = "please enter a floating point number";
		}
		break;
	}
	
	if (isInputValid)
	{
		pOutput->setText("You entered: " + input);
	}
	else
	{
		pOutput->setText("Invalid input - " + error);
	}
}

void MainWindow::showCode()
{
	qDebug() << "Show Code - requestedInput Int" << pSelectedProgram;
	requestedInputType = InputType::Int;
}

void MainWindow::runProgram()
{
	qDebug() << "Run Program - requestedInput Float" << pSelectedProgram;
	requestedInputType = InputType::Float;
}