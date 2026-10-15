<<<<<<< HEAD
#include "ui/mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtGui>
#include <QtCore>
#include <QFileDialog>
#include <QFileInfo>

void MainWindow::launchMusic(const QString &path)
{
    soundManager->playFile(path);
}

void MainWindow::updateWindowTitleForSource(const QString &path)
{
    const QString sourcePath = QDir::cleanPath(
        QFileInfo(path).absoluteFilePath());
    const auto &sounds =
        Repository::instance().soundsForCategory(ALL_SOUNDS);

    for (const SoundT &sound : sounds)
    {
        const QString soundPath = QDir::cleanPath(
            QFileInfo(sound.path).absoluteFilePath());

        if (soundPath.compare(sourcePath, Qt::CaseInsensitive) == 0)
        {
            setWindowTitle(SOUNDPAD + " - " + sound.nameOfMusic);
            return;
        }
    }
}

void MainWindow::playNextTrack()
{
    QString currentCategory = Repository::instance().getCurrCategoryData();
    const QVector<SoundT> &sounds =
        Repository::instance().soundsForCategory(currentCategory);

    int row = ui->soundsTable->selectionModel()->currentIndex().row();

    if (row < 0 || row + 1 >= sounds.size())
        return;

    row++;

    ui->soundsTable->selectRow(row);

    launchMusic(sounds[row].path);
}

void MainWindow::playPreviousTrack()
{
    QString currentCategory = Repository::instance().getCurrCategoryData();
    const QVector<SoundT> &sounds =
        Repository::instance().soundsForCategory(currentCategory);

    int row = ui->soundsTable->selectionModel()->currentIndex().row();
    const QUrl source = soundManager->source();
    if (!source.isEmpty())
    {
        const QString playingPath = QDir::cleanPath(
            QFileInfo(source.toLocalFile()).absoluteFilePath());
        for (int i = 0; i < sounds.size(); ++i)
        {
            const QString soundPath = QDir::cleanPath(
                QFileInfo(sounds[i].path).absoluteFilePath());
            if (soundPath.compare(playingPath, Qt::CaseInsensitive) == 0)
            {
                row = i;
                break;
            }
        }
    }

    if (soundManager->position() > 5000)
    {
        if (!source.isEmpty())
            launchMusic(source.toLocalFile());
        else if (row >= 0 && row < sounds.size())
            launchMusic(sounds[row].path);
        return;
    }

    if (row <= 0)
    {
        if (row == 0 && row < sounds.size())
            launchMusic(sounds[row].path);
        return;
    }

    row--;
    ui->soundsTable->selectRow(row);
    currentRow = row;
    launchMusic(sounds[row].path);
}
=======
#include "ui/mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtGui>
#include <QtCore>
#include <QFileDialog>
#include <QFileInfo>

void MainWindow::launchMusic(const QString &path)
{
    if (path.isEmpty())
        return;

    const QUrl newSource =
        QUrl::fromLocalFile(path);

    if (player->source() != newSource)
        player->setSource(newSource);

    player->play();
}

void MainWindow::playNextTrack()
{
    QMap<QString, QVector<SoundT>> categories = Repository::instance().getCategoriesData();
    QString currentCategory = Repository::instance().getCurrCategoryData();

    int row = ui->soundsTable->selectionModel()->currentIndex().row();

    if (row < 0 || row + 1 >= categories[currentCategory].size())
        return;

    row++;

    ui->soundsTable->selectRow(row);

    launchMusic(categories[currentCategory][row].path);
}

void MainWindow::playPreviousTrack()
{
    QMap<QString, QVector<SoundT>> categories = Repository::instance().getCategoriesData();
    QString currentCategory = Repository::instance().getCurrCategoryData();

    int row = ui->soundsTable->selectionModel()->currentIndex().row();

    if (row <= 0)
        return;

    row--;

    ui->soundsTable->selectRow(row);

    launchMusic(categories[currentCategory][row].path);
}
>>>>>>> origin/main
