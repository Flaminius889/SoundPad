#include "ui/widgets/categoriesView.h"

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QPainter>

categoriesView::categoriesView(QWidget *parent)
    : QTreeView(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
}

void categoriesView::dragEnterEvent(QDragEnterEvent *event)
{
    QTreeView::dragEnterEvent(event);
}

void categoriesView::dragMoveEvent(QDragMoveEvent *event)
{
    QModelIndex index =
        indexAt(event->position().toPoint());

    if (index != m_dropIndex)
    {
        m_dropIndex = index;
        viewport()->update();
    }

    QTreeView::dragMoveEvent(event);
}

void categoriesView::dragLeaveEvent(QDragLeaveEvent *event)
{
    m_dropIndex = QPersistentModelIndex();

    viewport()->update();

    QTreeView::dragLeaveEvent(event);
}

void categoriesView::dropEvent(QDropEvent *event)
{
    m_dropIndex = QPersistentModelIndex();

    QTreeView::dropEvent(event);

    viewport()->update();
}

void categoriesView::paintEvent(QPaintEvent *event)
{
    QTreeView::paintEvent(event);

    if (!m_dropIndex.isValid())
        return;

    QPainter painter(viewport());

    QRect rect = visualRect(m_dropIndex);

    rect.setLeft(0);
    rect.setRight(viewport()->width());

    painter.setPen(
        QPen(QColor(0, 0, 0), 3)
        );

    painter.drawRect(
        rect.adjusted(1, 4, -2, 1)
        );
}

void categoriesView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        QModelIndex index =
            indexAt(event->position().toPoint());

        if (index.isValid())
            setCurrentIndex(index);
    }

    QTreeView::mousePressEvent(event);
}