#ifndef SPLINE_H
#define SPLINE_H

#include <QPainter>
#include <QPointF>

class Spline {
public:
  explicit Spline();
  size_t addPoint(QPointF &point);
  size_t addPoint(double x, double y);
  QPointF &at(size_t idx);
  void removeAt(size_t idx);
  size_t size();

private:
  class OrderedPointSet : public QList<QPointF> {
  public:
    size_t insert(QPointF &point);
  };

  OrderedPointSet points;
};

#endif // SPLINE_H
