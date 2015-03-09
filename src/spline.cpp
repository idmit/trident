#include "spline.h"

Spline::Spline(QObject *parent) {
  if (parent != nullptr) {
    undoStack = new QUndoStack(parent);
  }
}

size_t Spline::addPoint(QPointF &point) {
  size_t idx = support.insert(point);
  build(approx);
  return idx;
}

size_t Spline::addPoint(double x, double y) {
  QPointF p(x, y);
  return support.insert(p);
}

QPointF &Spline::at(size_t idx) { return support[idx]; }

void Spline::build(size_t approxParam) {
  double step = 1.0 / approxParam, arg = 0;
  size_t supSize = support.size();
  OrderedPointSet extendedSupport;

  if (supSize > 3) {
    for (size_t i = 1; i < supSize - 2; ++i) {
      arg = 0;
      for (size_t num = 0; num <= approxParam; ++num) {
        extendedSupport.insert(buildBasicSpline(
            support[i - 1], support[i], support[i + 1], support[i + 2], arg));
        arg += step;
      }
    }
    extSupport = extendedSupport;
  } else {
    extSupport = support;
  }
}

QPointF Spline::buildBasicSpline(QPointF &p0, QPointF &p1, QPointF &p2,
                                 QPointF &p3, double t) {
  double s = 1 - t;
  double t2 = t * t;
  double t3 = t2 * t;

  QPointF r1 = p0 * (s * s * s / 6);
  QPointF r2 = p1 * ((3 * t3 - 6 * t2 + 4) / 6);
  QPointF r3 = p2 * ((-3 * t3 + 3 * t2 + 3 * t + 1) / 6);
  QPointF r4 = p3 * (t3 / 6);

  return QPointF(r1 + r2 + r3 + r4);
}

size_t Spline::OrderedPointSet::insert(QPointF point) {
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

QPointF &Spline::extAt(size_t idx) { return extSupport[idx]; }

size_t Spline::extSize() { return extSupport.size(); }

void Spline::removeAt(size_t idx) {
  support.removeAt(idx);
  build(approx);
}

size_t Spline::size() { return support.size(); }
