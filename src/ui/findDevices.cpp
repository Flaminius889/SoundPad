#include "ui/mainwindow.h"

QAudioDevice MainWindow::findOutputDevice(const QByteArray &id)
{
    for (const auto &device : QMediaDevices::audioOutputs())
    {
        if (device.id() == id)
            return device;
    }

    return QMediaDevices::defaultAudioOutput();
}

QAudioDevice MainWindow::findInputDevice(const QByteArray &id)
{
    for (const auto &device : QMediaDevices::audioInputs())
    {
        if (device.id() == id)
            return device;
    }

    return QMediaDevices::defaultAudioInput();
}