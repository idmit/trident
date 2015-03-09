#include <QHBoxLayout>
#include <QListWidgetItem>
#include <QSplitter>

#include "tabwidget.h"

TabWidget::TabWidget(QWidget *parent) : QWidget(parent) {
  QHBoxLayout *layout = new QHBoxLayout;
  QSplitter *splitter = new QSplitter(this);

  map["A"] = Spline(this);
  map["B"] = Spline(this);
  map["C"] = Spline(this);

  list = new QListWidget(this);
  list->setSelectionMode(QAbstractItemView::SingleSelection);
  canvas = new Canvas(this);

  list->addItem(new QListWidgetItem("A"));
  list->addItem(new QListWidgetItem("B"));
  list->addItem(new QListWidgetItem("C"));

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

void TabWidget::undoCmd() { canvas->undoCmd(); }
