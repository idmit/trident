#include "spline.h"

Spline::Spline(QObject *parent) {
  if (parent != nullptr) {
    undoStack = new QUndoStack(parent);
  }
}

size_t Spline::addPoint(QPointF &point, bool rebuild) {
  support.push_back(point);
  if (rebuild) {
    build(approx);
  }
  return support.size() - 1;
}

size_t Spline::addPoint(double x, double y) {
  QPointF p(x, y);
  return addPoint(p);
}

QPointF Spline::atSup(size_t idx) { return support[idx]; }

double Spline::bSpline(int deg, int index, double t) {
  if (deg == 0) {
    return (t >= knots[index] && t < knots[index + 1]) ? 1.0 : 0.0;
  }
  if (knots[index] == knots[index + deg + 1]) {
    return 0.0;
  }
  double s1 = 0.0, s2 = 0.0;
  if (knots[index + deg] != knots[index]) {
    s1 = (t - knots[index]) / (knots[index + deg] - knots[index]) *
         bSpline(deg - 1, index, t);
  }
  if (knots[index + deg + 1] != knots[index + 1]) {
    s2 = (knots[index + deg + 1] - t) /
         (knots[index + deg + 1] - knots[index + 1]) *
         bSpline(deg - 1, index + 1, t);
  }
  return s1 + s2;
}

QPointF Spline::generalSpline(double t) {
  QPointF sum(0.0, 0.0);
  size_t supSize = support.size();
  for (size_t i = 0; i < supSize; ++i) {
    sum += support[i] * bSpline(degree, i, t);
  }
  return sum;
}

void Spline::generateKnots() {
  knots.clear();
  knots.insert(1);
  for (size_t i = 1; i < supSize(); ++i) {
    QPointF p = support[i] - support[i - 1];
    knots.insert(knots[i - 1] + QPointF::dotProduct(p, p));
  }

  int k = degree + 1;
  for (int i = 0; i < k / 2; i++) {
    knots.insert(1);
  }
  for (int i = k / 2; i < k; i++) {
    knots.insert(knots.last());
  }
}

void Spline::build(size_t approxParam) {
  double arg = 1;
  size_t supSize = support.size();
  QPointFList vals;

  if (supSize > 3) {
    generateKnots();
    double step = (knots.last() - knots.first()) / (double)approxParam;
    for (size_t num = 1; num < approxParam - 1; ++num) {
      arg += step;
      vals.push_back(generalSpline(arg));
    }
    values = vals;
  } else {
    values = support;
  }
}

size_t Spline::OrderedDoubleSet::insert(double point) {
  size_t idx = 0;
  for (const auto &p : (*this)) {
    if (p >= point) {
      break;
    }
    ++idx;
  }
  QList<double>::insert(idx, point);
  return idx;
}

QPointF &Spline::atVal(size_t idx) { return values[idx]; }

void Spline::removeAt(size_t idx, bool rebuild) {
  support.removeAt(idx);
  if (rebuild) {
    build(approx);
  }
}

void Spline::resetPoint(size_t idx, QPointF &point) {
  support[idx] = point;
  build(approx);
}

void Spline::resetPointX(size_t idx, double x) {
  support[idx].setX(x);
  build(approx);
}

void Spline::resetPointY(size_t idx, double y) {
  support[idx].setY(y);
  build(approx);
}

size_t Spline::supSize() { return support.size(); }

size_t Spline::valSize() { return values.size(); }
