#include <QMenuBar>
#include <QTabWidget>
#include <QInputDialog>

#include "mainwindow.h"
#include "tabwidget.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  initMenuBar();

  qTabWidget = new QTabWidget(this);

  ui->gridLayout->addWidget(qTabWidget);
}

void MainWindow::initMenuBar() {
  QMenuBar *menuBar = this->menuBar();

  QMenu *menuFile = new QMenu("File", this);
  menuFile->addAction("Create Project", this, SLOT(createProject()),
                      QKeySequence(Qt::CTRL + Qt::Key_N));
  menuFile->addAction("Open Project", this, SLOT(openProject()),
                      QKeySequence(Qt::CTRL + Qt::Key_O));
  menuFile->addAction("Save Project", this, SLOT(saveProject()),
                      QKeySequence(Qt::CTRL + Qt::Key_S));
  menuFile->addAction("Close Project", this, SLOT(closeProject()),
                      QKeySequence(Qt::CTRL + Qt::Key_W));

  QMenu *menuEdit = new QMenu("Edit", this);
  menuEdit->addAction("Redo", this, SLOT(undoCmd()),
                      QKeySequence(Qt::CTRL + Qt::Key_Z));
  menuEdit->addAction("Undo", this, SLOT(redoCmd()),
                      QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Z));

  QMenu *menuCurve = new QMenu("Curve", this);
  menuCurve->addAction("Add Curve", this, SLOT(addCurve()),
                       QKeySequence(Qt::CTRL + Qt::Key_A));
  menuCurve->addAction("Copy Curve", this, SLOT(copyCurve()),
                       QKeySequence(Qt::CTRL + Qt::Key_C));
  menuCurve->addAction("Paste Curve", this, SLOT(pasteCurve()),
                       QKeySequence(Qt::CTRL + Qt::Key_V));
  menuCurve->addAction("Remove Curve", this, SLOT(removeCurve()),
                       QKeySequence(Qt::Key_Backspace));

  QMenu *menuLetter = new QMenu("Letter", this);
  menuLetter->addAction("Add Letter", this, SLOT(addLetter()),
                        QKeySequence(Qt::CTRL + Qt::Key_L));

  QMenu *menuAbout = new QMenu("Help", this);
  menuAbout->addAction("About");

  menuBar->addMenu(menuFile);
  menuBar->addMenu(menuEdit);
  menuBar->addMenu(menuCurve);
  menuBar->addMenu(menuLetter);
  menuBar->addMenu(menuAbout);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::addCurve() {
  TabWidget *currentTab =
      reinterpret_cast<TabWidget *>(qTabWidget->currentWidget());
  if (currentTab) {
    currentTab->addCurve();
  }
}

void MainWindow::copyCurve() {
  TabWidget *currentTab =
      reinterpret_cast<TabWidget *>(qTabWidget->currentWidget());
  if (currentTab) {
    currentTab->copyCurve();
  }
}

void MainWindow::pasteCurve() {
  TabWidget *currentTab =
      reinterpret_cast<TabWidget *>(qTabWidget->currentWidget());
  if (currentTab) {
    currentTab->pasteCurve();
  }
}

void MainWindow::removeCurve() {
  TabWidget *currentTab =
      reinterpret_cast<TabWidget *>(qTabWidget->currentWidget());
  if (currentTab) {
    currentTab->removeCurve();
  }
}

void MainWindow::redoCmd() {
  TabWidget *currentTab =
      reinterpret_cast<TabWidget *>(qTabWidget->currentWidget());
  if (currentTab) {
    currentTab->redoCmd();
  }
}
void MainWindow::openProject() {
  TabWidget *newTab = new TabWidget(qTabWidget);
  QString projectName = newTab->openProject(ioController);
  qTabWidget->addTab(newTab, projectName);
  qTabWidget->setCurrentWidget(newTab);
}

void MainWindow::saveProject() {
  TabWidget *currentTab =
      reinterpret_cast<TabWidget *>(qTabWidget->currentWidget());
  if (currentTab) {
    currentTab->saveProject(ioController);
  }
}

void MainWindow::createProject() {
  TabWidget *newTab = new TabWidget(qTabWidget);
  bool isOK = true;
  while (isOK) {
    QString text = QInputDialog::getText(
        this, tr("Create Project"), tr("Choose project name:"),
        QLineEdit::Normal, "Untitled Project", &isOK);
    if (isOK) {
      if (!text.isEmpty()) {
        int i;
        for (i = 0; i < qTabWidget->count(); ++i) {
          if (text == qTabWidget->tabText(i)) {
            break;
          }
        }
        if (i == qTabWidget->count()) {
          qTabWidget->addTab(newTab, text);
          qTabWidget->setCurrentWidget(newTab);
          isOK = false;
        }
      }
    }
  }
}

void MainWindow::closeProject() {
  saveProject();
  qTabWidget->removeTab(qTabWidget->currentIndex());
}

void MainWindow::addLetter() {
  TabWidget *currentTab =
      reinterpret_cast<TabWidget *>(qTabWidget->currentWidget());
  if (currentTab) {
    bool isOK = true;
    while (isOK) {
      QString text =
          QInputDialog::getText(this, tr("Create Letter"), tr("Choose letter:"),
                                QLineEdit::Normal, "A", &isOK);
      if (isOK) {
        if (isOK && (text.size() > 0) && (text.size() < 2)) {
          currentTab->addLetter(QChar(text[0]));
          isOK = false;
        }
      }
    }
  }
}

void MainWindow::undoCmd() {
  TabWidget *currentTab =
      reinterpret_cast<TabWidget *>(qTabWidget->currentWidget());
  if (currentTab) {
    currentTab->undoCmd();
  }
}
