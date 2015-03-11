#ifndef CANVASACTIONS
#define CANVASACTIONS

#include <QUndoCommand>

#include "spline.h"

class AddPointCmd : public QUndoCommand {
public:
  AddPointCmd(QPointF point, Spline *spline) : point(point), spline(spline) {}
  virtual void redo() { idx = spline->addPoint(point); }
  virtual void undo() { spline->removeAt(idx); }

private:
  size_t idx = 0;
  QPointF point;
  Spline *spline;
};

class MovePointCmd : public QUndoCommand {
public:
  MovePointCmd(size_t idx, QPointF src, QPointF dst, Spline *spline)
      : idx(idx), src(src), dst(dst), spline(spline) {}
  virtual void redo() { spline->resetPoint(idx, dst); }
  virtual void undo() { spline->resetPoint(idx, src); }

private:
  size_t idx = 0;
  QPointF src, dst;
  Spline *spline;
};

#endif // CANVASACTIONS
