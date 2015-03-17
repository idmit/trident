#ifndef CANVASACTIONS
#define CANVASACTIONS

#include <QUndoCommand>

#include "splinegroup.h"

class AddPointCmd : public QUndoCommand {
public:
  AddPointCmd(size_t newIdx, QPointF point, SplineGroup *group)
      : newIdx(newIdx), point(point), group(group) {}
  virtual void redo() {
    activeSplineIdx = group->getIdx();
    idx = group->getActive().addPoint(newIdx, point);
  }
  virtual void undo() { group->get(activeSplineIdx).removeAt(idx); }

private:
  size_t activeSplineIdx = 0;
  size_t idx = 0;
  size_t newIdx = 0;
  QPointF point;
  SplineGroup *group;
};

class MovePointCmd : public QUndoCommand {
public:
  MovePointCmd(size_t idx, QPointF src, QPointF dst, SplineGroup *group)
      : idx(idx), src(src), dst(dst), group(group) {}
  virtual void redo() {
    activeSplineIdx = group->getIdx();
    group->getActive().resetPoint(idx, dst);
  }
  virtual void undo() { group->get(activeSplineIdx).resetPoint(idx, src); }

private:
  size_t activeSplineIdx = 0;
  size_t idx = 0;
  QPointF src, dst;
  SplineGroup *group;
};

class RemovePointCmd : public QUndoCommand {
public:
  RemovePointCmd(size_t idx, QPointF point, SplineGroup *group)
      : idx(idx), point(point), group(group) {}
  virtual void redo() {
    activeSplineIdx = group->getIdx();
    group->getActive().removeAt(idx);
  }
  virtual void undo() { group->get(activeSplineIdx).addPointTo(idx, point); }

private:
  size_t activeSplineIdx = 0;
  size_t idx = 0;
  QPointF point;
  SplineGroup *group;
};

class AddCurveCmd : public QUndoCommand {
public:
  AddCurveCmd(Spline spline, SplineGroup *group)
      : spline(spline), group(group) {}
  virtual void redo() { group->add(spline); }
  virtual void undo() { group->pop(); }

private:
  Spline spline;
  SplineGroup *group;
};

class MoveCurveCmd : public QUndoCommand {
public:
  MoveCurveCmd(size_t idx, Spline src, Spline dst, SplineGroup *group)
      : idx(idx), src(src), dst(dst), group(group) {}
  virtual void redo() { group->resetAt(idx, dst); }
  virtual void undo() { group->resetAt(idx, src); }

private:
  size_t idx = 0;
  Spline src, dst;
  SplineGroup *group;
};

class RemoveCurveCmd : public QUndoCommand {
public:
  RemoveCurveCmd(SplineGroup *group)
      : idx(group->getIdx()), spline(group->getActive()), group(group) {}
  virtual void redo() { group->removeAt(idx); }
  virtual void undo() { group->insertAt(idx, spline); }

private:
  size_t idx;
  Spline spline;
  SplineGroup *group;
};

#endif // CANVASACTIONS
