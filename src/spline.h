#ifndef SPLINE_H
#define SPLINE_H

#include <QPainter>
#include <QPointF>
#include <QUndoStack>

class Spline {
public:
  explicit Spline(QObject *parent = nullptr);
  size_t addPoint(QPointF &point);
  size_t addPoint(double x, double y);
  QPointF &at(size_t idx);
  void removeAt(size_t idx);
  size_t size();

  QUndoStack *undoStack = nullptr;

private:
  class OrderedPointSet : public QList<QPointF> {
  public:
    size_t insert(QPointF &point);
  };

  OrderedPointSet points;
};

#endif // SPLINE_H
