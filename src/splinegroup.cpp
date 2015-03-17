#include "splinegroup.h"

SplineGroup::SplineGroup() { splines.append(Spline()); }

void SplineGroup::setIdx(size_t idx) { activeIdx = idx; }

size_t SplineGroup::getIdx() { return activeIdx; }

void SplineGroup::add(Spline spline) {
  splines.append(spline);
  setIdx(splines.size() - 1);
}

void SplineGroup::pop() {
  splines.pop_back();
  if (activeIdx == (size_t)splines.size()) {
    activeIdx -= 1;
  }
}

void SplineGroup::insertAt(size_t idx, Spline spline) {
  splines.insert(idx, spline);
}

void SplineGroup::removeAt(size_t idx) {
  splines.removeAt(idx);
  if (activeIdx == (size_t)splines.size()) {
    activeIdx -= 1;
  }
}

Spline &SplineGroup::get(size_t idx) { return splines[idx]; }

Spline &SplineGroup::getActive() { return splines[activeIdx]; }

size_t SplineGroup::size() { return splines.size(); }
