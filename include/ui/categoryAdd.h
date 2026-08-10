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
