#include "ui/mainwindow.h"

void MainWindow::createModels()
{
    // Models share the repository's QVector<SoundCategory> storage.
    tableModel = new TableModel(this);
    categoryModel = new CategoryModel(this);

    Repository::instance().ReadData();

    tableModel->setCategories(
        &Repository::instance().getCategoriesData());

    tableModel->setCurrentCategory(
        Repository::instance().getCurrCategoryData());

    ui->soundsTable->setModel(tableModel);

    ui->categoriesTree->setModel(categoryModel);


    ui->CategoryName->setText(
        Repository::instance().getCurrCategoryData());

    updateCountLabel(
        Repository::instance().getCurrCategoryData());
}
#include "ui/mainwindow.h"

void MainWindow::createModels()
{
    tableModel = new TableModel(this);
    categoryModel = new CategoryModel(this);

    Repository::instance().ReadData();

    tableModel->setCategories(
        &Repository::instance().getCategoriesData());

    tableModel->setCurrentCategory(
        Repository::instance().getCurrCategoryData());

    ui->soundsTable->setModel(tableModel);

    ui->categoriesTree->setModel(categoryModel);


    ui->CategoryName->setText(
        Repository::instance().getCurrCategoryData());

    updateCountLabel(
        Repository::instance().getCurrCategoryData());
}
