#include "mainwindow.h"
#include "listwidget.h"
#include "appmenuitem.h"

#include <stdio.h>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QWidget>

struct Dimensions
{
	int mainWindowWidth = 800;
	int mainWindowHeight = 600;
	int topMenuSpacing = 10;
	int headerHeight = 50;
};

static const Dimensions dim;

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

	headerMenu = new QListWidget();

	headerMenu->setFlow(QListView::LeftToRight);
	headerMenu->setWrapping(true);
	headerMenu->setResizeMode(QListView::Adjust);
	headerMenu->setSpacing(dim.topMenuSpacing);

	for (int i = 0; i<5;i++)
	{
		AppMenuItem* menu = new AppMenuItem("Menu", 0);
		headerMenu->addItem(menu);
	}

	horizontalLayout->addWidget(headerMenu);

	// Connect signal to slot
	connect(headerMenu, &QListWidget::itemClicked, this, &MainWindow::onItemClicked);

}

MainWindow::~MainWindow() {}

void MainWindow::onItemClicked(QListWidgetItem* item) {
	QMessageBox::information(this, "Item clicked", "You clicked: " + item->text());
}