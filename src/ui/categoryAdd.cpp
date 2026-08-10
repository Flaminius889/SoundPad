#include "ui/categoryAdd.h"
#include "ui_categoryAdd.h"
#include <QKeyEvent>
#include <QFile>

CategoryAddDialog::CategoryAddDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::categoryAdd)
{
    ui->setupUi(this);
    setWindowTitle("Add category");

    QFile file(":/styles.qss");

    if (file.open(QFile::ReadOnly))
    {
        qApp->setStyleSheet(file.readAll());
        file.close();
    }

    ui->categoryName->installEventFilter(this);

    connect(ui->cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

QString CategoryAddDialog::getCategoryName()
{
    return categoryName;
}

CategoryAddDialog::~CategoryAddDialog()
{
    delete ui;
}

void CategoryAddDialog::on_okBtn_clicked()
{
    categoryName = ui->categoryName->toPlainText().trimmed();

    accept();
}

bool CategoryAddDialog::eventFilter(QObject *obj, QEvent *event)
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