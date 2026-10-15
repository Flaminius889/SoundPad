<<<<<<< HEAD
#include "ui/mainwindow.h"
#include <QStyledItemDelegate>

void MainWindow::setupTableView()
{
    ui->soundsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->soundsTable->verticalHeader()->hide();

    ui->soundsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->soundsTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->soundsTable->setAlternatingRowColors(true);
    ui->soundsTable->setShowGrid(false);
    ui->soundsTable->setFocusPolicy(Qt::StrongFocus);

    auto *header = ui->soundsTable->horizontalHeader();

    header->setStretchLastSection(true);
    header->setHighlightSections(false);
    header->setSectionsClickable(true);

    header->setMouseTracking(true);
    header->viewport()->setMouseTracking(true);
    header->setAttribute(Qt::WA_Hover, true);
    header->viewport()->setAttribute(Qt::WA_Hover, true);

    ui->soundsTable->setSortingEnabled(true);
    header->setSortIndicatorShown(true);
}
=======
#include "ui/mainwindow.h"

void MainWindow::setupTableView()
{
    ui->soundsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->soundsTable->verticalHeader()->hide();

    ui->soundsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->soundsTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->soundsTable->setAlternatingRowColors(true);
    ui->soundsTable->setShowGrid(false);
    ui->soundsTable->setFocusPolicy(Qt::StrongFocus);

    auto *header = ui->soundsTable->horizontalHeader();

    header->setStretchLastSection(true);
    header->setHighlightSections(false);
    header->setSectionsClickable(true);

    header->setMouseTracking(true);
    header->viewport()->setMouseTracking(true);
    header->setAttribute(Qt::WA_Hover, true);
    header->viewport()->setAttribute(Qt::WA_Hover, true);

    ui->soundsTable->setSortingEnabled(true);
    header->setSortIndicatorShown(true);
}
>>>>>>> origin/main
