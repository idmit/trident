#include <QMouseEvent>
#include <QPainter>

#include "canvas.h"
#include "canvas_actions.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent) {}

void Canvas::setSpline(Spline *value) { spline = value; }

void Canvas::mouseDoubleClickEvent(QMouseEvent *event) {
  QPointF pos = event->localPos();
  QPointF localPos = QPointF(pos.x() / width(), pos.y() / height());
  spline->undoStack->push(new AddPointCmd(localPos, spline));
  repaint();
}

void Canvas::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  painter.setPen(QPen(Qt::red, 1, Qt::DashLine));

  for (size_t i = 0; spline->supSize() > 0 && i < spline->supSize() - 1; ++i) {
    QPointF a = spline->atSup(i), b = spline->atSup(i + 1);
    painter.drawLine(a.x() * width(), a.y() * height(), b.x() * width(),
                     b.y() * height());
  }

  painter.setPen(QPen(Qt::red, 1, Qt::SolidLine));

  for (size_t i = 0; i < spline->supSize(); ++i) {
    QPointF a = spline->atSup(i);
    painter.drawEllipse(QPointF(a.x() * width(), a.y() * height()), radius,
                        radius);
  }

  for (size_t i = 0; spline->valSize() > 0 && i < spline->valSize() - 1; ++i) {
    QPointF a = spline->atVal(i), b = spline->atVal(i + 1);
    painter.drawLine(a.x() * width(), a.y() * height(), b.x() * width(),
                     b.y() * height());
  }
}

void Canvas::undoCmd() {
  spline->undoStack->undo();
  repaint();
}
