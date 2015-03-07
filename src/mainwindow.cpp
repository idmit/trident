#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), canvas(this) {
  ui->setupUi(this);
  ui->editorLayout->addWidget(&canvas);

  initMenuBar();
}

void MainWindow::initMenuBar() {
  QMenuBar *menuBar = this->menuBar();

  QMenu *menuFile = new QMenu("File", this);
  menuFile->addAction("Open...");
  menuFile->addAction("Save...");
  menuFile->addAction("Exit");

  QMenu *menuEdit = new QMenu("Edit", this);
  menuEdit->addAction("Undo");
  menuEdit->addAction("Copy");

  QMenu *menuAbout = new QMenu("About", this);

  menuBar->addMenu(menuFile);
  menuBar->addMenu(menuEdit);
  menuBar->addMenu(menuAbout);
}

MainWindow::~MainWindow() {
  delete ui;
}
