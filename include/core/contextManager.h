#ifndef CONTEXTMENUMANAGER_H
#define CONTEXTMENUMANAGER_H

#include <QObject>
#include <QPoint>
#include "models/TableModel.h"

class QMenu;
namespace Ui {
class MainWindow;
}

class ContextMenuManager : public QObject
{
    Q_OBJECT

public:
    explicit ContextMenuManager(Ui::MainWindow *ui, QObject *parent = nullptr);

signals:
    void addCategoryRequested();
    void deleteSoundsRequested(const QStringList &ids);
    void deleteCategoryRequested(const QString &categoryName);
    void showAllSoundsRequested();
    void changeCategoryRequested();
    void addHotkeyRequested(const QString &soundId);
    void removeHotkeyRequested(const QString &soundId);

public slots:
    void showTableMenu(const QPoint &pos);
    void showTreeMenu(const QPoint &pos);
    void showLowerBarMenu(const QPoint &pos);

private:
    Ui::MainWindow *m_ui;
};

#endif // CONTEXTMENUMANAGER_H