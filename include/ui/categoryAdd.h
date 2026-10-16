#ifndef CATEGORYADD_H
#define CATEGORYADD_H

#include <QDialog>
#include <QString>

namespace Ui {
    class categoryAdd;
}

class CategoryAddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CategoryAddDialog(QWidget *parent = nullptr);
    ~CategoryAddDialog();

    QString getCategoryName();
    QByteArray getCategoryIcon() const;
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void on_okBtn_clicked();
    void on_chooseIconBtn_clicked();
    void on_letterIconBtn_clicked();

private:
    void updateIconPreview();

    Ui::categoryAdd *ui;
    QString categoryName;
    QByteArray categoryIcon;
};

#endif // CATEGORYADD_H
#ifndef CATEGORYADD_H
#define CATEGORYADD_H

#include <QDialog>
#include <QString>

namespace Ui {
    class categoryAdd;
}

class CategoryAddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CategoryAddDialog(QWidget *parent = nullptr);
    ~CategoryAddDialog();

    QString getCategoryName();
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void on_okBtn_clicked();

private:
    Ui::categoryAdd *ui;
    QString categoryName;
};

#endif // CATEGORYADD_H
