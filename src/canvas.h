#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <spline.h>

class Canvas : public QWidget {
  Q_OBJECT
public:
  explicit Canvas(QWidget *parent = 0);
  void setSpline(Spline *spline);

protected:
  void paintEvent(QPaintEvent *);
  /*
   * void mousePressEvent(QMouseEvent *);
   * void mouseDoubleClickEvent(QMouseEvent *);
   * void mouseMoveEvent(QMouseEvent *);
   * void mouseReleaseEvent(QMouseEvent *);
   */

private:
  Spline *spline = nullptr;
};

#endif // CANVAS_H
