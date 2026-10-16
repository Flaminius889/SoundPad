#include "core/contextManager.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include <QAction>
#include <QModelIndex>

ContextMenuManager::ContextMenuManager(Ui::MainWindow *ui, QObject *parent)
    : QObject(parent)
    , m_ui(ui)
{
}

void ContextMenuManager::showTableMenu(const QPoint &pos)
{
    QModelIndex index = m_ui->soundsTable->indexAt(pos);

    if (index.column() != 3) {

        if (!index.isValid())
            return;

        QItemSelectionModel *selection = m_ui->soundsTable->selectionModel();

        if (!selection->isRowSelected(index.row(), QModelIndex()))
        {
            selection->clearSelection();

            selection->select(
                index,
                QItemSelectionModel::Select |
                    QItemSelectionModel::Rows);
        }

        QModelIndexList selectedRows = m_ui->soundsTable->selectionModel()->selectedRows();

        if (selectedRows.isEmpty())
            return;

        QMenu contextMenu;

        QString deleteText = (selectedRows.size() == 1) ? "Delete" :
                                 QString("Delete (%1)").arg(selectedRows.size());

        QAction *deleteAction = contextMenu.addAction(deleteText);

        QAction *selectedAction = contextMenu.exec(m_ui->soundsTable->mapToGlobal(pos));

        if (selectedAction == deleteAction)
        {
            QStringList soundIds;
            for (const QModelIndex &idx : selectedRows)
            {
                QModelIndex idIndex = idx.sibling(idx.row(), 0);
                soundIds.append(idx.data(TableModel::IdRole).toString());;
            }
            emit deleteSoundsRequested(soundIds);
        }
    } else if (index.column() == 3){

        QItemSelectionModel *selection = m_ui->soundsTable->selectionModel();

        if (!selection->isRowSelected(index.row(), QModelIndex()))
        {
            selection->clearSelection();

            selection->select(
                index,
                QItemSelectionModel::Select |
                    QItemSelectionModel::Rows);
        }

        QModelIndexList selectedRows = m_ui->soundsTable->selectionModel()->selectedRows();

        if (selectedRows.isEmpty())
            return;

        QMenu contextMenu;

        QString deleteText = (selectedRows.size() == 1) ? "Delete" :
                                 QString("Delete (%1)").arg(selectedRows.size());

        QAction *setHotkeyAction = nullptr;
        QAction *removeHotkeyAction = nullptr;

        if (selectedRows.size() == 1)
        {
            QString soundId = selectedRows.first().data(TableModel::IdRole).toString();
            bool ok = false;
            int id = soundId.toInt(&ok);

            setHotkeyAction = contextMenu.addAction("Set hotkey");

            if (ok && !Repository::instance().getSoundHotkey(id).isEmpty())
                removeHotkeyAction = contextMenu.addAction("Remove hotkey");
        }
        QAction *deleteAction = contextMenu.addAction(deleteText);

        QAction *selectedAction = contextMenu.exec(m_ui->soundsTable->mapToGlobal(pos));

        if (setHotkeyAction && selectedAction == setHotkeyAction)
        {
            QString soundId = selectedRows.first().data(TableModel::IdRole).toString();
            emit addHotkeyRequested(soundId);
        }
        else if (removeHotkeyAction && selectedAction == removeHotkeyAction)
        {
            QString soundId = selectedRows.first().data(TableModel::IdRole).toString();
            emit removeHotkeyRequested(soundId);
        }
        else if (selectedAction == deleteAction)
        {
            QStringList soundIds;
            for (const QModelIndex &idx : selectedRows)
                soundIds.append(idx.data(TableModel::IdRole).toString());

            emit deleteSoundsRequested(soundIds);
        }
    }
}

void ContextMenuManager::showTreeMenu(const QPoint &pos)
{
    QModelIndex index = m_ui->categoriesTree->indexAt(pos);

    QMenu menu;

    QAction *addCategoryAct = menu.addAction("Add category");
    QAction *chooseAllSounds = menu.addAction("All Sounds");

    QAction *deleteCategoryAct = nullptr;
    QAction *editCategoryAct = nullptr;

    if (index.isValid())
    {
        menu.addSeparator();
        editCategoryAct = menu.addAction("Edit category");
        if (index.data(Qt::UserRole).toString() != ALL_SOUNDS)
            deleteCategoryAct = menu.addAction("Delete category");
    }

    QAction *selectedAction =
        menu.exec(m_ui->categoriesTree->mapToGlobal(pos));

    if (selectedAction == addCategoryAct)
    {
        emit addCategoryRequested();
    }
    else if (selectedAction == chooseAllSounds)
    {
        emit showAllSoundsRequested();
    }
    else if (selectedAction == deleteCategoryAct)
    {
        emit deleteCategoryRequested(index.data().toString());
    }
    else if (selectedAction == editCategoryAct)
    {
        emit changeCategoryRequested();
    }
}

void ContextMenuManager::showLowerBarMenu(const QPoint &pos)
{
    QMenu menu;

    QAction *changeCategoryAct = menu.addAction("Change category");

    QAction *selectedAction = menu.exec(m_ui->lower_bar->mapToGlobal(pos));

    if (selectedAction == changeCategoryAct)
    {
        emit changeCategoryRequested();
    }
}
#include "core/contextManager.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include <QAction>
#include <QModelIndex>

ContextMenuManager::ContextMenuManager(Ui::MainWindow *ui, QObject *parent)
    : QObject(parent)
    , m_ui(ui)
{
}

void ContextMenuManager::showTableMenu(const QPoint &pos)
{
    QModelIndex index = m_ui->soundsTable->indexAt(pos);

    if (index.column() != 3) {

        if (!index.isValid())
            return;

        QItemSelectionModel *selection = m_ui->soundsTable->selectionModel();

        if (!selection->isRowSelected(index.row(), QModelIndex()))
        {
            selection->clearSelection();

            selection->select(
                index,
                QItemSelectionModel::Select |
                    QItemSelectionModel::Rows);
        }

        QModelIndexList selectedRows = m_ui->soundsTable->selectionModel()->selectedRows();

        if (selectedRows.isEmpty())
            return;

        QMenu contextMenu;

        QString deleteText = (selectedRows.size() == 1) ? "Delete" :
                                 QString("Delete (%1)").arg(selectedRows.size());

        QAction *deleteAction = contextMenu.addAction(deleteText);

        QAction *selectedAction = contextMenu.exec(m_ui->soundsTable->mapToGlobal(pos));

        if (selectedAction == deleteAction)
        {
            QStringList soundIds;
            for (const QModelIndex &idx : selectedRows)
            {
                QModelIndex idIndex = idx.sibling(idx.row(), 0);
                soundIds.append(idx.data(TableModel::IdRole).toString());;
            }
            emit deleteSoundsRequested(soundIds);
        }
    } else if (index.column() == 3){

        QItemSelectionModel *selection = m_ui->soundsTable->selectionModel();

        if (!selection->isRowSelected(index.row(), QModelIndex()))
        {
            selection->clearSelection();

            selection->select(
                index,
                QItemSelectionModel::Select |
                    QItemSelectionModel::Rows);
        }

        QModelIndexList selectedRows = m_ui->soundsTable->selectionModel()->selectedRows();

        if (selectedRows.isEmpty())
            return;

        QMenu contextMenu;

        QString deleteText = (selectedRows.size() == 1) ? "Delete" :
                                 QString("Delete (%1)").arg(selectedRows.size());

        QAction *setHotkeyAction = nullptr;
        QAction *removeHotkeyAction = nullptr;

        if (selectedRows.size() == 1)
        {
            QString soundId = selectedRows.first().data(TableModel::IdRole).toString();
            bool ok = false;
            int id = soundId.toInt(&ok);

            setHotkeyAction = contextMenu.addAction("Set hotkey");

            if (ok && !Repository::instance().getSoundHotkey(id).isEmpty())
                removeHotkeyAction = contextMenu.addAction("Remove hotkey");
        }
        QAction *deleteAction = contextMenu.addAction(deleteText);

        QAction *selectedAction = contextMenu.exec(m_ui->soundsTable->mapToGlobal(pos));

        if (setHotkeyAction && selectedAction == setHotkeyAction)
        {
            QString soundId = selectedRows.first().data(TableModel::IdRole).toString();
            emit addHotkeyRequested(soundId);
        }
        else if (removeHotkeyAction && selectedAction == removeHotkeyAction)
        {
            QString soundId = selectedRows.first().data(TableModel::IdRole).toString();
            emit removeHotkeyRequested(soundId);
        }
        else if (selectedAction == deleteAction)
        {
            QStringList soundIds;
            for (const QModelIndex &idx : selectedRows)
                soundIds.append(idx.data(TableModel::IdRole).toString());

            emit deleteSoundsRequested(soundIds);
        }
    }
}

void ContextMenuManager::showTreeMenu(const QPoint &pos)
{
    QModelIndex index = m_ui->categoriesTree->indexAt(pos);

    QMenu menu;

    QAction *addCategoryAct = menu.addAction("Add category");
    QAction *chooseAllSounds = menu.addAction("All Sounds");

    QAction *deleteCategoryAct = nullptr;

    if (index.isValid())
    {
        menu.addSeparator();
        deleteCategoryAct = menu.addAction("Delete category");
    }

    QAction *selectedAction =
        menu.exec(m_ui->categoriesTree->mapToGlobal(pos));

    if (selectedAction == addCategoryAct)
    {
        emit addCategoryRequested();
    }
    else if (selectedAction == chooseAllSounds)
    {
        emit showAllSoundsRequested();
    }
    else if (selectedAction == deleteCategoryAct)
    {
        emit deleteCategoryRequested(index.data().toString());
    }
}

void ContextMenuManager::showLowerBarMenu(const QPoint &pos)
{
    QMenu menu;

    QAction *changeCategoryAct = menu.addAction("Change category");

    QAction *selectedAction = menu.exec(m_ui->lower_bar->mapToGlobal(pos));

    if (selectedAction == changeCategoryAct)
    {
        emit changeCategoryRequested();
    }
}
