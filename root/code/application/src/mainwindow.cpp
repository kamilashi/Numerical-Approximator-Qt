#include "mainwindow.h"
#include "listwidget.h"
#include "appmenuitem.h"

#include <stdio.h>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QWidget>
#include <QMenu>

struct Dimensions
{
	int mainWindowWidth = 800;
	int mainWindowHeight = 600;
	int topMenuSpacing = 10;
	int headerHeight = 50;
};

static const Dimensions dim;

// should be back end
static const int programCount = 5;
ProgramItem programItems[programCount] = { ProgramItem("Determinant"),
							ProgramItem("Other"),
							ProgramItem("Other"),
							ProgramItem("Other"),
							ProgramItem("Other") };

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	resize(dim.mainWindowWidth, dim.mainWindowHeight);
	setWindowTitle("Numerical Approximator Qt");

	// Central widget
	QWidget* central = new QWidget(this);
	setCentralWidget(central);

	QVBoxLayout* centralLayout = new QVBoxLayout;
	central->setLayout(centralLayout);

	QWidget* header = new QWidget();
	QWidget* body = new QWidget();
	QWidget* footer = new QWidget();

	header->setMaximumHeight(dim.headerHeight);
	footer->setMaximumHeight(dim.headerHeight);

	centralLayout->addWidget(header);
	centralLayout->addWidget(body);
	centralLayout->addWidget(footer);

	QHBoxLayout* horizontalLayout = new QHBoxLayout;
	horizontalLayout->setAlignment(Qt::AlignVCenter);
	header->setLayout(horizontalLayout);

	pHeaderMenu = new QListWidget();

	pHeaderMenu->setFlow(QListView::LeftToRight);
	pHeaderMenu->setWrapping(true);
	pHeaderMenu->setResizeMode(QListView::Adjust);
	pHeaderMenu->setSpacing(dim.topMenuSpacing);

	for (int i = 0; i< programCount; i++)
	{
		AppMenuItem* menu = new AppMenuItem(programItems[i].pName, programItems[i].index);
		pHeaderMenu->addItem(menu);
	}

	horizontalLayout->addWidget(pHeaderMenu);

	// Connect signal to slot
	connect(pHeaderMenu, &QListWidget::itemClicked, this, &MainWindow::onItemClicked);

}

MainWindow::~MainWindow() {}

void MainWindow::onItemClicked(QListWidgetItem* item) 
{
	QString itemText = item->text();

	AppMenuItem* pAppMenuItem = static_cast<AppMenuItem*>(item);

	pSelectedMenuEntry = &programItems[pAppMenuItem->index]; 

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

void MainWindow::showCode()
{
	qDebug() << "Show Code" << pSelectedMenuEntry;
	// Add logic depending on selectedItemText
}

void MainWindow::runProgram()
{
	qDebug() << "Run Program" << pSelectedMenuEntry;
	// Add logic depending on selectedItemText
}