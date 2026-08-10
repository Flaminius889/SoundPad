#include "ui/mainwindow.h"

void MainWindow::setupPlayer()
{
    player = new QMediaPlayer(this);

    audioOutput = new QAudioOutput(this);

    audioInput = new QAudioInput(this);

    player->setAudioOutput(audioOutput);

    audioOutput->setVolume(0.5f);
}