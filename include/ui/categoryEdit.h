<<<<<<< HEAD
#pragma once

#include <QDialog>
#include <QString>
#include "core/repository.h"
#include "core/Sound.h"
#include "core/consts.h"
#include "ui_categoryEdit.h"

namespace Ui {
class categoryEdit;
}

class categoryEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit categoryEditDialog(QWidget *parent = nullptr);
    ~categoryEditDialog();

    QString getCategoryName();
    QByteArray getCategoryIcon() const;
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void on_okBtn_clicked();
    void on_chooseIconBtn_clicked();
    void on_letterIconBtn_clicked();

private:
    void updateIconPreview();

    Ui::categoryEdit *ui;
    QString categoryName;
    QByteArray categoryIcon;
};
=======
#pragma once

#include <QDialog>
#include <QString>
#include "core/repository.h"
#include "core/Sound.h"
#include "core/consts.h"
#include "ui_categoryEdit.h"

namespace Ui {
class categoryEdit;
}

class categoryEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit categoryEditDialog(QWidget *parent = nullptr);
    ~categoryEditDialog();

    QString getCategoryName();
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void on_okBtn_clicked();

private:
    Ui::categoryEdit *ui;
    QString categoryName;
};
>>>>>>> origin/main
