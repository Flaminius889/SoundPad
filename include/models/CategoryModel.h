#pragma once

#include <QAbstractItemModel>
#include "core/repository.h"
#include <QStringList>

class CategoryModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit CategoryModel(QObject *parent = nullptr);

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                      int row, int column, const QModelIndex &parent) override;

    bool canDropMimeData(const QMimeData *data, Qt::DropAction action,
                         int row, int column, const QModelIndex &parent) const override;

    QMimeData *mimeData(const QModelIndexList &indexes) const override;

    QStringList mimeTypes() const override;

    Qt::DropActions supportedDropActions() const override;

    void refreshModel();

    void moveCategory(int fromRow, int toRow);

    QString categoryAt(int row) const;
    QStringList getCategoryNames() const;

    enum Roles
    {
        CategoryIdRole = Qt::UserRole + 1
    };

signals:
    void categoryMoved(const QString& category, int fromRow, int toRow);
    void soundsMovedToCategory(const QStringList& soundIds, const QString& targetCategory);
};
#pragma once

#include <QAbstractItemModel>
#include "core/repository.h"
#include <QStringList>

class CategoryModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit CategoryModel(QObject *parent = nullptr);

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                      int row, int column, const QModelIndex &parent) override;

    bool canDropMimeData(const QMimeData *data, Qt::DropAction action,
                         int row, int column, const QModelIndex &parent) const override;

    QMimeData *mimeData(const QModelIndexList &indexes) const override;

    QStringList mimeTypes() const override;

    Qt::DropActions supportedDropActions() const override;

    void refreshModel();

    void moveCategory(int fromRow, int toRow);

    QString categoryAt(int row) const;
    QStringList getCategoryNames() const;

signals:
    void categoryMoved(const QString& category, int fromRow, int toRow);
    void soundsMovedToCategory(const QStringList& soundIds, const QString& targetCategory);
};
