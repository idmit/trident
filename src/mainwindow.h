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
  void createProject();
  void closeProject();
  void openProject();
  void saveProject();
  void saveAll();
  void addLetter();
  void deleteLetter();
  void renameLetter();
  void redoCmd();
  void rhwCmd();
  void rhsCmd();
  void undoCmd();
  void about();
};

#endif // MAINWINDOW_H
