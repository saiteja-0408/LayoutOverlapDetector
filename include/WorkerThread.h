#pragma once
#include <QThread>
#include <vector>
#include "Rectangle.h"
class LayoutModel;

class WorkerThread : public QThread {
    Q_OBJECT
public:
    explicit WorkerThread(std::vector<Rectangle>& rects, LayoutModel* model, QObject* parent=nullptr);
    ~WorkerThread() override;
protected:
    void run() override;
private:
    std::vector<Rectangle>& m_rects;
    LayoutModel*            m_model;
};
