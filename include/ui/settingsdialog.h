#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QAudioDevice>
#include <QMediaDevices>
#include <QFile>

namespace Ui {
class settingsDialog;
}

class settingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit settingsDialog(QWidget *parent = nullptr);
    ~settingsDialog();

    void fillOutputComboBox();
    void fillInputComboBox();

    QAudioDevice getOutputDevice();
    QAudioDevice getInputDevice();
    void setDevices(const QAudioDevice &output, const QAudioDevice &input);

    void onOutputDeviceChanged(int index);
    void onInputDeviceChanged(int index);
private slots:
    void on_okBtn_clicked();

    void on_cancelBtn_clicked();

private:
    QAudioDevice OutputDevice;
    QAudioDevice InputDevice;


    Ui::settingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QAudioDevice>
#include <QMediaPlayer>
#include <QMediaDevices>
#include <QAudioOutput>
#include <QAudioInput>
#include <QFile>

Q_DECLARE_METATYPE(QAudioDevice)

namespace Ui {
class settingsDialog;
}

class settingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit settingsDialog(QWidget *parent = nullptr);
    ~settingsDialog();

    void fillOutputComboBox();
    void fillInputComboBox();

    QAudioDevice getOutputDevice();
    QAudioDevice getInputDevice();

    void onOutputDeviceChanged(int index);
    void onInputDeviceChanged(int index);
private slots:
    void on_okBtn_clicked();

    void on_cancelBtn_clicked();

private:
    QAudioDevice OutputDevice;
    QAudioDevice InputDevice;


    Ui::settingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
