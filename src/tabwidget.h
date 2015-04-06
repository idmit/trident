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
  QString projectFileName;

public slots:
  void reloadCanvas(const QString &currentText);
  void addCurve();
  void copyCurve();
  void pasteCurve();
  void removeCurve();
  void setHWRatio(double r);
  void setHSRatio(double r);

  void addLetter(QChar name);
  void deleteLetter();

  QString openProject(IOController &ioController);
  QString saveProject(IOController &ioController);

  void redoCmd();
  void undoCmd();
};

#endif // TABWIDGET_H
