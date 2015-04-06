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

void TabWidget::setHWRatio(double r)
{
  canvas->setHWRatio(r);
}

void TabWidget::setHSRatio(double r)
{
  canvas->setHSRatio(r);
}

QString TabWidget::openProject(IOController &ioController) {
  projectFileName = QFileDialog::getOpenFileName(
      this, tr("Open Project"), "", tr("Trident font files (*.trft)"));
  if (!projectFileName.isEmpty()) {
    list->clear();
    map = ioController.readFromFile(projectFileName);
    list->addItems(map.keys());
    list->setCurrentRow(0);
    reloadCanvas(list->selectedItems().first()->text());
  }
  QFileInfo fileInfo(projectFileName);
  return fileInfo.completeBaseName();
}

QString TabWidget::saveProject(IOController &ioController) {
  if (projectFileName.isEmpty()) {
    projectFileName = QFileDialog::getSaveFileName(
        this, tr("Save Project"), "", tr("Trident font files (*.trft)"));
  }
  ioController.writeToFile(projectFileName, map);
  QFileInfo fileInfo(projectFileName);
  return fileInfo.completeBaseName();
}

void TabWidget::addLetter(QChar name) {
  if (!map.contains(name)) {
    map.insert(name, SplineGroup());
    QListWidgetItem *item = new QListWidgetItem(name);
    list->addItem(item);
    list->setCurrentItem(item);
    reloadCanvas(name);
  }
}

void TabWidget::deleteLetter() {
  if (list->count() == 1) {
    return;
  }
  QListWidgetItem *active = list->currentItem();
  if (active) {
    QChar name = active->text().at(0);
    auto iter = map.find(name);
    map.erase(iter);
    delete active;
    list->setCurrentRow(0);
    reloadCanvas(list->currentItem()->text());
  }
}

void TabWidget::redoCmd() { canvas->redoCmd(); }
void TabWidget::undoCmd() { canvas->undoCmd(); }
