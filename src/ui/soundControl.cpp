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