#include <QPainter>

#include "canvas.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent) {}

void Canvas::setSpline(Spline *value) { spline = value; }

void Canvas::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  painter.setPen(QPen(Qt::red, 1, Qt::DashLine));
  for (size_t i = 0; i < spline->size() - 1; ++i) {
    Point a = spline->at(i), b = spline->at(i + 1);
    painter.drawLine(a.x * width(), a.y * height(), b.x * width(),
                     b.y * height());
  }
}
