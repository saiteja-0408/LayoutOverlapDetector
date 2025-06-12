#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <nlohmann/json.hpp>

#include "LayoutModel.h"
#include "WorkerThread.h"

class AppController : public QObject {
    Q_OBJECT
public:
    AppController(std::vector<Rectangle>& r, LayoutModel* m, QObject* p=nullptr)
      : QObject(p), rects(r), model(m) {}
public slots:
    void onDetectClicked() {
        auto* t = new WorkerThread(rects, model, this);
        connect(t, &QThread::finished, t, &QObject::deleteLater);
        t->start();
    }
private:
    std::vector<Rectangle>& rects;
    LayoutModel*            model;
};
#include "main.moc"

int main(int argc, char** argv) {
    QGuiApplication app(argc, argv);
    qRegisterMetaType<std::vector<Rectangle>>();

    // Determine which JSON to load (default = sample_layout.json)
    QStringList args = QCoreApplication::arguments();
    QString fileName = (args.size()>1
        ? args[1]
        : QStringLiteral("sample_layout.json")
    );

    // Compute path: executable_dir/../resources/layouts/<fileName>
    QDir exeDir(QCoreApplication::applicationDirPath());
    QString jsonPath = exeDir.filePath("../resources/layouts/" + fileName);
    qDebug() << "Loading JSON from disk path:" << jsonPath;

    QFile jsonFile(jsonPath);
    if (!jsonFile.open(QIODevice::ReadOnly|QIODevice::Text)) {
        qCritical() << "Failed to open JSON file:" << jsonPath
                    << jsonFile.errorString();
        return -1;
    }

    const auto raw = jsonFile.readAll();
    nlohmann::json j;
    try {
        j = nlohmann::json::parse(raw.constData(),
                                  raw.constData()+raw.size());
    } catch (const std::exception& e) {
        qCritical() << "JSON parse error:" << e.what();
        return -1;
    }

    // Build the model
    LayoutModel model;
    std::vector<Rectangle> rects;
    for (auto& item : j) {
        rects.push_back({
            item.value("id", 0),
            item.value("x", 0.0),
            item.value("y", 0.0),
            item.value("w", 50.0),
            item.value("h", 50.0),
            false
        });
    }
    model.setRectangles(rects);

    // QML setup
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("layoutModel", &model);
    AppController controller(rects, &model);
    engine.rootContext()->setContextProperty("controller", &controller);
    engine.load(QUrl(QStringLiteral("qrc:/qml/MainView.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
