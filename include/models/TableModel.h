<<<<<<< HEAD
#pragma once

#include <QAbstractTableModel>
#include "core/repository.h"
#include "core/consts.h"
#include "core/Sound.h"

class TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit TableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;

    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QStringList mimeTypes() const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                      int row, int column, const QModelIndex &parent) override;
    Qt::DropActions supportedDropActions() const override;

    QString ModelformatDuration(qint64 ms) const;

    void refreshModel();
    void addSound(const SoundT& sound);
    void removeSounds(const QStringList& soundIds);
    void moveSoundsWithinCategory(const QStringList& soundIds, int toRow);

    void setCategories(QVector<SoundCategory> *categories);

    void setCurrentCategory(const QString &category);
    QString getCurrentCategory() const;

    const SoundT* soundAt(int row) const;
    SoundT getSoundAt(int row) const;
    int findSoundIndex(int soundId) const;

    void sort(int column, Qt::SortOrder order) override;

signals:
    void soundsMoved(const QStringList& soundIds, const QString& sourceCategory,
                     const QString& targetCategory, int targetRow);
    void soundsRemoved(const QStringList& soundIds);
    void soundMovedWithinCategory(const QStringList& soundIds, int toRow);

public:
    QVector<SoundCategory> *m_categories;
    QString m_currentCategory = ALL_SOUNDS;

    enum Roles
    {
        IdRole = Qt::UserRole + 1
    };
};
=======
#pragma once

#include <QAbstractTableModel>
#include "core/repository.h"
#include "core/consts.h"
#include "core/Sound.h"

class TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit TableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;

    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QStringList mimeTypes() const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                      int row, int column, const QModelIndex &parent) override;
    Qt::DropActions supportedDropActions() const override;

    QString ModelformatDuration(qint64 ms) const;

    void refreshModel();
    void addSound(const SoundT& sound);
    void removeSounds(const QStringList& soundIds);
    void moveSoundsWithinCategory(const QStringList& soundIds, int toRow);

    void setCategories(QMap<QString, QVector<SoundT>> *categories);

    void setCurrentCategory(const QString &category);
    QString getCurrentCategory() const;

    const SoundT* soundAt(int row) const;
    SoundT getSoundAt(int row) const;
    int findSoundIndex(int soundId) const;

    void sort(int column, Qt::SortOrder order) override;

signals:
    void soundsMoved(const QStringList& soundIds, const QString& sourceCategory,
                     const QString& targetCategory, int targetRow);
    void soundsRemoved(const QStringList& soundIds);
    void soundMovedWithinCategory(const QStringList& soundIds, int toRow);

public:
    QMap<QString, QVector<SoundT>> *m_categories;
    QString m_currentCategory = ALL_SOUNDS;

    enum Roles
    {
        IdRole = Qt::UserRole + 1
    };
};
>>>>>>> origin/main
