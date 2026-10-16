// #endif // CATEGORIESVIEW_H
#ifndef CATEGORIESVIEW_H
#define CATEGORIESVIEW_H

#include <QTreeView>
#include <QPersistentModelIndex>

class QDragEnterEvent;
class QDragMoveEvent;
class QDragLeaveEvent;
class QDropEvent;
class QPaintEvent;
class QMouseEvent;

class categoriesView : public QTreeView
{
    Q_OBJECT

public:
    explicit categoriesView(QWidget *parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QPersistentModelIndex m_dropIndex;
};

// #endif // CATEGORIESVIEW_H
#ifndef CATEGORIESVIEW_H
#define CATEGORIESVIEW_H

#include <QTreeView>
#include <QPersistentModelIndex>

class QDragEnterEvent;
class QDragMoveEvent;
class QDragLeaveEvent;
class QDropEvent;
class QPaintEvent;
class QMouseEvent;

class categoriesView : public QTreeView
{
    Q_OBJECT

public:
    explicit categoriesView(QWidget *parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QPersistentModelIndex m_dropIndex;
};

#endif // CATEGORIESVIEW_H