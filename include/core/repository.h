#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QString>
#include <QKeySequence>
#include "core/SoundCategory.h"
#include "core/consts.h"


class Repository {
public:
    Repository();
    static Repository& instance();

    QVector<SoundCategory> &getCategoriesData();
    const QVector<SoundCategory> &getCategoriesData() const;
    QString &getCurrCategoryData();
    QStringList getCategoryOrder() const;

    bool hasCategory(const QString &name) const;
    SoundCategory *categoryByName(const QString &name);
    const SoundCategory *categoryByName(const QString &name) const;
    SoundCategory *categoryById(const QUuid &id);
    const SoundCategory *categoryById(const QUuid &id) const;
    QVector<SoundT> &soundsForCategory(const QString &name);
    const QVector<SoundT> &soundsForCategory(const QString &name) const;

    void setCurrCategoryData(const QString &name);
    void setCategoryOrder(const QStringList &CategoryOrder);

    void renameCategory(const QString& oldName, const QString& newName);

    void addSound(const QString& category, const SoundT& sound);
    void addCategory(const QString& name, const QByteArray &icon = {});
    void setCategoryIcon(const QString &name, const QByteArray &icon);
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
    QVector<SoundCategory> categories;
    QString currentCategory;
};

#endif // REPOSITORY_H
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
