#pragma once
#include <QAbstractListModel>
#include <vector>
#include "Rectangle.h"

class LayoutModel : public QAbstractListModel {
    Q_OBJECT
public:
         enum Roles {
           IdRole        = Qt::UserRole,
           RectXRole,    // avoid clash with QML 'x'
           RectYRole,    // avoid clash with QML 'y'
           RectWRole,    // avoid clash with QML 'width'
           RectHRole,    // avoid clash with QML 'height'
           OverlapsRole
         };
    explicit LayoutModel(QObject* parent=nullptr);
    ~LayoutModel() override;
    Q_INVOKABLE void setRectangles(const std::vector<Rectangle>& rects);
    int rowCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int,QByteArray> roleNames() const override;

private:
    std::vector<Rectangle> m_rects;
};
