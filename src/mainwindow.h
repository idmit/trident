#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "canvas.h"

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
  QTabWidget *qTabWidget;

public slots:
  void addCurve();
  void undoCmd();
};

#endif // MAINWINDOW_H
