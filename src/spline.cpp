#include "spline.h"

Spline::Spline() {}

void Spline::addPoint(const QPoint &point) { points.insert(point); }

void Spline::OrderedPointSet::insert(const QPoint &point) {
  size_t idx = 0;
  for (const auto &p : (*this)) {
    if (p.x() >= point.x()) {
      break;
    }
    ++idx;
  }
  QList<QPoint>::insert(idx, point);
}
