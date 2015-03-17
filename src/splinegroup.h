#ifndef SPLINEGROUP_H
#define SPLINEGROUP_H

#include "spline.h"

#include <memory>
#include <QUndoStack>

class SplineGroup {
public:
  SplineGroup();
  void add(Spline spline);
  void insertAt(size_t idx, Spline spline);
  Spline &get(size_t idx);
  Spline &getActive();
  size_t getIdx();
  void pop();
  void removeAt(size_t idx);
  void setIdx(size_t idx);
  size_t size();

  QUndoStack *undoStack = new QUndoStack();

private:
  size_t activeIdx = 0;
  QVector<Spline> splines;
};

#endif // SPLINEGROUP_H
