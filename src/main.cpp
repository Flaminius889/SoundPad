#include "ui/mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QDirIterator>
#include <QDebug>

int main(int argc, char *argv[])
{
    // QAudioBufferOutput requires Qt's FFmpeg backend for microphone playback.
    qputenv("QT_MEDIA_BACKEND", "ffmpeg");
    QApplication app(argc, argv);
    qRegisterMetaType<QAudioDevice>();

    QCoreApplication::setApplicationVersion("1.0.0");
    QCoreApplication::setOrganizationName("MyCompany");
    QCoreApplication::setApplicationName("SoundPad");

    MainWindow w;
    w.show();

    return QApplication::exec();
}
#include "ui/mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QDirIterator>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qRegisterMetaType<QAudioDevice>();

    QCoreApplication::setApplicationVersion("1.0.0");
    QCoreApplication::setOrganizationName("MyCompany");
    QCoreApplication::setApplicationName("SoundPad");

    MainWindow w;
    w.resize(1100, 890);
    w.show();

    return QApplication::exec();
}
