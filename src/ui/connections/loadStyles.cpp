#include "ui/mainwindow.h"

QString MainWindow::loadStyle(const QString &path)
{
    QFile file(path);

    if (!file.open(QFile::ReadOnly))
        return {};

    return QString::fromUtf8(file.readAll());
}

void MainWindow::loadStyles()
{
    QFile file(":/styles.qss");

    if (file.open(QFile::ReadOnly))
    {
        qApp->setStyleSheet(file.readAll());
        file.close();
    }

    m_sliderEnabledStyle = loadStyle(":/slider.qss");
    m_sliderDisabledStyle = loadStyle(":/slider_disabled.qss");
}