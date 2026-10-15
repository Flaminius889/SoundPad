#include "ui/mainwindow.h"
#include <QMessageBox>

void MainWindow::setupSoundManager()
{
    soundManager = new SoundManager(this);
    connect(soundManager, &SoundManager::playbackError, this, [this](const QString &message) {
        QMessageBox::warning(this, tr("Audio playback"), message);
    });

    connect(soundManager, &SoundManager::sourceAccepted,
            this, &MainWindow::updateWindowTitleForSource);

    connect(soundManager, &SoundManager::sourceChanged,
            this, [this](const QUrl &source)
            {
                if (source.isEmpty())
                    setWindowTitle(SOUNDPAD);
            });
}
