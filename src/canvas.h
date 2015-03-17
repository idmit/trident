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
  void mousePressEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

private:
  bool chosen = false;
  size_t chosenIdx;
  size_t radius = 10;
  Spline *spline = nullptr;
  QPointF diffToPress, originPos;
  void drawGrid(QPainter &painter, size_t cellNum = 20,
                size_t cellsInThick = 2);

public slots:
  void undoCmd();
};

#endif // CANVAS_H
