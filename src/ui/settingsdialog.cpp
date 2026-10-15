<<<<<<< HEAD
#include "ui/settingsdialog.h"
#include "ui_settingsdialog.h"

settingsDialog::settingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::settingsDialog)
{
    ui->setupUi(this);

    setWindowTitle("Settings");

    QFile file(":/styles.qss");

    if (file.open(QFile::ReadOnly))
    {
        qApp->setStyleSheet(file.readAll());
        file.close();
    }

    fillOutputComboBox();
    fillInputComboBox();

    connect(ui->outputComboBox,
            &QComboBox::currentIndexChanged,
            this,
            &settingsDialog::onOutputDeviceChanged);

    connect(ui->inputComboBox,
            &QComboBox::currentIndexChanged,
            this,
            &settingsDialog::onInputDeviceChanged);
}

settingsDialog::~settingsDialog()
{
    delete ui;
}

void settingsDialog::on_okBtn_clicked()
{
    accept();
}

void settingsDialog::on_cancelBtn_clicked()
{
    reject();
}

void settingsDialog::fillOutputComboBox()
{
    const auto devices = QMediaDevices::audioOutputs();

    ui->outputComboBox->clear();

    ui->outputComboBox->addItem("By default", QVariant());

    QAudioDevice defaultDevice = QMediaDevices::defaultAudioOutput();

    for (const QAudioDevice &device : devices)
    {
        ui->outputComboBox->addItem(device.description(),
                                    QVariant::fromValue(device));
    }
}

void settingsDialog::fillInputComboBox()
{
    const auto devices = QMediaDevices::audioInputs();

    ui->inputComboBox->clear();

    ui->inputComboBox->addItem("By default", QVariant());

    QAudioDevice defaultDevice = QMediaDevices::defaultAudioInput();

    for (const QAudioDevice &device : devices)
    {
        ui->inputComboBox->addItem(device.description(),
                                   QVariant::fromValue(device));
    }
}

QAudioDevice settingsDialog::getOutputDevice()
{
    return OutputDevice;
}

QAudioDevice settingsDialog::getInputDevice()
{
    return InputDevice;
}

void settingsDialog::setDevices(const QAudioDevice &output, const QAudioDevice &input)
{
    const auto selectDevice = [](QComboBox *combo, const QAudioDevice &device) {
        for (int i = 1; i < combo->count(); ++i) {
            if (combo->itemData(i).value<QAudioDevice>().id() == device.id()) {
                combo->setCurrentIndex(i);
                return;
            }
        }
        combo->setCurrentIndex(0);
    };
    selectDevice(ui->outputComboBox, output);
    selectDevice(ui->inputComboBox, input);
    OutputDevice = ui->outputComboBox->currentData().value<QAudioDevice>();
    InputDevice = ui->inputComboBox->currentData().value<QAudioDevice>();
}

void settingsDialog::onOutputDeviceChanged(int index)
{
    if (index < 0)
        return;

    OutputDevice =
        ui->outputComboBox->itemData(index).value<QAudioDevice>();

}
void settingsDialog::onInputDeviceChanged(int index)
{
    if (index < 0)
        return;

    InputDevice =
        ui->inputComboBox->itemData(index).value<QAudioDevice>();
}

=======
#include "ui/settingsdialog.h"
#include "ui_settingsdialog.h"

settingsDialog::settingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::settingsDialog)
{
    ui->setupUi(this);

    setWindowTitle("Settings");

    QFile file(":/styles.qss");

    if (file.open(QFile::ReadOnly))
    {
        qApp->setStyleSheet(file.readAll());
        file.close();
    }

    fillOutputComboBox();
    fillInputComboBox();

    connect(ui->outputComboBox,
            &QComboBox::currentIndexChanged,
            this,
            &settingsDialog::onOutputDeviceChanged);

    connect(ui->inputComboBox,
            &QComboBox::currentIndexChanged,
            this,
            &settingsDialog::onInputDeviceChanged);
}

settingsDialog::~settingsDialog()
{
    delete ui;
}

void settingsDialog::on_okBtn_clicked()
{
    accept();
}

void settingsDialog::on_cancelBtn_clicked()
{
    reject();
}

void settingsDialog::fillOutputComboBox()
{
    const auto devices = QMediaDevices::audioOutputs();

    ui->outputComboBox->clear();

    ui->outputComboBox->addItem("By default", QVariant());

    QAudioDevice defaultDevice = QMediaDevices::defaultAudioOutput();

    for (const QAudioDevice &device : devices)
    {
        ui->outputComboBox->addItem(device.description(),
                                    QVariant::fromValue(device));
    }
}

void settingsDialog::fillInputComboBox()
{
    const auto devices = QMediaDevices::audioInputs();

    ui->inputComboBox->clear();

    ui->inputComboBox->addItem("By default", QVariant());

    QAudioDevice defaultDevice = QMediaDevices::defaultAudioInput();

    for (const QAudioDevice &device : devices)
    {
        if (device == defaultDevice)
            continue;

        ui->inputComboBox->addItem(device.description(),
                                   QVariant::fromValue(device));
    }
}

QAudioDevice settingsDialog::getOutputDevice()
{
    return OutputDevice;
}

QAudioDevice settingsDialog::getInputDevice()
{
    return InputDevice;
}

void settingsDialog::onOutputDeviceChanged(int index)
{
    if (index < 0)
        return;

    OutputDevice =
        ui->outputComboBox->itemData(index).value<QAudioDevice>();

}
void settingsDialog::onInputDeviceChanged(int index)
{
    if (index < 0)
        return;

    InputDevice =
        ui->inputComboBox->itemData(index).value<QAudioDevice>();
}

>>>>>>> origin/main
