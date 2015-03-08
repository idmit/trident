#include "spline.h"

Spline::Spline() {}

void Spline::addPoint(Point &point) { points.insert(point); }

void Spline::addPoint(double x, double y) {
  Point p(x, y);
  points.insert(p);
}

Point &Spline::at(size_t idx) { return points[idx]; }

void Spline::OrderedPointSet::insert(Point &point) {
  size_t idx = 0;
  for (const auto &p : (*this)) {
    if (p.x >= point.x) {
      break;
    }
    ++idx;
  }
  QList<Point>::insert(idx, point);
}

size_t Spline::size() { return points.size(); }
