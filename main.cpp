#include <QDir>
#include <QGuiApplication>
#include <QFileSystemModel>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QDebug>

static QFileSystemModel* mptr = nullptr;

void printDirectoryInfo(const QModelIndex& index, const QString path = QString{}){

    auto rowCount = mptr->rowCount(index);
    auto fileInfo = mptr->fileInfo(index);
    qInfo() << QString{path.toStdString().data()} << "/" << fileInfo.fileName() << " " << rowCount;
    auto newpath = path + "/" + fileInfo.fileName();
    for (auto indexCount = 0; indexCount < rowCount; ++indexCount){

        auto children_index = mptr->index(indexCount, 0, index);
        auto children_fileInfo = mptr->fileInfo(children_index);
        if (children_fileInfo.isDir()){
            printDirectoryInfo(children_index, newpath);
        } else qInfo() << newpath << "/" << children_fileInfo.fileName();
    }


}


void dloaded(const QString &path){

    auto index = mptr->index(path);
    printDirectoryInfo(index);

}
int main(int argc, char *argv[])
{

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);


    auto rootpath = QString{"/mnt/shared/datasets/colombia/ivan/083019_ivan_xiaomi/"};

    mptr = new QFileSystemModel;
    auto engineContext = engine.rootContext();
    engineContext->setContextProperty("fileSystemModel", mptr);

    QObject::connect(mptr,&QFileSystemModel::directoryLoaded, dloaded);
    mptr -> setRootPath(rootpath);
    qInfo() << mptr;


    engine.load(url);
    return app.exec();
}
