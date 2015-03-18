#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "canvas.h"
#include "iocontroller.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private:
  void initMenuBar();

  Ui::MainWindow *ui;
  IOController ioController;
  QTabWidget *qTabWidget;

public slots:
  void addCurve();
  void copyCurve();
  void pasteCurve();
  void removeCurve();
  void redoCmd();
  void openProject();
  void saveProject();
  void undoCmd();
};

#endif // MAINWINDOW_H
