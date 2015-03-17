#include <QMouseEvent>
#include <QPainter>

constexpr double pi = 3.1415926535;
constexpr double rad2deg = 57.2957795131;

#include <cmath>

#include "canvas.h"
#include "canvas_actions.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent) {}

void Canvas::setActiveSplineIdx(size_t idx) { activeGroup->setIdx(idx); }

void Canvas::setActiveGroup(SplineGroup *group) { activeGroup = group; }

void Canvas::drawGrid(QPainter &painter, size_t cellNum, size_t cellsInThick) {
  int w = width(), h = height();
  int grayTone = 240, lineWidth = 0;
  QColor color(grayTone, grayTone, grayTone);

  painter.fillRect(QRectF(0, 0, w, h), QBrush(QColor("white")));
  for (size_t i = 0; i < cellNum; ++i) {
    if ((i + 1) % cellsInThick == 0) {
      lineWidth = 1;
    } else {
      lineWidth = 2;
    }
    painter.setPen(QPen(color, lineWidth, Qt::SolidLine));
    painter.drawLine(0, i * h / cellNum, w, i * h / cellNum);
    painter.drawLine(i * w / cellNum, 0, i * w / cellNum, h);
  }
}

void Canvas::drawSplines(QPainter &painter) {
  int w = width(), h = height();

  for (size_t i = 0; i < activeGroup->size(); ++i) {
    Spline spline = activeGroup->get(i);

    painter.setPen(QPen(Qt::gray, 1, Qt::DashLine));
    if (spline.supSize() > 0) {
      for (size_t i = 0; i < spline.supSize() - 1; ++i) {
        QPointF a = spline.atSup(i), b = spline.atSup(i + 1);
        painter.drawLine(a.x() * w, a.y() * h, b.x() * w, b.y() * h);
      }
      QPointF a = spline.atSup(spline.supSize() - 1), b = spline.atSup(0);
      painter.drawLine(a.x() * w, a.y() * h, b.x() * w, b.y() * h);

      if (i == activeGroup->getIdx()) {
        painter.setPen(QPen(Qt::blue, 1, Qt::SolidLine));
      }

      for (size_t i = 0; i < spline.supSize(); ++i) {
        QPointF a = spline.atSup(i);
        painter.drawEllipse(QPointF(a.x() * w, a.y() * h), radius, radius);
      }
    }

    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine));
    for (size_t i = 0; spline.valSize() > 0 && i < spline.valSize() - 1; ++i) {
      QPointF a = spline.atVal(i), b = spline.atVal(i + 1);
      painter.drawLine(a.x() * w, a.y() * h, b.x() * w, b.y() * h);
    }
  }
}

void Canvas::mouseDoubleClickEvent(QMouseEvent *event) {
  QPointF pos = event->localPos();
  QPointF localPos = QPointF(pos.x() / width(), pos.y() / height());
  activeGroup->undoStack->push(
      new AddPointCmd(newPointIdx(localPos), localPos, activeGroup));
  repaint();
}

void Canvas::mousePressEvent(QMouseEvent *event) {
  QPointF pos = event->localPos();
  QPointF localPos = QPointF(pos.x() / width(), pos.y() / height());
  size_t rad2 = radius * radius;

  for (size_t k = 0; k < activeGroup->size(); ++k) {
    Spline spline = activeGroup->get(k);

    for (size_t i = 0; i < spline.supSize(); ++i) {
      double dx = spline.atSup(i).x() * width() - pos.x();
      double dy = spline.atSup(i).y() * height() - pos.y();
      if (dx * dx + dy * dy < rad2) {
        if (event->button() == Qt::LeftButton) {
          setActiveSplineIdx(k);
          chosen = true;
          chosenIdx = i;
          originPos = spline.atSup(i);
          diffToPress = originPos - localPos;
          repaint();
        } else if (event->button() == Qt::RightButton) {
          setActiveSplineIdx(k);
          if (spline.supSize() > 5)
            activeGroup->undoStack->push(
                new RemovePointCmd(i, spline.atSup(i), activeGroup));
          repaint();
        }
        break;
      }
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
        activeGroup->getActive().resetPoint(chosenIdx, point);
      } else {
        activeGroup->getActive().resetPointX(chosenIdx, x);
      }
    } else {
      if (0 < y && y < 1) {
        activeGroup->getActive().resetPointY(chosenIdx, y);
      }
    }
    repaint();
  }
}

void Canvas::mouseReleaseEvent(QMouseEvent *) {
  if (chosen && originPos != activeGroup->getActive().atSup(chosenIdx)) {
    activeGroup->undoStack->push(new MovePointCmd(
        chosenIdx, originPos, activeGroup->getActive().atSup(chosenIdx),
        activeGroup));
    chosen = false;
  }
}

void Canvas::paintEvent(QPaintEvent *) {
  QPainter painter(this);

  drawGrid(painter);
  drawSplines(painter);
}

size_t Canvas::newPointIdx(QPointF &point) {
  size_t idx = activeGroup->getActive().supSize();
  for (size_t k = 0; k < activeGroup->size(); ++k) {
    Spline s = activeGroup->get(k);

    for (size_t i = 0; s.supSize() > 0 && i < s.supSize() - 1; ++i) {
      QMatrix mat;
      QPointF l = s.atSup(i), r = s.atSup(i + 1);
      double dy = r.y() - l.y(), dx = r.x() - l.x();
      double angle = std::atan(dy / dx);
      if (dx < 0) {
        angle += pi;
      }
      QPointF rel = point - l;
      mat.rotate(-angle * rad2deg);
      rel = mat.map(rel);
      rel += l;

      double dist = std::sqrt(dx * dx + dy * dy);

      if (l.x() < rel.x() && rel.x() < l.x() + dist) {
        if (l.y() - 0.01 < rel.y() && rel.y() < l.y() + 0.01) {
          idx = i + 1;
          activeGroup->setIdx(k);
          break;
        }
      }
    }

    QPointF l = s.atSup(s.supSize() - 1), r = s.atSup(0);
    double dy = r.y() - l.y(), dx = r.x() - l.x();
    double angle = std::atan(dy / dx);
    if (dx < 0) {
      angle += pi;
    }
    QMatrix mat;
    QPointF rel = point - l;
    mat.rotate(-angle * rad2deg);
    rel = mat.map(rel);
    rel += l;

    double dist = std::sqrt(dx * dx + dy * dy);

    if (l.x() < rel.x() && rel.x() < l.x() + dist) {
      if (l.y() - 0.01 < rel.y() && rel.y() < l.y() + 0.01) {
        idx = s.supSize();
        activeGroup->setIdx(k);
      }
    }
  }

  return idx;
}

void Canvas::addCurve(Spline spline) {
  activeGroup->undoStack->push(new AddCurveCmd(spline, activeGroup));
  repaint();
}

void Canvas::copyCurve() {
  copiedSpline = activeGroup->getActive();
  repaint();
}

void Canvas::pasteCurve() {
  activeGroup->undoStack->push(new AddCurveCmd(copiedSpline, activeGroup));
  repaint();
}

void Canvas::removeCurve() {
  if (activeGroup->size() > 1) {
    activeGroup->undoStack->push(new RemoveCurveCmd(activeGroup));
  }
  repaint();
}

void Canvas::redoCmd() {
  activeGroup->undoStack->redo();
  repaint();
}

void Canvas::undoCmd() {
  activeGroup->undoStack->undo();
  repaint();
}
