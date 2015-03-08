#ifndef SPLINE_H
#define SPLINE_H

#include <QPainter>

class Point {
public:
  Point(double x, double y) : x(x), y(y) {}

public:
  double x;
  double y;
};

class Spline {
public:
  explicit Spline();
  void addPoint(Point &point);
  void addPoint(double x, double y);
  Point &at(size_t idx);
  void draw(QPainter *painter);
  size_t size();

private:
  class OrderedPointSet : public QList<Point> {
  public:
    void insert(Point &point);
  };

  OrderedPointSet points;
};

#endif // SPLINE_H
