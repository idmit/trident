#include <QHBoxLayout>
#include <QListWidgetItem>
#include <QSplitter>

#include "tabwidget.h"

TabWidget::TabWidget(QWidget *parent) : QWidget(parent) {
  QHBoxLayout *layout = new QHBoxLayout;
  QSplitter *splitter = new QSplitter(this);

  list = new QListWidget(this);
  list->setSelectionMode(QAbstractItemView::SingleSelection);
  canvas = new Canvas(this);

  list->addItem(new QListWidgetItem("A"));
  list->addItem(new QListWidgetItem("B"));
  list->addItem(new QListWidgetItem("C"));

  Spline s;
  s.addPoint(0, 0);
  s.addPoint(0.33, 1);
  map["A"] = s;
  map["B"] = s;
  map["C"] = s;

  map["B"].at(1).x = 0.66;
  map["C"].at(1).x = 1;

  connect(list, &QListWidget::currentTextChanged, this,
          &TabWidget::reloadCanvas);

  list->setCurrentRow(0);

  splitter->addWidget(list);
  splitter->addWidget(canvas);
  canvas->resize(maximumSize());

  layout->addWidget(splitter);
  setLayout(layout);
}

TabWidget::~TabWidget() {}

void TabWidget::reloadCanvas(const QString &currentText) {
  canvas->setSpline(&map[currentText]);
  canvas->repaint();
}
