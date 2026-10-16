#include "ui/mainwindow.h"

void MainWindow::setupContextMenus()
{
    m_contextMenuManager = new ContextMenuManager(ui, this);

    ui->lower_bar->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->categoriesTree->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->soundsTable->setContextMenuPolicy(Qt::CustomContextMenu);
}
#include "ui/mainwindow.h"

void MainWindow::setupContextMenus()
{
    m_contextMenuManager = new ContextMenuManager(ui, this);

    ui->lower_bar->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->categoriesTree->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->soundsTable->setContextMenuPolicy(Qt::CustomContextMenu);
}
