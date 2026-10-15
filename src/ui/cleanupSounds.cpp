<<<<<<< HEAD
#include "ui/mainwindow.h"

void MainWindow::cleanupSoundsBeforeRemoval(const QVector<SoundT>& sounds)
{
    QSet<int> processedIds;

    bool removeCurrentSource = false;

    QString currentSourcePath;

    if (!soundManager->source().isEmpty())
    {
        currentSourcePath = QDir::cleanPath(
            QFileInfo(soundManager->source().toLocalFile()).absoluteFilePath()
            );
    }

    for (const SoundT& sound : sounds)
    {
        // -------------------------
        // Remove hotkey
        // -------------------------
        if (!processedIds.contains(sound.id))
        {
            if (m_hotkeyManager)
                m_hotkeyManager->unregisterHotkey(sound.id);

            Repository::instance().setSoundHotkey(
                sound.id,
                QKeySequence()
                );

            processedIds.insert(sound.id);
        }

        // -------------------------
        // Check player source
        // -------------------------
        if (!currentSourcePath.isEmpty())
        {
            const QString soundPath = QDir::cleanPath(
                QFileInfo(sound.path).absoluteFilePath()
                );

            if (soundPath == currentSourcePath)
                removeCurrentSource = true;
        }
    }

    // -------------------------
    // Stop deleted sound
    // -------------------------
    if (removeCurrentSource)
    {
        soundManager->stopAndClear();

        ui->duration_time_slider->setValue(0);
        ui->timeOf_music->setText("0:00");

        currentRow = -1;
        setWindowTitle(SOUNDPAD);
    }
}
=======
#include "ui/mainwindow.h"

void MainWindow::cleanupSoundsBeforeRemoval(const QVector<SoundT>& sounds)
{
    QSet<int> processedIds;

    bool removeCurrentSource = false;

    QString currentSourcePath;

    if (!player->source().isEmpty())
    {
        currentSourcePath = QDir::cleanPath(
            QFileInfo(player->source().toLocalFile()).absoluteFilePath()
            );
    }

    for (const SoundT& sound : sounds)
    {
        // -------------------------
        // Remove hotkey
        // -------------------------
        if (!processedIds.contains(sound.id))
        {
            if (m_hotkeyManager)
                m_hotkeyManager->unregisterHotkey(sound.id);

            Repository::instance().setSoundHotkey(
                sound.id,
                QKeySequence()
                );

            processedIds.insert(sound.id);
        }

        // -------------------------
        // Check player source
        // -------------------------
        if (!currentSourcePath.isEmpty())
        {
            const QString soundPath = QDir::cleanPath(
                QFileInfo(sound.path).absoluteFilePath()
                );

            if (soundPath == currentSourcePath)
                removeCurrentSource = true;
        }
    }

    // -------------------------
    // Stop deleted sound
    // -------------------------
    if (removeCurrentSource)
    {
        player->stop();
        player->setSource({});

        ui->duration_time_slider->setValue(0);
        ui->timeOf_music->setText("0:00");

        currentRow = -1;
        setWindowTitle(SOUNDPAD);
    }
}
>>>>>>> origin/main
