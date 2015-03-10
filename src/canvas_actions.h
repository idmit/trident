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

#endif // CANVASACTIONS
