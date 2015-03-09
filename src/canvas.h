#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>

#include "spline.h"

class Canvas : public QWidget {
  Q_OBJECT
public:
  explicit Canvas(QWidget *parent = 0);
  void setSpline(Spline *spline);

protected:
  void paintEvent(QPaintEvent *);
  void mouseDoubleClickEvent(QMouseEvent *);
  /*
   * void mousePressEvent(QMouseEvent *);
   * void mouseMoveEvent(QMouseEvent *);
   * void mouseReleaseEvent(QMouseEvent *);
   */

private:
  size_t radius = 10;
  Spline *spline = nullptr;

public slots:
  void undoCmd();
};

#endif // CANVAS_H
