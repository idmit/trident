#ifndef SPLINE_H
#define SPLINE_H

#include <QPainter>
#include <QPointF>
#include <QUndoStack>

class Spline {
public:
  explicit Spline(QObject *parent = nullptr);

  size_t addPoint(QPointF &point);
  size_t addPoint(double x, double y);
  QPointF &at(size_t idx);
  void build(size_t approxParam);
  QPointF &extAt(size_t idx);
  size_t extSize();
  void removeAt(size_t idx);
  size_t size();

  QUndoStack *undoStack = nullptr;

private:
  class OrderedPointSet : public QList<QPointF> {
  public:
    size_t insert(QPointF point);
  };

  static constexpr size_t approx = 100;
  OrderedPointSet extSupport;
  OrderedPointSet support;

  QPointF buildBasicSpline(QPointF &p0, QPointF &p1, QPointF &p2, QPointF &p3,
                           double t);
};

#endif // SPLINE_H
