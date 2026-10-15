<<<<<<< HEAD
#include "ui/widgets/soundstableview.h"

SoundsTableView::SoundsTableView(QWidget *parent)
    : QTableView(parent)
{
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void SoundsTableView::mousePressEvent(QMouseEvent *event)
{
    QModelIndex index = indexAt(event->pos());

    if (event->button() == Qt::RightButton)
    {
        if (!index.isValid())
            return;

        auto *selection = selectionModel();

        if (!selection->isRowSelected(index.row(), QModelIndex()))
        {
            selection->select(
                index,
                QItemSelectionModel::ClearAndSelect |
                    QItemSelectionModel::Rows);
        }

        event->accept();
        return;
    }

    QTableView::mousePressEvent(event);
}

void SoundsTableView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QTableView::mouseDoubleClickEvent(event);
=======
#include "ui/widgets/soundstableview.h"

SoundsTableView::SoundsTableView(QWidget *parent)
    : QTableView(parent)
{
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void SoundsTableView::mousePressEvent(QMouseEvent *event)
{
    QModelIndex index = indexAt(event->pos());

    if (event->button() == Qt::RightButton)
    {
        if (!index.isValid())
            return;

        auto *selection = selectionModel();

        if (!selection->isRowSelected(index.row(), QModelIndex()))
        {
            selection->select(
                index,
                QItemSelectionModel::ClearAndSelect |
                    QItemSelectionModel::Rows);
        }

        event->accept();
        return;
    }

    QTableView::mousePressEvent(event);
}

void SoundsTableView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QTableView::mouseDoubleClickEvent(event);
>>>>>>> origin/main
}