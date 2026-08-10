#include "ui/mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , settings(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/settings.ini",
               QSettings::IniFormat)
{
    ui->setupUi(this);

    loadStyles();
    createModels();

    setupPlayer();
    setDefaultSettings();

    setupTableView();
    setupTreeView();

    loadSettings();

    setupContextMenus();
    setupConnections();

    setupDragDrop();
}

MainWindow::~MainWindow()
{
    QString currentCategory = Repository::instance().getCurrCategoryData();
    int count = Repository::instance().getCategoriesData()[currentCategory].size();

    settings.setValue("currentCategory", Repository::instance().getCurrCategoryData());
    settings.setValue("categoriesOrder", Repository::instance().getCategoryOrder());

    settings.setValue("MediaPosition", player->position());
    settings.setValue("SliderPosition", ui->duration_time_slider->value());
    settings.setValue("Volume", ui->volumeControl->value());
    settings.setValue("MuteBTNIconPath", MuteBTNiconPath);
    settings.setValue("MusicCount", count);

    settings.setValue("DeviceOutput", audioOutput->device().id());
    settings.setValue("DeviceInput", audioInput->device().id());

    settings.setValue("CurrentTrack", currentRow);

    settings.setValue(
        "SoundsTableHeader",
        ui->soundsTable->horizontalHeader()->saveState()
        );

    Repository::instance().SaveData();
    delete ui;
}

void MainWindow::on_actionSettings_triggered()
{
    auto *dialog = new settingsDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    connect(dialog, &QDialog::accepted, this,
            [this, dialog]()
            {
                selectedOutputDevice = dialog->getOutputDevice();

                if (!selectedOutputDevice.isNull())
                {
                    audioOutput->setDevice(selectedOutputDevice);
                }
                else
                {
                    audioOutput->setDevice(QMediaDevices::defaultAudioOutput());
                }

                if(!selectedOutputDevice.isNull() || !selectedInputDevice.isNull())
                    qDebug() << "Audio device changed";
            });

    connect(dialog, &QDialog::rejected, this,
            []()
            {
                qDebug() << "Audio devices not changed";
            });

    dialog->show();
}

void MainWindow::onSoundDoubleClicked(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    if (index.column() == 3)
        return;

    const SoundT* sound = tableModel->soundAt(index.row());

    if (!sound)
        return;

    setWindowTitle(SOUNDPAD + " - " + sound->nameOfMusic);
    launchMusic(sound->path);
}

void MainWindow::onSelectionChanged()
{
    int count = ui->soundsTable
                    ->selectionModel()
                    ->selectedRows()
                    .count();

    ui->countOfSelected->setText(QString("Choosed: %1").arg(count));
}

void MainWindow::onCategorySelectionChanged()
{
    auto *header = ui->soundsTable->horizontalHeader();
    header->setSortIndicator(-1, Qt::AscendingOrder);

    QModelIndex index = ui->categoriesTree->currentIndex();
    if (!index.isValid())
        return;

    QString category = categoryModel->data(index, Qt::UserRole).toString();

    if (category.isEmpty())
        return;

    Repository::instance().getCurrCategoryData() = category;

    ui->countOfSelected->setText("Choosed: 0");
    refreshAllModels();
}

Qt::SortOrder MainWindow::toggleSortOrder(int column)
{
    Qt::SortOrder &order = sortOrders[column];

    order = order == Qt::AscendingOrder
                ? Qt::DescendingOrder
                : Qt::AscendingOrder;

    return order;
}