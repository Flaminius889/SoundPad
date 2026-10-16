#ifndef ADDHOTKEY_H
#define ADDHOTKEY_H

#include <QDialog>
#include <QKeySequence>
#include <QKeyEvent>

namespace Ui {
class addHotkey;
}

class addHotkey : public QDialog
{
    Q_OBJECT

public:
    explicit addHotkey(int soundId, QWidget *parent = nullptr);
    ~addHotkey();

    int soundId() const;

    QKeySequence keySequence() const;

private slots:
    void on_ok_btn_clicked();
    bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::addHotkey *ui;
    int m_soundId;

    QKeySequence keySeque;
};

#ifndef ADDHOTKEY_H
#define ADDHOTKEY_H

#include <QDialog>
#include <QKeySequence>
#include <QKeyEvent>

namespace Ui {
class addHotkey;
}

class addHotkey : public QDialog
{
    Q_OBJECT

public:
    explicit addHotkey(int soundId, QWidget *parent = nullptr);
    ~addHotkey();

    int soundId() const;

    QKeySequence keySequence() const;

private slots:
    void on_ok_btn_clicked();
    bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::addHotkey *ui;
    int m_soundId;

    QKeySequence keySeque;
};

#endif // ADDHOTKEY_H