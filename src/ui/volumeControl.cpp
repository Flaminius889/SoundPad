<<<<<<< HEAD
#include "ui/mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_muteBtn_clicked()
{
    const int volume = ui->volumeControl->value();
    const bool isMuted =
        ui->muteBtn->property("muted").toBool();

    if (!isMuted)
    {
        soundManager->setVolume(0);

        MuteBTNiconPath =
            ":/icons/btn_multimedia/volume_mute.ico";

        ui->muteBtn->setIcon(QIcon(MuteBTNiconPath));
        ui->muteBtn->setIconSize(QSize(24, 24));
        ui->muteBtn->setProperty("muted", true);
    }
    else
    {
        if (volume <= 25)
        {
            MuteBTNiconPath =
                ":/icons/btn_multimedia/25_volume.ico";

            ui->muteBtn->setIconSize(QSize(20, 20));
        }
        else
        {
            MuteBTNiconPath =
                ":/icons/btn_multimedia/max_volume.ico";

            ui->muteBtn->setIconSize(QSize(22, 22));
        }

        ui->muteBtn->setIcon(QIcon(MuteBTNiconPath));

        soundManager->setVolume(volume);

        ui->muteBtn->setProperty("muted", false);
    }
}

void MainWindow::on_volumeControl_valueChanged(int value)
{
    if (!soundManager)
        return;

    soundManager->setVolume(value);

    if (value == 0)
    {
        MuteBTNiconPath =
            ":/icons/btn_multimedia/volume_mute.ico";

        ui->muteBtn->setIconSize(QSize(24, 24));
        ui->muteBtn->setProperty("muted", true);
    }
    else if (value <= 25)
    {
        MuteBTNiconPath =
            ":/icons/btn_multimedia/25_volume.ico";

        ui->muteBtn->setIconSize(QSize(20, 20));
        ui->muteBtn->setProperty("muted", false);
    }
    else
    {
        MuteBTNiconPath =
            ":/icons/btn_multimedia/max_volume.ico";

        ui->muteBtn->setIconSize(QSize(22, 22));
        ui->muteBtn->setProperty("muted", false);
    }

    ui->muteBtn->setIcon(QIcon(MuteBTNiconPath));
}
=======
#include "ui/mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_muteBtn_clicked()
{
    const int volume = ui->volumeControl->value();
    const bool isMuted =
        ui->muteBtn->property("muted").toBool();

    if (!isMuted)
    {
        audioOutput->setVolume(0.0f);

        MuteBTNiconPath =
            ":/icons/btn_multimedia/volume_mute.ico";

        ui->muteBtn->setIcon(QIcon(MuteBTNiconPath));
        ui->muteBtn->setIconSize(QSize(24, 24));
        ui->muteBtn->setProperty("muted", true);
    }
    else
    {
        if (volume <= 25)
        {
            MuteBTNiconPath =
                ":/icons/btn_multimedia/25_volume.ico";

            ui->muteBtn->setIconSize(QSize(20, 20));
        }
        else
        {
            MuteBTNiconPath =
                ":/icons/btn_multimedia/max_volume.ico";

            ui->muteBtn->setIconSize(QSize(22, 22));
        }

        ui->muteBtn->setIcon(QIcon(MuteBTNiconPath));

        audioOutput->setVolume(
            static_cast<float>(volume) / 100.0f);

        ui->muteBtn->setProperty("muted", false);
    }
}

void MainWindow::on_volumeControl_valueChanged(int value)
{
    if (!audioOutput)
        return;

    audioOutput->setVolume(
        static_cast<float>(value) / 100.0f);

    if (value == 0)
    {
        MuteBTNiconPath =
            ":/icons/btn_multimedia/volume_mute.ico";

        ui->muteBtn->setIconSize(QSize(24, 24));
        ui->muteBtn->setProperty("muted", true);
    }
    else if (value <= 25)
    {
        MuteBTNiconPath =
            ":/icons/btn_multimedia/25_volume.ico";

        ui->muteBtn->setIconSize(QSize(20, 20));
        ui->muteBtn->setProperty("muted", false);
    }
    else
    {
        MuteBTNiconPath =
            ":/icons/btn_multimedia/max_volume.ico";

        ui->muteBtn->setIconSize(QSize(22, 22));
        ui->muteBtn->setProperty("muted", false);
    }

    ui->muteBtn->setIcon(QIcon(MuteBTNiconPath));
}
>>>>>>> origin/main
