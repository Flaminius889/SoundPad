<<<<<<< HEAD
#include "ui/categoryAdd.h"
#include "ui/categoryIcon.h"
#include "ui_categoryAdd.h"
#include <QKeyEvent>
#include <QFile>
#include <QFileDialog>

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

    connect(ui->categoryName, &QTextEdit::textChanged, this, [this]() {
        if (categoryIcon.isEmpty())
            updateIconPreview();
    });

    connect(ui->cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    updateIconPreview();
}

QString CategoryAddDialog::getCategoryName()
{
    return categoryName;
}

QByteArray CategoryAddDialog::getCategoryIcon() const
{
    return categoryIcon;
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

void CategoryAddDialog::on_chooseIconBtn_clicked()
{
    const QString path = QFileDialog::getOpenFileName(
        this, "Choose category icon", {},
        "Images (*.png *.jpg *.jpeg *.bmp *.ico)");
    if (path.isEmpty())
        return;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;

    const QByteArray bytes = file.readAll();
    if (QPixmap pixmap; !pixmap.loadFromData(bytes))
        return;

    categoryIcon = bytes;
    updateIconPreview();
}

void CategoryAddDialog::on_letterIconBtn_clicked()
{
    categoryIcon.clear();
    updateIconPreview();
}

void CategoryAddDialog::updateIconPreview()
{
    ui->iconPreview->setPixmap(createCategoryIcon(
        ui->categoryName->toPlainText(), categoryIcon,
        ui->iconPreview->size()));
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
=======
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
>>>>>>> origin/main
