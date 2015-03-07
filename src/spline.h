#ifndef SPLINE_H
#define SPLINE_H

#include <QPoint>
#include <QPainter>

class Spline {
public:
  explicit Spline();
  void addPoint(const QPoint &point);
  void draw(QPainter *painter);

private:
  class OrderedPointSet : public QList<QPoint> {
  public:
    void insert(const QPoint &point);
  };

  OrderedPointSet points;
};

#endif // SPLINE_H
