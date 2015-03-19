#include <QHBoxLayout>
#include <QListWidgetItem>
#include <QSplitter>
#include <QFileDialog>

#include "tabwidget.h"

TabWidget::TabWidget(QWidget *parent) : QWidget(parent) {
  QHBoxLayout *layout = new QHBoxLayout;
  QSplitter *splitter = new QSplitter(this);

  list = new QListWidget(this);
  list->setSelectionMode(QAbstractItemView::SingleSelection);
  canvas = new Canvas(this);

  list->addItem(new QListWidgetItem("A"));

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
  canvas->setActiveGroup(&map[currentText]);
  // This does some extra work. Can be slow.
  canvas->getActiveGroup()->rebuildAll();
  canvas->repaint();
}

void TabWidget::addCurve() { canvas->addCurve(Spline()); }
void TabWidget::copyCurve() { canvas->copyCurve(); }
void TabWidget::pasteCurve() { canvas->pasteCurve(); }
void TabWidget::removeCurve() { canvas->removeCurve(); }

QString TabWidget::openProject(IOController &ioController) {
  QString filename = QFileDialog::getOpenFileName(
      this, tr("Open Project"), "", tr("Trident font files (*.trft)"));
  if (!filename.isEmpty()) {
    list->clear();
    map = ioController.readFromFile(filename);
    list->addItems(map.keys());
    list->setCurrentRow(0);
    reloadCanvas(list->selectedItems().first()->text());
  }
  return filename;
}

void TabWidget::saveProject(IOController &ioController) {
  QString filename = QFileDialog::getSaveFileName(
      this, tr("Save Project"), "", tr("Trident font files (*.trft)"));
  ioController.writeToFile(filename, map);
}

void TabWidget::redoCmd() { canvas->redoCmd(); }
void TabWidget::undoCmd() { canvas->undoCmd(); }
