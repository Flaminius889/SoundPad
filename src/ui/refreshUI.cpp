#include "ui/mainwindow.h"

void MainWindow::refreshAllModels()
{
    categoryModel->refreshModel();

    tableModel->refreshModel();

    ui->categoriesTree->expandAll();

    ui->CategoryName->setText(Repository::instance().getCurrCategoryData());
    updateCountLabel(Repository::instance().getCurrCategoryData());
}

void MainWindow::updateCountLabel(const QString& category)
{
    int count = Repository::instance().getCategoriesData()[category].size();
    ui->countOfMusic->setText(QString("Count: %1").arg(count));
}