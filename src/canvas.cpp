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

void Canvas::mousePressEvent(QMouseEvent *event) {
  QPointF pos = event->localPos();
  QPointF localPos = QPointF(pos.x() / width(), pos.y() / height());
  size_t rad2 = radius * radius;

  for (size_t i = 0; i < spline->supSize(); ++i) {
    double dx = spline->atSup(i).x() * width() - pos.x();
    double dy = spline->atSup(i).y() * height() - pos.y();
    if (dx * dx + dy * dy < rad2) {
      if (event->button() == Qt::LeftButton) {
        chosen = true;
        chosenIdx = i;
        originPos = spline->atSup(i);
        diffToPress = originPos - localPos;
      } else if (event->button() == Qt::RightButton) {
        spline->undoStack->push(
            new RemovePointCmd(i, spline->atSup(i), spline));
        repaint();
      }
      break;
    }
  }
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
  QPointF pos = event->localPos();
  QPointF localPos = QPointF(pos.x() / width(), pos.y() / height());
  QPointF point = localPos + diffToPress;
  double x = point.x(), y = point.y();

  if (chosen) {
    if (0 < x && x < 1) {
      if (0 < y && y < 1) {
        spline->resetPoint(chosenIdx, point);
      } else {
        spline->resetPointX(chosenIdx, x);
      }
    } else {
      if (0 < y && y < 1) {
        spline->resetPointY(chosenIdx, y);
      }
    }
    repaint();
  }
}

void Canvas::mouseReleaseEvent(QMouseEvent *) {
  if (chosen) {
    spline->undoStack->push(new MovePointCmd(chosenIdx, originPos,
                                             spline->atSup(chosenIdx), spline));
    chosen = false;
  }
}

void Canvas::paintEvent(QPaintEvent *) {
  QPainter painter(this);

  int w = width(), h = height();

  painter.fillRect(QRectF(0, 0, w, h), QBrush(QColor("white")));

  painter.setPen(QPen(Qt::gray, 1, Qt::DashLine));

  if (spline->supSize() > 0) {
    for (size_t i = 0; i < spline->supSize() - 1; ++i) {
      QPointF a = spline->atSup(i), b = spline->atSup(i + 1);
      painter.drawLine(a.x() * w, a.y() * h, b.x() * w, b.y() * h);
    }
    if (spline->supSize() > 4) {
      QPointF a = spline->atSup(spline->supSize() - 1), b = spline->atSup(0);
      painter.drawLine(a.x() * w, a.y() * h, b.x() * w, b.y() * h);
    }

    painter.setPen(QPen(Qt::gray, 1, Qt::SolidLine));

    for (size_t i = 0; i < spline->supSize() - 1; ++i) {
      QPointF a = spline->atSup(i);
      painter.drawEllipse(QPointF(a.x() * w, a.y() * h), radius, radius);
    }
    painter.setPen(QPen(Qt::blue, 1, Qt::SolidLine));
    QPointF a = spline->atSup(spline->supSize() - 1);
    painter.drawEllipse(QPointF(a.x() * w, a.y() * h), radius, radius);
  }

  painter.setPen(QPen(Qt::black, 1, Qt::SolidLine));

  for (size_t i = 0; spline->valSize() > 0 && i < spline->valSize() - 1; ++i) {
    QPointF a = spline->atVal(i), b = spline->atVal(i + 1);
    painter.drawLine(a.x() * w, a.y() * h, b.x() * w, b.y() * h);
  }
}

void Canvas::undoCmd() {
  spline->undoStack->undo();
  repaint();
}
