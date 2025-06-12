#pragma once
#include <vector>
#include "Rectangle.h"

class OverlapEngine {
public:
    explicit OverlapEngine(std::vector<Rectangle>& rects);
    void computeOverlaps();
private:
    std::vector<Rectangle>& m_rects;
};
