#include "spline.h"

#include <cmath>

Spline::Spline(QObject *parent) {
  if (parent != nullptr) {
    undoStack = new QUndoStack(parent);
  }
}

size_t Spline::addPoint(QPointF &point, bool rebuild) {
  size_t idx = supSize();
  for (size_t i = 0; supSize() > 0 && i < supSize() - 1; ++i) {
    QMatrix mat;
    QPointF l = atSup(i), r = atSup(i + 1);
    double dy = r.y() - l.y(), dx = r.x() - l.x();
    double angle = std::atan(dy / dx);
    if (dx < 0) {
      angle += M_PI;
    }
    QPointF rel = point - l;
    mat.rotate(-angle * 57.2957795131);
    rel = mat.map(rel);
    rel += l;

    double dist = std::sqrt(dx * dx + dy * dy);

    if (l.x() < rel.x() && rel.x() < l.x() + dist) {
      if (l.y() - 0.01 < rel.y() && rel.y() < l.y() + 0.01) {
        idx = i + 1;
        break;
      }
    }
  }

  support.insert(idx, point);
  if (rebuild) {
    build(approx);
  }
  return idx;
}

size_t Spline::addPoint(double x, double y) {
  QPointF p(x, y);
  return addPoint(p);
}

size_t Spline::addPointTo(size_t idx, QPointF &point, bool rebuild) {
  support.insert(idx, point);
  if (rebuild) {
    build(approx);
  }
  return idx;
}

QPointF Spline::atSup(size_t idx) { return support[idx]; }

double Spline::bSpline(int deg, int index, double t) {
  if (deg == 0) {
    return (knots[index] <= t && t < knots[index + 1]) ? 1 : 0;
  }
  if (knots[index] == knots[index + deg + 1]) {
    return 0.0;
  }
  double s1 = 0, s2 = 0;
  if (knots[index + deg] - knots[index] > 1e-9) {
    s1 = bSpline(deg - 1, index, t) * (t - knots[index]) /
         (knots[index + deg] - knots[index]);
  }
  if (knots[index + deg + 1] - knots[index + 1] > 1e-9) {
    s2 = bSpline(deg - 1, index + 1, t) * (knots[index + deg + 1] - t) /
         (knots[index + deg + 1] - knots[index + 1]);
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

  double L = 0;
  for (size_t i = 1; i < supSize(); ++i) {
    QPointF p = support[i] - support[i - 1];
    L += std::sqrt(QPointF::dotProduct(p, p));
  }

  QList<double> ts;
  ts << 0;
  for (size_t i = 1; i < supSize() - 1; ++i) {
    QPointF p = support[i] - support[i - 1];
    ts << (ts[i - 1] + std::sqrt(QPointF::dotProduct(p, p)) / L);
  }
  ts << 1;

  double S = 0;
  knots.append(0);
  for (size_t i = 1; i < degree + 1; ++i) {
    knots.append(0);
    S += ts.at(i);
  }
  knots.insert(S / degree);
  for (size_t j = 2; j < supSize() - degree; j++) {
    knots.append(knots.at(j + degree - 1) +
                 (ts.at(j + degree - 1) - ts.at(j - 1)) / degree);
  }
  for (size_t i = 0; i < degree + 1; i++) {
    knots.append(1);
  }
}

void Spline::build(size_t approxParam) {
  double arg = 0;
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
    values.clear();
  }
}

size_t Spline::OrderedDoubleSet::append(double point) {
  QList<double>::push_back(point);
  return QList<double>::size() - 1;
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
