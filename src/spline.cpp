#include "spline.h"

Spline::Spline(QObject *parent) {
  if (parent != nullptr) {
    undoStack = new QUndoStack(parent);
  }
}

size_t Spline::addPoint(QPointF &point) { return points.insert(point); }

size_t Spline::addPoint(double x, double y) {
  QPointF p(x, y);
  return points.insert(p);
}

QPointF &Spline::at(size_t idx) { return points[idx]; }

size_t Spline::OrderedPointSet::insert(QPointF &point) {
  size_t idx = 0;
  for (const auto &p : (*this)) {
    if (p.x() >= point.x()) {
      break;
    }
    ++idx;
  }
  QList<QPointF>::insert(idx, point);
  return idx;
}

void Spline::removeAt(size_t idx) { points.removeAt(idx); }

size_t Spline::size() { return points.size(); }
