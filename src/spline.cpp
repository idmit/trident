#include "spline.h"

Spline::Spline(QObject *parent) {
  if (parent != nullptr) {
    undoStack = new QUndoStack(parent);
  }
}

size_t Spline::addPoint(QPointF &point, bool rebuild) {
  support.push_back(point);
  int idx = support.size() - 1;
  //knots.insert(idx);
  if (rebuild) {
    build(approx);
  }
  return idx;
}

size_t Spline::addPoint(double x, double y) {
  QPointF p(x, y);
  support.push_back(p);
  int idx = support.size() - 1;
  //knots.insert(idx);
  return idx;
}

QPointF Spline::atSup(size_t idx) { return support[idx]; }

double Spline::BSpline(int degree, int index, double t) {
  if (degree == 0) {
    return (t >= knots[index] && t < knots[index + 1]) ? 1.0 : 0.0;
  }
  if (knots[index] == knots[index + degree + 1]) {
    return 0.0;
  }
  double s1 = 0.0, s2 = 0.0;
  if (knots[index + degree] != knots[index]) {
    s1 = (t - knots[index]) / (knots[index + degree] - knots[index]) *
         BSpline(degree - 1, index, t);
  }
  if (knots[index + degree + 1] != knots[index + 1]) {
    s2 = (knots[index + degree + 1] - t) /
         (knots[index + degree + 1] - knots[index + 1]) *
         BSpline(degree - 1, index + 1, t);
  }
  return s1 + s2;
}

QPointF Spline::generalSpline(double t) {
  QPointF sum(0.0, 0.0);
  size_t supSize = support.size();
  for (size_t i = 0; i < supSize; ++i) {
    sum += support[i] * BSpline(splineDegree, i, t);
  }
  return sum;
}

void Spline::build(size_t approxParam) {
  double arg = 0;
  size_t supSize = support.size();
  QList<QPointF> extendedSupport;


  if (supSize > 3) {
    int k = supSize + splineDegree + 1;
    for (int i = 0; i < k; i++) {
        knots.insert(i);
    }
   /* if (knots.size() != support.size() + splineDegree + 1) {
      size_t k = support.size() + splineDegree + 1 - knots.size();
      for (size_t i = 0;i < k / 2;i++)
      {
        knots.insert(0.0);
      }
      double d = knots.last();
      for (size_t i = k / 2;i < k;i++)
      {
        knots.insert(d);
      }
    }*/
    double step = (knots.size() - 1) / (double)approxParam;
    for (size_t num = step; num < approxParam; ++num) {
      extendedSupport.push_back(generalSpline(arg));
      arg += step;
    }
    values = extendedSupport;
  } else {
    values = support;
  }
}

size_t Spline::OrderedSet::insert(double point) {
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
