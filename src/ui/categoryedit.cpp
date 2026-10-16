#include "ui/categoryEdit.h"
#include "ui/categoryIcon.h"
#include <QKeyEvent>
#include <QPushButton>
#include <QFile>
#include <QFileDialog>


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

    if (const SoundCategory *category =
            Repository::instance().categoryByName(currentCategory))
    {
        categoryIcon = category->icon;
    }

    if(currentCategory == ALL_SOUNDS) {
        ui->categoryName->setText(currentCategory);
        ui->categoryName->setDisabled(true);
    }
    else {
        ui->categoryName->setText(currentCategory);
        ui->categoryName->setEnabled(true);
    }

    ui->categoryName->installEventFilter(this);

    connect(ui->categoryName, &QTextEdit::textChanged, this, [this]() {
        if (categoryIcon.isEmpty())
            updateIconPreview();
    });

    connect(ui->cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    updateIconPreview();
}

QString categoryEditDialog::getCategoryName()
{
    return categoryName;
}

QByteArray categoryEditDialog::getCategoryIcon() const
{
    return categoryIcon;
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

void categoryEditDialog::on_chooseIconBtn_clicked()
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

void categoryEditDialog::on_letterIconBtn_clicked()
{
    categoryIcon.clear();
    updateIconPreview();
}

void categoryEditDialog::updateIconPreview()
{
    ui->iconPreview->setPixmap(createCategoryIcon(
        ui->categoryName->toPlainText(), categoryIcon,
        ui->iconPreview->size()));
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
