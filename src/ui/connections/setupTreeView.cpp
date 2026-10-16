#include "ui/mainwindow.h"

void MainWindow::setupTreeView()
{
    QFont font("Segoe UI", 12);

    ui->categoriesTree->setFont(font);
    ui->categoriesTree->setIconSize(QSize(20, 20));

    ui->categoriesTree->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->categoriesTree->setSelectionBehavior(QAbstractItemView::SelectRows);

    auto header = ui->categoriesTree->header();

    header->hide();

    header->setStretchLastSection(true);

    header->setSectionResizeMode(0, QHeaderView::Stretch);
    header->setSectionResizeMode(1, QHeaderView::Fixed);

    header->resizeSection(1, 40);

    ui->categoriesTree->setHeaderHidden(true);
    ui->categoriesTree->setRootIsDecorated(false);
    ui->categoriesTree->setIndentation(0);

    ui->categoriesTree->setUniformRowHeights(true);
    ui->categoriesTree->setAnimated(true);
    ui->categoriesTree->setAllColumnsShowFocus(false);

    ui->categoriesTree->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->categoriesTree->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->categoriesTree->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->categoriesTree->setExpandsOnDoubleClick(false);

    ui->categoriesTree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->categoriesTree->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    ui->categoriesTree->expandAll();
}
#include "ui/mainwindow.h"

void MainWindow::setupTreeView()
{
    QFont font("Segoe UI", 12);

    ui->categoriesTree->setFont(font);

    ui->categoriesTree->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->categoriesTree->setSelectionBehavior(QAbstractItemView::SelectRows);

    auto header = ui->categoriesTree->header();

    header->hide();

    header->setStretchLastSection(true);

    header->setSectionResizeMode(0, QHeaderView::Stretch);
    header->setSectionResizeMode(1, QHeaderView::Fixed);

    header->resizeSection(1, 40);

    ui->categoriesTree->setHeaderHidden(true);
    ui->categoriesTree->setRootIsDecorated(false);
    ui->categoriesTree->setIndentation(0);

    ui->categoriesTree->setUniformRowHeights(true);
    ui->categoriesTree->setAnimated(true);
    ui->categoriesTree->setAllColumnsShowFocus(false);

    ui->categoriesTree->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->categoriesTree->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->categoriesTree->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->categoriesTree->setExpandsOnDoubleClick(false);

    ui->categoriesTree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->categoriesTree->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    ui->categoriesTree->expandAll();
}
