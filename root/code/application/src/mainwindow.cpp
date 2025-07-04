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

MainWindow::MainWindow(Approximator* pAppr, QWidget* parent) : QMainWindow(parent)
{
	pApproximator = pAppr;

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

	for (int i = 0; i< pApproximator->programCount; i++)
	{
		AppMenuItem* menu = new AppMenuItem(pApproximator->programItems[i].pName, pApproximator->programItems[i].index);
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

void MainWindow::showCode()
{
	qDebug() << "Show Code" << pSelectedProgram;
	// Add logic depending on selectedItemText
}

void MainWindow::runProgram()
{
	qDebug() << "Run Program" << pSelectedProgram;
	// Add logic depending on selectedItemText
}