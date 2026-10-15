<<<<<<< HEAD
#include "models/CategoryModel.h"
#include "ui/categoryIcon.h"

#include <QIcon>
#include <QMimeData>

namespace
{
int repositoryRowForModelRow(int modelRow)
{
    const auto &categories = Repository::instance().getCategoriesData();
    int visibleRow = 0;

    for (int repositoryRow = 0; repositoryRow < categories.size();
         ++repositoryRow)
    {
        if (categories[repositoryRow].name == ALL_SOUNDS)
            continue;

        if (visibleRow == modelRow)
            return repositoryRow;
        ++visibleRow;
    }

    return -1;
}

int modelRowForCategoryId(const QUuid &id)
{
    const auto &categories = Repository::instance().getCategoriesData();
    int visibleRow = 0;

    for (const SoundCategory &category : categories)
    {
        if (category.name == ALL_SOUNDS)
            continue;
        if (category.id == id)
            return visibleRow;
        ++visibleRow;
    }

    return -1;
}
}

CategoryModel::CategoryModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

QModelIndex CategoryModel::index(int row, int column,
                                 const QModelIndex &parent) const
{
    if (parent.isValid() || row < 0 || row >= rowCount()
        || column < 0 || column >= columnCount())
        return {};
    return createIndex(row, column);
}

QModelIndex CategoryModel::parent(const QModelIndex &) const
{
    return {};
}

int CategoryModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    int count = 0;
    for (const SoundCategory &category :
         Repository::instance().getCategoriesData())
    {
        if (category.name != ALL_SOUNDS)
            ++count;
    }
    return count;
}

int CategoryModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant CategoryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    const auto &categories = Repository::instance().getCategoriesData();
    const int repositoryRow = repositoryRowForModelRow(index.row());
    if (repositoryRow < 0)
        return {};

    const SoundCategory &category = categories.at(repositoryRow);

    if (role == Qt::TextAlignmentRole && index.column() == 1)
        return QVariant::fromValue(Qt::AlignRight | Qt::AlignVCenter);

    if (role == Qt::DecorationRole && index.column() == 0)
        return QIcon(createCategoryIcon(category.name, category.icon,
                                        QSize(32, 32)));

    if (role == Qt::DisplayRole)
        return index.column() == 0
            ? QVariant(category.name)
            : QVariant(category.sounds.size());

    if (role == Qt::UserRole)
        return category.name;

    if (role == CategoryIdRole)
        return category.id;

    return {};
}

Qt::ItemFlags CategoryModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags result = Qt::ItemIsEnabled | Qt::ItemIsSelectable
                           | Qt::ItemIsDragEnabled;
    if (index.isValid())
        result |= Qt::ItemIsDropEnabled;
    return result;
}

QStringList CategoryModel::mimeTypes() const
{
    return {"application/x-category-id", "application/x-sound-ids"};
}

QMimeData *CategoryModel::mimeData(const QModelIndexList &indexes) const
{
    for (const QModelIndex &modelIndex : indexes)
    {
        if (!modelIndex.isValid() || modelIndex.column() != 0)
            continue;

        auto *mime = new QMimeData;
        mime->setData("application/x-category-id",
                      modelIndex.data(CategoryIdRole).toUuid()
                          .toString(QUuid::WithoutBraces).toUtf8());
        return mime;
    }

    return nullptr;
}

bool CategoryModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
                                 int row, int column,
                                 const QModelIndex &parent)
{
    Q_UNUSED(column);
    if (action == Qt::IgnoreAction)
        return true;
    if (!data)
        return false;

    if (data->hasFormat("application/x-sound-ids"))
    {
        if (!parent.isValid())
            return false;
        const QString targetCategory = parent.data(Qt::UserRole).toString();
        const QString ids = QString::fromUtf8(
            data->data("application/x-sound-ids"));
        if (ids.isEmpty() || targetCategory.isEmpty())
            return false;
        emit soundsMovedToCategory(ids.split('|'), targetCategory);
        return true;
    }

    if (!data->hasFormat("application/x-category-id"))
        return false;

    const QUuid id(QString::fromUtf8(data->data("application/x-category-id")));
    const auto &categories = Repository::instance().getCategoriesData();
    const int fromRow = modelRowForCategoryId(id);

    if (fromRow < 0)
        return false;

    int targetRow = parent.isValid() ? parent.row() : row;
    if (targetRow < 0)
        targetRow = rowCount() - 1;
    targetRow = qBound(0, targetRow, rowCount() - 1);
    if (fromRow == targetRow)
        return false;

    const int sourceRepositoryRow = repositoryRowForModelRow(fromRow);
    emit categoryMoved(categories[sourceRepositoryRow].name,
                       fromRow, targetRow);
    return true;
}

Qt::DropActions CategoryModel::supportedDropActions() const
{
    return Qt::MoveAction | Qt::CopyAction;
}

bool CategoryModel::canDropMimeData(const QMimeData *data,
                                    Qt::DropAction action, int row, int column,
                                    const QModelIndex &parent) const
{
    Q_UNUSED(action);
    Q_UNUSED(row);
    Q_UNUSED(column);
    if (!data)
        return false;
    if (data->hasFormat("application/x-sound-ids"))
        return parent.isValid();
    if (!data->hasFormat("application/x-category-id"))
        return false;

    const QUuid id(QString::fromUtf8(data->data("application/x-category-id")));
    const int sourceRow = modelRowForCategoryId(id);
    return sourceRow >= 0
        && (!parent.isValid() || parent.row() != sourceRow);
}

void CategoryModel::moveCategory(int fromRow, int toRow)
{
    if (fromRow == toRow)
        return;

    const int fromRepositoryRow = repositoryRowForModelRow(fromRow);
    const int toRepositoryRow = repositoryRowForModelRow(toRow);
    if (fromRepositoryRow < 0 || toRepositoryRow < 0)
        return;

    const int destinationRow = fromRow < toRow ? toRow + 1 : toRow;
    beginMoveRows({}, fromRow, fromRow, {}, destinationRow);
    Repository::instance().moveCategory(fromRepositoryRow, toRepositoryRow);
    endMoveRows();
}

void CategoryModel::refreshModel()
{
    beginResetModel();
    endResetModel();
}

QStringList CategoryModel::getCategoryNames() const
{
    QStringList names;
    for (const SoundCategory &category :
         Repository::instance().getCategoriesData())
    {
        if (category.name != ALL_SOUNDS)
            names.append(category.name);
    }
    return names;
}

QString CategoryModel::categoryAt(int row) const
{
    const auto &categories = Repository::instance().getCategoriesData();
    const int repositoryRow = repositoryRowForModelRow(row);
    return repositoryRow >= 0
        ? categories[repositoryRow].name
        : QString();
}
=======
#include "models/CategoryModel.h"
#include <QMimeData>

CategoryModel::CategoryModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

QModelIndex CategoryModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid())
        return QModelIndex();

    if (row < 0 || row >= rowCount() || column < 0 || column >= columnCount())
        return QModelIndex();

    return createIndex(row, column);
}

QModelIndex CategoryModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

int CategoryModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return Repository::instance().getCategoryOrder().size();
}

int CategoryModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant CategoryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    auto &repository = Repository::instance();
    const auto &order = repository.getCategoryOrder();

    if (index.row() < 0 || index.row() >= order.size())
        return {};

    const QString &category = order.at(index.row());

    if (role == Qt::TextAlignmentRole && index.column() == 1)
        return QVariant::fromValue(Qt::AlignRight | Qt::AlignVCenter);

    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
            return category;

        if (index.column() == 1)
            return repository.getCategoriesData().value(category).size();
    }

    if (role == Qt::UserRole)
        return category;

    return {};
}

Qt::ItemFlags CategoryModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable |
                          Qt::ItemIsDragEnabled;

    if (index.isValid())
        flags |= Qt::ItemIsDropEnabled;

    return flags;
}

QStringList CategoryModel::mimeTypes() const
{
    return {"application/x-category", "application/x-sound-ids"};
}

QMimeData *CategoryModel::mimeData(
    const QModelIndexList &indexes) const
{
    if (indexes.isEmpty())
        return nullptr;

    auto *mime = new QMimeData;

    QStringList categories;

    const auto &order =
        Repository::instance().getCategoryOrder();

    for (const QModelIndex &index : indexes)
    {
        if (!index.isValid())
            continue;

        if (index.column() == 0)
            categories << order[index.row()];
    }

    if (categories.isEmpty())
    {
        delete mime;
        return nullptr;
    }

    mime->setData(
        "application/x-category",
        categories.join('|').toUtf8()
        );

    return mime;
}

bool CategoryModel::dropMimeData(
    const QMimeData *data,
    Qt::DropAction action,
    int row,
    int column,
    const QModelIndex &parent)
{
    Q_UNUSED(column);

    if (action == Qt::IgnoreAction)
        return true;

    if (!data)
        return false;

    // =============================
    // SOUND -> CATEGORY
    // =============================

    if (data->hasFormat("application/x-sound-ids"))
    {
        if (!parent.isValid())
            return false;

        QString targetCategory =
            parent.data(Qt::UserRole).toString();

        QString soundIdsStr =
            QString::fromUtf8(
                data->data("application/x-sound-ids")
                );

        if (soundIdsStr.isEmpty() ||
            targetCategory.isEmpty())
        {
            return false;
        }

        emit soundsMovedToCategory(
            soundIdsStr.split('|'),
            targetCategory
            );

        return true;
    }

    // =============================
    // CATEGORY -> CATEGORY
    // =============================

    if (data->hasFormat("application/x-category"))
    {
        QString category =
            QString::fromUtf8(
                data->data("application/x-category")
                );

        const auto &order =
            Repository::instance().getCategoryOrder();

        int fromRow =
            order.indexOf(category);

        if (fromRow == -1)
            return false;

        int targetRow;

        if (parent.isValid())
        {
            targetRow = parent.row();
        }
        else
        {
            targetRow = row;
        }

        if (targetRow < 0)
            targetRow = rowCount() - 1;

        if (fromRow == targetRow)
            return false;

        emit categoryMoved(
            category,
            fromRow,
            targetRow
            );

        return true;
    }

    return false;
}

Qt::DropActions CategoryModel::supportedDropActions() const
{
    return Qt::MoveAction | Qt::CopyAction;
}

bool CategoryModel::canDropMimeData(
    const QMimeData *data,
    Qt::DropAction action,
    int row,
    int column,
    const QModelIndex &parent) const
{
    Q_UNUSED(action);
    Q_UNUSED(row);
    Q_UNUSED(column);

    if (!data)
        return false;

    if (data->hasFormat("application/x-sound-ids"))
    {
        return parent.isValid();
    }

    if (data->hasFormat("application/x-category"))
    {
        QString category =
            QString::fromUtf8(
                data->data("application/x-category")
                );

        int fromRow =
            Repository::instance()
                .getCategoryOrder()
                .indexOf(category);

        if (fromRow == -1)
            return false;

        if (parent.isValid() &&
            parent.row() == fromRow)
        {
            return false;
        }

        return true;
    }

    return false;
}

void CategoryModel::moveCategory(
    int fromRow,
    int toRow)
{
    if (fromRow == toRow)
        return;

    int destinationRow =
        fromRow < toRow
            ? toRow + 1
            : toRow;

    beginMoveRows(
        QModelIndex(),
        fromRow,
        fromRow,
        QModelIndex(),
        destinationRow
        );

    Repository::instance().moveCategory(fromRow, toRow);

    endMoveRows();
}

void CategoryModel::refreshModel()
{
    beginResetModel();
    endResetModel();
}

QStringList CategoryModel::getCategoryNames() const
{
    return Repository::instance().getCategoryOrder();
}

QString CategoryModel::categoryAt(int row) const
{
    const auto &order = Repository::instance().getCategoryOrder();

    if (row < 0 || row >= order.size())
        return {};

    return order[row];
}
>>>>>>> origin/main
