#include "LayoutModel.h"
LayoutModel::LayoutModel(QObject* p):QAbstractListModel(p){}
LayoutModel::~LayoutModel()=default;
void LayoutModel::setRectangles(const std::vector<Rectangle>& r){
    beginResetModel(); m_rects=r; endResetModel();
}
int LayoutModel::rowCount(const QModelIndex&) const { return int(m_rects.size()); }
QVariant LayoutModel::data(const QModelIndex& idx,int role) const {
    if(!idx.isValid()||idx.row()<0||idx.row()>=rowCount()) return {};
    auto const& r=m_rects[idx.row()];
    switch(role){
                 case IdRole:        return r.id;
                 case RectXRole:     return r.x;
                 case RectYRole:     return r.y;
                 case RectWRole:     return r.w;
                 case RectHRole:     return r.h;
                 case OverlapsRole:  return r.overlaps;
        default:            return {};

    }
    return {};
}
QHash<int,QByteArray> LayoutModel::roleNames() const {
    return {
             { IdRole,       "id"       },
             { RectXRole,    "rectX"    },
             { RectYRole,    "rectY"    },
             { RectWRole,    "rectW"    },
             { RectHRole,    "rectH"    },
             { OverlapsRole, "overlaps" }
           };
}
