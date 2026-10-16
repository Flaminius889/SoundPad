#include "ui/mainwindow.h"
#include <QShortcut>

void MainWindow::setupConnections()
{
    // =========================
    // 🎵 MEDIA PLAYER
    // =========================

    connect(soundManager, &SoundManager::positionChanged,
            this, [this](qint64 position)
            {
                if (ui->duration_time_slider->isSliderDown())
                    return;

                ui->timeOf_music->setText(formatDuration(position));
                ui->duration_time_slider->setValue(
                    static_cast<int>(position));
            });

    connect(soundManager, &SoundManager::durationChanged,
            this, [this](qint64 duration)
            {
                ui->duration_time_slider->setRange(0, duration);
            });

    connect(soundManager, &SoundManager::playbackStateChanged,
            this, [this](bool playing)
            {
                ui->duration_time_slider->setEnabled(playing);

                ui->duration_time_slider->setStyleSheet(
                    playing ? m_sliderEnabledStyle
                            : m_sliderDisabledStyle
                    );
            });

    connect(soundManager, &SoundManager::mediaEnded,
            this, [this]()
            {
                ui->duration_time_slider->setEnabled(false);
            });

    connect(ui->duration_time_slider, &QSlider::sliderMoved,
            this, [this](int position)
            {
                ui->timeOf_music->setText(formatDuration(position));
            });

    connect(ui->duration_time_slider, &QSlider::sliderReleased,
            this, [this]()
            {
                soundManager->setPosition(
                    ui->duration_time_slider->value());
            });

    connect(ui->duration_time_slider, &SeekSlider::seekRequested,
            this, [this](int position)
            {
                ui->timeOf_music->setText(formatDuration(position));
                soundManager->setPosition(position);
            });

    // =========================
    // BUTTONS (PLAYBACK)
    // =========================

    connect(ui->playBtn, &QPushButton::clicked,
            soundManager, &SoundManager::play);

    connect(ui->playOnHeadset, &QPushButton::clicked,
            soundManager, &SoundManager::play);

    ui->playOnHeadset->setToolTip(tr("Play through the selected output device"));
    ui->playBtn->setToolTip(tr("Play"));

    connect(ui->pauseBtn, &QPushButton::clicked,
            soundManager, &SoundManager::pause);

    connect(ui->stopBtn, &QPushButton::clicked,
            this, [this]()
            {
                ui->duration_time_slider->setValue(0);
                ui->timeOf_music->setText("0:00");

                soundManager->stopAndClear();

                currentRow = -1;
                setWindowTitle(SOUNDPAD);
            });

    connect(ui->repeat_btn, &QPushButton::toggled,
            soundManager, &SoundManager::setRepeating);

    connect(soundManager, &SoundManager::repeatingChanged,
            ui->repeat_btn, &QPushButton::setChecked);

    connect(ui->nextBtn, &QPushButton::clicked,
            this, &MainWindow::playNextTrack);

    connect(ui->previousBtn, &QPushButton::clicked,
            this, &MainWindow::playPreviousTrack);

    // =========================
    // TABLE VIEW
    // =========================

    connect(ui->soundsTable,
            &QTableView::clicked,
            this,
            [this](const QModelIndex &index)
            {
                currentRow = index.row();
            });

    connect(ui->soundsTable->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &MainWindow::onSelectionChanged);

    connect(ui->soundsTable,
            &QTableView::doubleClicked,
            this,
            &MainWindow::onSoundDoubleClicked);

    connect(ui->soundsTable->horizontalHeader(),
            &QHeaderView::sectionClicked,
            this,
            [this](int column)
            {
                if (column == 0)
                    return;

                Qt::SortOrder order = toggleSortOrder(column);

                tableModel->sort(column, order);

                ui->soundsTable->horizontalHeader()
                    ->setSortIndicator(column, order);

                ui->soundsTable->viewport()->update();
            });

    // =========================
    // CATEGORY TREE
    // =========================

    connect(ui->categoriesTree->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &MainWindow::onCategorySelectionChanged);

    // =========================
    // DRAG & DROP (MODEL SIGNALS)
    // =========================

    connect(tableModel, &TableModel::soundsMoved,
            this, &MainWindow::onSoundsMoved);

    connect(categoryModel, &CategoryModel::soundsMovedToCategory,
            this, &MainWindow::onSoundsMovedToCategory);

    connect(categoryModel, &CategoryModel::categoryMoved,
            this, &MainWindow::onCategoryMoved);

    connect(tableModel, &TableModel::soundMovedWithinCategory,
            this, &MainWindow::onSoundMovedWithinCategory);

    // =========================
    // CONTEXT MENU (TABLE)
    // =========================

    connect(ui->soundsTable,
            &QWidget::customContextMenuRequested,
            m_contextMenuManager,
            &ContextMenuManager::showTableMenu);

    connect(ui->categoriesTree,
            &QWidget::customContextMenuRequested,
            m_contextMenuManager,
            &ContextMenuManager::showTreeMenu);

    connect(ui->lower_bar,
            &QWidget::customContextMenuRequested,
            m_contextMenuManager,
            &ContextMenuManager::showLowerBarMenu);

    QShortcut *deleteTableShortcut = new QShortcut(QKeySequence::Delete, ui->soundsTable);

    deleteTableShortcut->setContext(Qt::WidgetWithChildrenShortcut);

    connect(deleteTableShortcut, &QShortcut::activated, this, [this]()
            {
                QModelIndexList selectedRows = ui->soundsTable->selectionModel()->selectedRows();

                if (selectedRows.isEmpty())
                    return;

                QStringList soundIds;

                for (const QModelIndex &idx : selectedRows)
                    soundIds << idx.data(TableModel::IdRole).toString();

                handleDeleteSounds(soundIds);
            });

    QShortcut *deleteCategoryShortcut = new QShortcut(QKeySequence::Delete, ui->categoriesTree);

    deleteCategoryShortcut->setContext(Qt::WidgetWithChildrenShortcut);

    connect(deleteCategoryShortcut, &QShortcut::activated, this, [this]()
            {
                QModelIndex index = ui->categoriesTree->currentIndex();

                if (!index.isValid())
                    return;

                QString categoryName = index.data(Qt::UserRole).toString();

                if (categoryName.isEmpty())
                    return;

                handleDeleteCategory(categoryName);
            });

    connect(m_contextMenuManager, &ContextMenuManager::addCategoryRequested,
            this, &MainWindow::handleAddCategory);

    connect(m_contextMenuManager, &ContextMenuManager::deleteSoundsRequested,
            this, &MainWindow::handleDeleteSounds);

    connect(m_contextMenuManager,
            &ContextMenuManager::deleteCategoryRequested,
            this,
            &MainWindow::handleDeleteCategory);


    connect(m_contextMenuManager, &ContextMenuManager::addHotkeyRequested,
            this, &MainWindow::handleAddHotkey);

    connect(m_contextMenuManager, &ContextMenuManager::removeHotkeyRequested,
            this, &MainWindow::handleRemoveHotkey);

    connect(ui->soundsTable, &QTableView::doubleClicked, this,
            [this](const QModelIndex &index)
            {
                if (index.column() != 3)
                    return;

                QString soundId = index.data(TableModel::IdRole).toString();
                handleAddHotkey(soundId);
            });

    connect(m_contextMenuManager, &ContextMenuManager::showAllSoundsRequested,
            this, &MainWindow::handleShowAllSounds);
    connect(m_contextMenuManager, &ContextMenuManager::changeCategoryRequested,
            this, &MainWindow::handleChangeCategory);

    // =========================
    // HotKeys
    // =========================

    m_hotkeyManager = new HotkeyManager(this);

    connect(m_hotkeyManager, &HotkeyManager::hotkeyPressed,
        this,
        [this](int id)
        {
            const auto &categories = Repository::instance().getCategoriesData();

            for (const SoundCategory &category : categories)
            {
                for (const auto& sound : category.sounds)
                {
                    if(sound.id == id) {
                        launchMusic(sound.path);
                        return;
                    }
                }
            }
        });

    auto &allSounds = Repository::instance().soundsForCategory(ALL_SOUNDS);

    if(allSounds.empty()) return;

    for (const auto &sound : allSounds)
    {
        if (!sound.hotkey.isEmpty())
            m_hotkeyManager->registerHotkey(sound.id, sound.hotkey);
    }
}
#include "ui/mainwindow.h"
#include <QShortcut>

void MainWindow::setupConnections()
{
    // =========================
    // 🎵 MEDIA PLAYER
    // =========================

    connect(player, &QMediaPlayer::positionChanged,
            this, [this](qint64 position)
            {
                ui->timeOf_music->setText(formatDuration(position));
            });

    connect(player, &QMediaPlayer::positionChanged,
            ui->duration_time_slider,
            &QSlider::setValue);

    connect(player, &QMediaPlayer::durationChanged,
            this, [this](qint64 duration)
            {
                ui->duration_time_slider->setRange(0, duration);
            });

    connect(player, &QMediaPlayer::playbackStateChanged,
            this, [this](QMediaPlayer::PlaybackState state)
            {
                bool playing = (state == QMediaPlayer::PlayingState);

                ui->duration_time_slider->setEnabled(playing);

                ui->duration_time_slider->setStyleSheet(
                    playing ? m_sliderEnabledStyle
                            : m_sliderDisabledStyle
                    );
            });

    connect(player, &QMediaPlayer::mediaStatusChanged,
            this, [this](QMediaPlayer::MediaStatus status)
            {
                if (status == QMediaPlayer::EndOfMedia)
                    ui->duration_time_slider->setEnabled(false);

                if (status != QMediaPlayer::LoadedMedia)
                    return;

                if (m_restorePosition >= 0)
                {
                    player->setPosition(m_restorePosition);

                    ui->duration_time_slider->setValue(
                        static_cast<int>(m_restorePosition));

                    ui->timeOf_music->setText(
                        formatDuration(m_restorePosition));

                    m_restorePosition = -1;
                }
            });

    connect(ui->duration_time_slider,
            &QSlider::sliderMoved,
            player,
            &QMediaPlayer::setPosition);

    // =========================
    // BUTTONS (PLAYBACK)
    // =========================

    connect(ui->playBtn, &QPushButton::clicked,
            player, &QMediaPlayer::play);

    connect(ui->playOnHeadset, &QPushButton::clicked,
            this, [this]()
            {
                if(!selectedOutputDevice.isNull())
                    audioOutput->setDevice(selectedOutputDevice);
                player->play();
                if (selectedOutputDevice.isNull())
                    audioOutput->setDevice(QMediaDevices::defaultAudioOutput());
                player->play();
            });

    connect(ui->pauseBtn, &QPushButton::clicked,
            player, &QMediaPlayer::pause);

    connect(ui->stopBtn, &QPushButton::clicked,
            this, [this]()
            {
                ui->duration_time_slider->setValue(0);
                ui->timeOf_music->setText("0:00");

                player->stop();
                player->setSource({});

                currentRow = -1;
                setWindowTitle(SOUNDPAD);
            });

    connect(ui->nextBtn, &QPushButton::clicked,
            this, &MainWindow::playNextTrack);

    connect(ui->previousBtn, &QPushButton::clicked,
            this, &MainWindow::playPreviousTrack);

    // =========================
    // TABLE VIEW
    // =========================

    connect(ui->soundsTable,
            &QTableView::clicked,
            this,
            [this](const QModelIndex &index)
            {
                currentRow = index.row();
            });

    connect(ui->soundsTable->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &MainWindow::onSelectionChanged);

    connect(ui->soundsTable,
            &QTableView::doubleClicked,
            this,
            &MainWindow::onSoundDoubleClicked);

    connect(ui->soundsTable->horizontalHeader(),
            &QHeaderView::sectionClicked,
            this,
            [this](int column)
            {
                if (column == 0)
                    return;

                Qt::SortOrder order = toggleSortOrder(column);

                tableModel->sort(column, order);

                ui->soundsTable->horizontalHeader()
                    ->setSortIndicator(column, order);

                ui->soundsTable->viewport()->update();
            });

    // =========================
    // CATEGORY TREE
    // =========================

    connect(ui->categoriesTree->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &MainWindow::onCategorySelectionChanged);

    // =========================
    // DRAG & DROP (MODEL SIGNALS)
    // =========================

    connect(tableModel, &TableModel::soundsMoved,
            this, &MainWindow::onSoundsMoved);

    connect(categoryModel, &CategoryModel::soundsMovedToCategory,
            this, &MainWindow::onSoundsMovedToCategory);

    connect(categoryModel, &CategoryModel::categoryMoved,
            this, &MainWindow::onCategoryMoved);

    connect(tableModel, &TableModel::soundMovedWithinCategory,
            this, &MainWindow::onSoundMovedWithinCategory);

    // =========================
    // CONTEXT MENU (TABLE)
    // =========================

    connect(ui->soundsTable,
            &QWidget::customContextMenuRequested,
            m_contextMenuManager,
            &ContextMenuManager::showTableMenu);

    connect(ui->categoriesTree,
            &QWidget::customContextMenuRequested,
            m_contextMenuManager,
            &ContextMenuManager::showTreeMenu);

    connect(ui->lower_bar,
            &QWidget::customContextMenuRequested,
            m_contextMenuManager,
            &ContextMenuManager::showLowerBarMenu);

    QShortcut *deleteTableShortcut = new QShortcut(QKeySequence::Delete, ui->soundsTable);

    deleteTableShortcut->setContext(Qt::WidgetWithChildrenShortcut);

    connect(deleteTableShortcut, &QShortcut::activated, this, [this]()
            {
                QModelIndexList selectedRows = ui->soundsTable->selectionModel()->selectedRows();

                if (selectedRows.isEmpty())
                    return;

                QStringList soundIds;

                for (const QModelIndex &idx : selectedRows)
                    soundIds << idx.data(TableModel::IdRole).toString();

                handleDeleteSounds(soundIds);
            });

    QShortcut *deleteCategoryShortcut = new QShortcut(QKeySequence::Delete, ui->categoriesTree);

    deleteCategoryShortcut->setContext(Qt::WidgetWithChildrenShortcut);

    connect(deleteCategoryShortcut, &QShortcut::activated, this, [this]()
            {
                QModelIndex index = ui->categoriesTree->currentIndex();

                if (!index.isValid())
                    return;

                QString categoryName = index.data(Qt::UserRole).toString();

                if (categoryName.isEmpty())
                    return;

                handleDeleteCategory(categoryName);
            });

    connect(m_contextMenuManager, &ContextMenuManager::addCategoryRequested,
            this, &MainWindow::handleAddCategory);

    connect(m_contextMenuManager, &ContextMenuManager::deleteSoundsRequested,
            this, &MainWindow::handleDeleteSounds);

    connect(m_contextMenuManager,
            &ContextMenuManager::deleteCategoryRequested,
            this,
            &MainWindow::handleDeleteCategory);


    connect(m_contextMenuManager, &ContextMenuManager::addHotkeyRequested,
            this, &MainWindow::handleAddHotkey);

    connect(m_contextMenuManager, &ContextMenuManager::removeHotkeyRequested,
            this, &MainWindow::handleRemoveHotkey);

    connect(ui->soundsTable, &QTableView::doubleClicked, this,
            [this](const QModelIndex &index)
            {
                if (index.column() != 3)
                    return;

                QString soundId = index.data(TableModel::IdRole).toString();
                handleAddHotkey(soundId);
            });

    connect(m_contextMenuManager, &ContextMenuManager::showAllSoundsRequested,
            this, &MainWindow::handleShowAllSounds);
    connect(m_contextMenuManager, &ContextMenuManager::changeCategoryRequested,
            this, &MainWindow::handleChangeCategory);

    // =========================
    // VOLUME
    // =========================

    connect(ui->volumeControl,
            &QSlider::valueChanged,
            this,
            [this](int value)
            {
                audioOutput->setVolume(static_cast<float>(value) / 100.0f);
            });

    // =========================
    // HotKeys
    // =========================

    m_hotkeyManager = new HotkeyManager(this);

    connect(m_hotkeyManager, &HotkeyManager::hotkeyPressed,
        this,
        [this](int id)
        {
            auto categories = Repository::instance().getCategoriesData();

            for (auto it = categories.begin(); it != categories.end(); ++it)
            {
                const auto sounds = it.value();

                for (const auto& sound : sounds)
                {
                    if(sound.id == id) {
                        setWindowTitle(SOUNDPAD + " - " + sound.nameOfMusic);
                        launchMusic(sound.path);
                        return;
                    }
                }
            }
        });

    auto &allSounds = Repository::instance().getCategoriesData()[ALL_SOUNDS];

    if(allSounds.empty()) return;

    for (const auto &sound : allSounds)
    {
        if (!sound.hotkey.isEmpty())
            m_hotkeyManager->registerHotkey(sound.id, sound.hotkey);
    }
}
