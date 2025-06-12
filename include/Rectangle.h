#pragma once
#include <vector>
#include <QMetaType>

struct Rectangle {
    int    id;
    double x, y, w, h;
    bool   overlaps{false};
};
Q_DECLARE_METATYPE(std::vector<Rectangle>)
