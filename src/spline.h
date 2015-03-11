#ifndef SPLINE_H
#define SPLINE_H

#include <QPainter>
#include <QPointF>
#include <QUndoStack>

class Spline {
public:
  explicit Spline(QObject *parent = nullptr);

  size_t addPoint(QPointF &point, bool rebuild = true);
  size_t addPoint(double x, double y);
  QPointF atSup(size_t idx);
  void build(size_t approxParam);
  QPointF &atVal(size_t idx);
  void removeAt(size_t idx, bool rebuild = true);
  void resetPoint(size_t idx, QPointF &point);
  void resetPointX(size_t idx, double x);
  void resetPointY(size_t idx, double y);
  size_t supSize();
  size_t valSize();

  QUndoStack *undoStack = nullptr;

private:
  class OrderedPointSet : public QList<QPointF> {
  public:
    size_t insert(QPointF point);
  };

  static constexpr size_t approx = 100;
  OrderedPointSet support;
  OrderedPointSet values;

  QPointF buildBasicSpline(QPointF &p0, QPointF &p1, QPointF &p2, QPointF &p3,
                           double t);
};

#endif // SPLINE_H
