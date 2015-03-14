#ifndef SPLINE_H
#define SPLINE_H

#include <QPainter>
#include <QPointF>
#include <QUndoStack>

class Spline {
public:
  explicit Spline(QObject *parent = nullptr);

  size_t addPoint(QPointF &point, bool rebuild = true);
  size_t addPoint(double x, double y);
  QPointF atSup(size_t idx);
  void build(size_t approxParam);
  QPointF &atVal(size_t idx);
  void removeAt(size_t idx, bool rebuild = true);
  void resetPoint(size_t idx, QPointF &point);
  void resetPointX(size_t idx, double x);
  void resetPointY(size_t idx, double y);
  size_t supSize();
  size_t valSize();

  QUndoStack *undoStack = nullptr;

private:
  class OrderedSet : public QList<double> {
  public:
    size_t insert(double point);
  };

  static constexpr size_t approx = 100;
  OrderedSet knots;
  QList<QPointF> support;
  QList<QPointF> values;

  static const int splineDegree = 3;

  double BSpline(int degree, int index, double t);
  QPointF generalSpline(double t);
};

#endif // SPLINE_H
