#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QString>
#include <QKeySequence>
#include "core/Sound.h"
#include "core/consts.h"


class Repository {
public:
    Repository();
    static Repository& instance();

    QMap<QString, QVector<SoundT>> &getCategoriesData();
    QString &getCurrCategoryData();
    QStringList& getCategoryOrder();

    void setCurrCategoryData(const QString &name);
    void setCategoryOrder(const QStringList &CategoryOrder);

    void renameCategory(const QString& oldName, const QString& newName);

    void addSound(const QString& category, const SoundT& sound);
    void addCategory(const QString& name);
    void removeCategory(const QString& name);
    void removeSound(const QString& category, int index);

    void setSoundHotkey(int soundId, const QKeySequence &hotkey);
    QKeySequence getSoundHotkey(int soundId) const;

    void moveCategory(int fromRow, int toRow);

    void deleteFiles();

    QString dataPath();
    void SaveData();
    bool ReadData();

    Repository(const Repository&) = delete;
    Repository& operator=(const Repository&) = delete;
private:
    QMap<QString, QVector<SoundT>> categories;
    QString currentCategory;

    QStringList categoryOrder;
};

#endif // REPOSITORY_H