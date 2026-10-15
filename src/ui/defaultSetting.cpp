<<<<<<< HEAD
#include "ui/mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::setDefaultSettings()
{
    setWindowTitle(SOUNDPAD);

    setWindowIcon(QIcon(":/icons/app48.ico"));

    // Slider
    if (ui->duration_time_slider->value() == 0)
    {
        ui->duration_time_slider->setStyleSheet(m_sliderDisabledStyle);
        ui->duration_time_slider->setEnabled(false);
    }

    // Name of category
    const QString currentCategory =
        Repository::instance().getCurrCategoryData();

    if (currentCategory == ALL_SOUNDS)
        ui->CategoryName->setText(currentCategory);

    // Playback buttons
    ui->playOnHeadset->setIcon(
        QIcon(":/icons/btn_multimedia/headset.ico"));
    ui->playOnHeadset->setIconSize(QSize(20, 20));

    ui->playBtn->setIcon(
        QIcon(":/icons/btn_multimedia/play.ico"));
    ui->playBtn->setIconSize(QSize(16, 16));

    ui->pauseBtn->setIcon(
        QIcon(":/icons/btn_multimedia/pause.ico"));
    ui->pauseBtn->setIconSize(QSize(16, 16));

    ui->stopBtn->setIcon(
        QIcon(":/icons/btn_multimedia/stop.ico"));
    ui->stopBtn->setIconSize(QSize(16, 16));

    ui->repeat_btn->setIcon(
        QIcon(":/icons/btn_multimedia/repeat.ico"));
    ui->repeat_btn->setIconSize(QSize(16, 16));
    ui->repeat_btn->setChecked(soundManager->isRepeating());

    ui->previousBtn->setIcon(
        QIcon(":/icons/btn_multimedia/play_previous.ico"));
    ui->previousBtn->setIconSize(QSize(16, 16));

    ui->nextBtn->setIcon(
        QIcon(":/icons/btn_multimedia/play_next.ico"));
    ui->nextBtn->setIconSize(QSize(16, 16));

    if (ui->volumeControl->value() == 0)
        ui->volumeControl->setValue(25);

    MuteBTNiconPath = ":/icons/btn_multimedia/25_volume.ico";

    ui->muteBtn->setIcon(QIcon(MuteBTNiconPath));
    ui->muteBtn->setIconSize(QSize(20, 20));
    ui->muteBtn->setProperty("muted", false);
}
=======
#include "ui/mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::setDefaultSettings()
{
    setWindowTitle(SOUNDPAD);

    setWindowIcon(QIcon(":/icons/app48.ico"));

    // Slider
    if (ui->duration_time_slider->value() == 0)
    {
        ui->duration_time_slider->setStyleSheet(m_sliderDisabledStyle);
        ui->duration_time_slider->setEnabled(false);
    }

    // Name of category
    const QString currentCategory =
        Repository::instance().getCurrCategoryData();

    if (currentCategory == ALL_SOUNDS)
        ui->CategoryName->setText(currentCategory);

    // Playback buttons
    ui->playOnHeadset->setIcon(
        QIcon(":/icons/btn_multimedia/headset.ico"));
    ui->playOnHeadset->setIconSize(QSize(20, 20));

    ui->playOnMic->setIcon(
        QIcon(":/icons/btn_multimedia/mic.ico"));
    ui->playOnMic->setIconSize(QSize(20, 20));

    ui->playBtn->setIcon(
        QIcon(":/icons/btn_multimedia/play.ico"));
    ui->playBtn->setIconSize(QSize(16, 16));

    ui->pauseBtn->setIcon(
        QIcon(":/icons/btn_multimedia/pause.ico"));
    ui->pauseBtn->setIconSize(QSize(16, 16));

    ui->stopBtn->setIcon(
        QIcon(":/icons/btn_multimedia/stop.ico"));
    ui->stopBtn->setIconSize(QSize(16, 16));

    ui->previousBtn->setIcon(
        QIcon(":/icons/btn_multimedia/play_previous.ico"));
    ui->previousBtn->setIconSize(QSize(16, 16));

    ui->nextBtn->setIcon(
        QIcon(":/icons/btn_multimedia/play_next.ico"));
    ui->nextBtn->setIconSize(QSize(16, 16));

    if (ui->volumeControl->value() == 0)
        ui->volumeControl->setValue(25);

    MuteBTNiconPath = ":/icons/btn_multimedia/25_volume.ico";

    ui->muteBtn->setIcon(QIcon(MuteBTNiconPath));
    ui->muteBtn->setIconSize(QSize(20, 20));
    ui->muteBtn->setProperty("muted", false);
}
>>>>>>> origin/main
