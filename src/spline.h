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
  size_t addPointTo(size_t idx, QPointF &point, bool rebuild = true);
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
  class OrderedDoubleSet : public QList<double> {
  public:
    size_t append(double point);
    size_t insert(double point);
  };

  using QPointFList = QList<QPointF>;

  static constexpr size_t approx = 100;
  OrderedDoubleSet knots;
  QPointFList support;
  QPointFList values;

  static constexpr size_t degree = 3;

  double bSpline(int deg, int index, double t);
  QPointF generalSpline(double t);
  void generateKnots();
};

#endif // SPLINE_H
