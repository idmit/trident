#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>

#include "splinegroup.h"

class Canvas : public QWidget {
  Q_OBJECT
public:
  explicit Canvas(QWidget *parent = 0);
  SplineGroup *getActiveGroup();
  void setActiveSplineIdx(size_t idx);
  void setActiveGroup(SplineGroup *group);

protected:
  void paintEvent(QPaintEvent *);
  void mouseDoubleClickEvent(QMouseEvent *);
  void mousePressEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

private:
  bool moving = false;
  size_t pressedPointIdx = 0;
  QPointF diffToPress, originPos;
  bool movingCurve = false;
  Spline originSpline;

  size_t radius = 10;
  SplineGroup *activeGroup = nullptr;

  Spline copiedSpline;

  void drawGrid(QPainter &painter, size_t cellNum = 20,
                size_t cellsInThick = 2);
  void drawSplines(QPainter &painter);
  size_t newPointIdx(QPointF &point);
  char movedCurveFits(QPointF &point, QList<QPointF> *newPositions);

public slots:
  void addCurve(Spline spline);
  void copyCurve();
  void pasteCurve();
  void removeCurve();
  void redoCmd();
  void undoCmd();
};

#endif // CANVAS_H
