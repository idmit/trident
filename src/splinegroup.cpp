#include "splinegroup.h"

SplineGroup::SplineGroup() { splines.append(Spline()); }

void SplineGroup::setIdx(size_t idx) { activeIdx = idx; }

size_t SplineGroup::getIdx() { return activeIdx; }

void SplineGroup::add(Spline spline) {
  splines.append(spline);
  setIdx(splines.size() - 1);
}

Spline &SplineGroup::get(size_t idx) { return splines[idx]; }

Spline &SplineGroup::getActive() { return splines[activeIdx]; }

size_t SplineGroup::size() { return splines.size(); }
