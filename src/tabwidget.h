#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QHash>
#include <QListWidget>
#include <QString>
#include <QWidget>

#include "canvas.h"
#include "iocontroller.h"
#include "spline.h"

class TabWidget : public QWidget {
  Q_OBJECT
public:
  explicit TabWidget(QWidget *parent = 0);
  ~TabWidget();

private:
  Canvas *canvas;
  QHash<QString, SplineGroup> map;
  QListWidget *list;

public slots:
  void reloadCanvas(const QString &currentText);
  void addCurve();
  void copyCurve();
  void pasteCurve();
  void removeCurve();

  void addLetter(QChar name);

  QString openProject(IOController &ioController);
  void saveProject(IOController &ioController);

  void redoCmd();
  void undoCmd();
};

#endif // TABWIDGET_H
