#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QHash>
#include <QListWidget>
#include <QString>
#include <QWidget>

#include "canvas.h"
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
  void redoCmd();
  void undoCmd();
};

#endif // TABWIDGET_H
