#include <QPainter>

#include "canvas.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent) {}

void Canvas::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  painter.setPen(QPen(Qt::red, 1, Qt::SolidLine));
  painter.drawLine(0, 0, width(), height());
}
