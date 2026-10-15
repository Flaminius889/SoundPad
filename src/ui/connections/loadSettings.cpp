<<<<<<< HEAD
#include "ui/mainwindow.h"

void MainWindow::loadSettings()
{
    currentRow =
        settings.value("CurrentTrack", 0).toInt();

    const QUuid savedCategoryId(
        settings.value("CurrentCategoryId").toString());
    if (const SoundCategory *category =
            Repository::instance().categoryById(savedCategoryId))
    {
        Repository::instance().setCurrCategoryData(category->name);
    }
    else
    {
        Repository::instance().setCurrCategoryData(
            settings.value("currentCategory", ALL_SOUNDS).toString());
    }

    Repository::instance().setCategoryOrder(settings.value("categoriesOrder").toStringList());

    refreshAllModels();

    ui->volumeControl->setValue(
        settings.value("Volume", 25).toInt());

    QString iconPath =
        settings.value("MuteBTNIconPath").toString();

    if (!iconPath.isEmpty() && QFile::exists(iconPath))
    {
        QIcon icon(iconPath);

        ui->muteBtn->setIcon(icon);
    }

    const QString &category =
        Repository::instance().getCurrCategoryData();

    if (!Repository::instance().hasCategory(category))
        return;

    QByteArray headerState =
        settings.value("SoundsTableHeader").toByteArray();

    if (!headerState.isEmpty())
    {
        ui->soundsTable->horizontalHeader()->restoreState(headerState);
    }

    selectedOutputDevice = SoundManager::findOutputDevice(
        settings.value("DeviceOutput").toByteArray());
    soundManager->setOutputDevice(selectedOutputDevice);

    selectedInputDevice = SoundManager::findInputDevice(
        settings.value("DeviceInput").toByteArray());
    soundManager->setInputDevice(selectedInputDevice);

    soundManager->setRepeating(
        settings.value("IsRepeating", false).toBool());
    ui->repeat_btn->setChecked(soundManager->isRepeating());

    const auto &tracks =
        Repository::instance().soundsForCategory(category);

    if (currentRow < 0 || currentRow >= tracks.size())
        return;

    const SoundT &track = tracks[currentRow];

    qint64 position =
        settings.value("MediaPosition").toLongLong();

    soundManager->loadFile(track.path, position);
}
=======
#include "ui/mainwindow.h"

void MainWindow::loadSettings()
{
    currentRow =
        settings.value("CurrentTrack", 0).toInt();

    Repository::instance().getCurrCategoryData() =
        settings.value(
                    "currentCategory",
                    ALL_SOUNDS).toString();

    Repository::instance().setCategoryOrder(settings.value("categoriesOrder").toStringList());

    refreshAllModels();

    ui->volumeControl->setValue(
        settings.value("Volume", 25).toInt());

    QString iconPath =
        settings.value("MuteBTNIconPath").toString();

    if (!iconPath.isEmpty() && QFile::exists(iconPath))
    {
        QIcon icon(iconPath);

        ui->muteBtn->setIcon(icon);
    }

    auto &categories =
        Repository::instance().getCategoriesData();

    const QString &category =
        Repository::instance().getCurrCategoryData();

    if (!categories.contains(category))
        return;

    QByteArray headerState =
        settings.value("SoundsTableHeader").toByteArray();

    if (!headerState.isEmpty())
    {
        ui->soundsTable->horizontalHeader()->restoreState(headerState);
    }

    audioOutput->setDevice(findOutputDevice(
        settings.value("DeviceOutput").toByteArray()));

    audioInput->setDevice(findInputDevice(
        settings.value("DeviceInput").toByteArray()));

    const auto &tracks = categories[category];

    if (currentRow < 0 || currentRow >= tracks.size())
        return;

    const SoundT &track = tracks[currentRow];

    setWindowTitle(SOUNDPAD + '-' + track.nameOfMusic);

    qint64 position =
        settings.value("MediaPosition").toLongLong();

    m_restorePosition = position;

    player->setSource(
        QUrl::fromLocalFile(track.path));
}
>>>>>>> origin/main
