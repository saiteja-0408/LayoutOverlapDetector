#include "WorkerThread.h"
#include "OverlapEngine.h"
#include "LayoutModel.h"
#include <QMetaObject>
WorkerThread::WorkerThread(std::vector<Rectangle>& r, LayoutModel* m, QObject* p)
    : QThread(p), m_rects(r), m_model(m){}
WorkerThread::~WorkerThread()=default;
void WorkerThread::run(){
    OverlapEngine eng(m_rects);
    eng.computeOverlaps();
    QMetaObject::invokeMethod(m_model,"setRectangles",Qt::QueuedConnection,
        Q_ARG(std::vector<Rectangle>, m_rects));
}
