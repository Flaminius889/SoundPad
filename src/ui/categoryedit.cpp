#include "ui/categoryEdit.h"
#include <QKeyEvent>
#include <QPushButton>
#include <QFile>


categoryEditDialog::categoryEditDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::categoryEdit)
{
    ui->setupUi(this);
    setWindowTitle("Edit category");

    QFile file(":/styles.qss");

    if (file.open(QFile::ReadOnly))
    {
        qApp->setStyleSheet(file.readAll());
        file.close();
    }

    QString currentCategory = Repository::instance().getCurrCategoryData();

    if(currentCategory == ALL_SOUNDS) {
        ui->categoryName->setText(currentCategory);
        ui->categoryName->setDisabled(true);
    }
    else {
        ui->categoryName->setText(currentCategory);
        ui->categoryName->setEnabled(true);
    }

    ui->categoryName->installEventFilter(this);

    connect(ui->cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

QString categoryEditDialog::getCategoryName()
{
    return categoryName;
}

categoryEditDialog::~categoryEditDialog()
{
    delete ui;
}

void categoryEditDialog::on_okBtn_clicked()
{
    categoryName = ui->categoryName->toPlainText().trimmed();

    accept();
}

bool categoryEditDialog::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->categoryName && event->type() == QEvent::KeyPress)
    {
        auto *keyEvent = static_cast<QKeyEvent*>(event);

        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
        {
            on_okBtn_clicked();
            return true;
        }
    }

    return QDialog::eventFilter(obj, event);
}