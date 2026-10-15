<<<<<<< HEAD
#include "ui/mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtCore>
#include <QFileDialog>
#include <QFileInfo>
#include <QtWidgets>
#include <QMimeDatabase>

void MainWindow::addSound(const QString &filePath, const QString &category)
{
    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(filePath);

    qDebug() << filePath;
    qDebug() << mime.name();

    if (!mime.name().startsWith("audio/"))
    {
        QMessageBox::warning(
            this,
            "Error",
            "Incorrect type of file!"
            );
        return;
    }

    QFileInfo info(filePath);

    SoundT sound;

    int id = 0;

    for (const SoundCategory &categoryData :
         Repository::instance().getCategoriesData())
    {
        for (const SoundT &s : categoryData.sounds)
            id = std::max(id, s.id + 1);
    }

    sound.category = category;
    sound.id = id;
    sound.nameOfMusic = info.completeBaseName();
    sound.duration = getAudioDuration(filePath);
    sound.path = filePath;
    sound.data = getCurrentTime();

    Repository::instance().addSound(ALL_SOUNDS, sound);

    if(category != ALL_SOUNDS)
        Repository::instance().addSound(category, sound);

    refreshAllModels();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    QString category = Repository::instance().getCurrCategoryData();

    for (const QUrl &url : std::as_const(urls))
        addSound(url.toLocalFile(), category);

    refreshAllModels();
}

void MainWindow::on_actionAdd_new_sound_triggered()
{
    QString category = Repository::instance().getCurrCategoryData();

    QStringList files = QFileDialog::getOpenFileNames(
        this, "Choose musical file",
        "",
        "Music (*.mp3 *.wav *.ogg)");

    if (files.isEmpty()) return;

    for (const QString &file : files)
        addSound(file, category);

    refreshAllModels();
}
=======
#include "ui/mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtCore>
#include <QFileDialog>
#include <QFileInfo>
#include <QtWidgets>
#include <QMimeDatabase>

void MainWindow::addSound(const QString &filePath, const QString &category)
{
    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(filePath);

    qDebug() << filePath;
    qDebug() << mime.name();

    if (!mime.name().startsWith("audio/"))
    {
        QMessageBox::warning(
            this,
            "Error",
            "Incorrect type of file!"
            );
        return;
    }

    QFileInfo info(filePath);

    SoundT sound;

    int id = 0;

    for (auto it = Repository::instance().getCategoriesData().begin(); it != Repository::instance().getCategoriesData().end(); ++it)
    {
        for (const SoundT &s : it.value())
            id = std::max(id, s.id + 1);
    }

    sound.category = category;
    sound.id = id;
    sound.nameOfMusic = info.completeBaseName();
    sound.duration = getAudioDuration(filePath);
    sound.path = filePath;
    sound.data = getCurrentTime();

    Repository::instance().addSound(ALL_SOUNDS, sound);

    if(category != ALL_SOUNDS)
        Repository::instance().addSound(category, sound);

    refreshAllModels();
    updateCountLabel(category);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    QString category = Repository::instance().getCurrCategoryData();

    for (const QUrl &url : std::as_const(urls))
        addSound(url.toLocalFile(), category);

    refreshAllModels();
    updateCountLabel(category);
}

void MainWindow::on_actionAdd_new_sound_triggered()
{
    QString category = Repository::instance().getCurrCategoryData();

    QStringList files = QFileDialog::getOpenFileNames(
        this, "Choose musical file",
        "",
        "Music (*.mp3 *.wav *.ogg)");

    if (files.isEmpty()) return;

    for (const QString &file : files)
        addSound(file, category);

    refreshAllModels();
    updateCountLabel(category);
}
>>>>>>> origin/main
