#include "ui/mainwindow.h"

QString MainWindow::getCurrentTime() {
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
}

QString TableModel::ModelformatDuration(qint64 ms) const
{
    int totalSeconds = ms / 1000;

    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    return QString("%1:%2")
        .arg(minutes)
        .arg(seconds, 2, 10, QChar('0'));
}

QString MainWindow::formatDuration(qint64 ms)
{
    int totalSeconds = ms / 1000;

    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    return QString("%1:%2")
        .arg(minutes)
        .arg(seconds, 2, 10, QChar('0'));
}

qint64 MainWindow::getAudioDuration(const QString& filePath)
{
    QMediaPlayer player;
    QEventLoop loop;

    QObject::connect(&player,
                     &QMediaPlayer::durationChanged,
                     &loop,
                     &QEventLoop::quit);

    player.setSource(QUrl::fromLocalFile(filePath));

    QTimer::singleShot(3000, &loop, &QEventLoop::quit);

    loop.exec();

    return player.duration();
}