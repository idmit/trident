#include <QMenuBar>
#include <QTabWidget>

#include "mainwindow.h"
#include "tabwidget.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  initMenuBar();

  qTabWidget = new QTabWidget(this);
  qTabWidget->addTab(new TabWidget(qTabWidget), "First Project");
  qTabWidget->addTab(new TabWidget(qTabWidget), "Second Project");
  qTabWidget->addTab(new TabWidget(qTabWidget), "Third Project");

  ui->gridLayout->addWidget(qTabWidget);
}

void MainWindow::initMenuBar() {
  QMenuBar *menuBar = this->menuBar();

  QMenu *menuFile = new QMenu("File", this);
  menuFile->addAction("Open...");
  menuFile->addAction("Save...");
  menuFile->addAction("Exit");

  QMenu *menuEdit = new QMenu("Edit", this);
  menuEdit->addAction("Add Curve", this, SLOT(addCurve()),
                      QKeySequence(Qt::CTRL + Qt::Key_A));
  menuEdit->addAction("Undo", this, SLOT(undoCmd()),
                      QKeySequence(Qt::CTRL + Qt::Key_Z));
  menuEdit->addAction("Copy");

  QMenu *menuAbout = new QMenu("About", this);

  menuBar->addMenu(menuFile);
  menuBar->addMenu(menuEdit);
  menuBar->addMenu(menuAbout);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::addCurve() {
  TabWidget *currentTab =
      reinterpret_cast<TabWidget *>(qTabWidget->currentWidget());
  currentTab->addCurve();
}

void MainWindow::undoCmd() {
  TabWidget *currentTab =
      reinterpret_cast<TabWidget *>(qTabWidget->currentWidget());
  currentTab->undoCmd();
}
