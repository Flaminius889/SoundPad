<<<<<<< HEAD
#include "ui/addhotkey.h"
#include "ui_addhotkey.h"

addHotkey::addHotkey(int soundId, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addHotkey)
    , m_soundId(soundId)
{
    ui->setupUi(this);

    setWindowTitle("Add/Change hotkey");

    ui->keySequenceEdit->setMaximumSequenceLength(1);

    ui->keySequenceEdit->installEventFilter(this);
    connect(ui->cancel_btn, &QPushButton::clicked, this, &QDialog::reject);
}

addHotkey::~addHotkey()
{
    delete ui;
}

int addHotkey::soundId() const
{
    return m_soundId;
}

QKeySequence addHotkey::keySequence() const
{
    return keySeque;
}

void addHotkey::on_ok_btn_clicked()
{
    keySeque = ui->keySequenceEdit->keySequence();

    accept();
}

bool addHotkey::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->keySequenceEdit &&
        event->type() == QEvent::KeyPress)
    {
        auto *keyEvent = static_cast<QKeyEvent *>(event);

        int key = keyEvent->key();

        if (key == Qt::Key_Return || key == Qt::Key_Enter)
        {
            on_ok_btn_clicked();
            return true;
        }

        if (key == Qt::Key_Shift ||
            key == Qt::Key_Control ||
            key == Qt::Key_Alt ||
            key == Qt::Key_Meta)
        {
            return true;
        }

        QKeyCombination combination(
            keyEvent->modifiers(),
            static_cast<Qt::Key>(key)
            );

        ui->keySequenceEdit->setKeySequence(
            QKeySequence(combination)
            );

        return true;
    }

    return QDialog::eventFilter(obj, event);
=======
#include "ui/addhotkey.h"
#include "ui_addhotkey.h"

addHotkey::addHotkey(int soundId, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addHotkey)
    , m_soundId(soundId)
{
    ui->setupUi(this);

    setWindowTitle("Add/Change hotkey");

    ui->keySequenceEdit->setMaximumSequenceLength(1);

    ui->keySequenceEdit->installEventFilter(this);
    connect(ui->cancel_btn, &QPushButton::clicked, this, &QDialog::reject);
}

addHotkey::~addHotkey()
{
    delete ui;
}

int addHotkey::soundId() const
{
    return m_soundId;
}

QKeySequence addHotkey::keySequence() const
{
    return keySeque;
}

void addHotkey::on_ok_btn_clicked()
{
    keySeque = ui->keySequenceEdit->keySequence();

    accept();
}

bool addHotkey::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->keySequenceEdit &&
        event->type() == QEvent::KeyPress)
    {
        auto *keyEvent = static_cast<QKeyEvent *>(event);

        int key = keyEvent->key();

        if (key == Qt::Key_Return || key == Qt::Key_Enter)
        {
            on_ok_btn_clicked();
            return true;
        }

        if (key == Qt::Key_Shift ||
            key == Qt::Key_Control ||
            key == Qt::Key_Alt ||
            key == Qt::Key_Meta)
        {
            return true;
        }

        QKeyCombination combination(
            keyEvent->modifiers(),
            static_cast<Qt::Key>(key)
            );

        ui->keySequenceEdit->setKeySequence(
            QKeySequence(combination)
            );

        return true;
    }

    return QDialog::eventFilter(obj, event);
>>>>>>> origin/main
}