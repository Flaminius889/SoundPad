#include "models/TableModel.h"
#include <QColor>
#include <QFileInfo>
#include <QFont>
#include <QMimeData>

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel(parent),
    m_categories(nullptr)
{
}

int TableModel::rowCount(const QModelIndex &) const
{
    if (!m_categories) return 0;
    return Repository::instance().soundsForCategory(m_currentCategory).size();
}

int TableModel::columnCount(const QModelIndex &) const
{
    return 4;
}

const SoundT* TableModel::soundAt(int row) const
{
    const QString &category = Repository::instance().getCurrCategoryData();
    const QVector<SoundT> &sounds =
        Repository::instance().soundsForCategory(category);

    if (row < 0 || row >= sounds.size())
        return nullptr;

    return &sounds[row];
}

SoundT TableModel::getSoundAt(int row) const
{
    if (!m_categories) return SoundT{};

    const QVector<SoundT> &sounds =
        Repository::instance().soundsForCategory(m_currentCategory);
    if (row >= 0 && row < sounds.size())
        return sounds.at(row);

    return SoundT{};
}

int TableModel::findSoundIndex(int soundId) const
{
    if (!m_categories || !Repository::instance().hasCategory(m_currentCategory))
        return -1;

    const QVector<SoundT>& sounds =
        Repository::instance().soundsForCategory(m_currentCategory);
    for (int i = 0; i < sounds.size(); ++i)
    {
        if (sounds[i].id == soundId)
            return i;
    }
    return -1;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_categories)
        return {};

    const SoundT &sound = getSoundAt(index.row());

    if (role == IdRole)
        return sound.id;

    if (!QFileInfo(sound.path).isFile())
    {
        if (role == Qt::BackgroundRole)
            return QColor(255, 192, 192);

        if (role == Qt::FontRole)
        {
            QFont font;
            font.setStrikeOut(true);
            return font;
        }
    }

    if (role != Qt::DisplayRole)
        return {};


    switch(index.column())
    {
    case 0:
        return index.row() + 1;

    case 1:
        return sound.nameOfMusic;

    case 2:
        return ModelformatDuration(sound.duration);

    case 3:
        return sound.hotkey;
    default:
        return {};
    }

    return {};
}

QVariant TableModel::headerData(int section,
                                Qt::Orientation orientation,
                                int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch (section) {
            case 0: return "№";
            case 1: return "Tag";
            case 2: return "Duration";
            case 3: return "HotKeys";
            }
        }

        if (role == Qt::TextAlignmentRole)
            return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = Qt::ItemIsSelectable |
                          Qt::ItemIsEnabled |
                          Qt::ItemIsDragEnabled |
                          Qt::ItemIsDropEnabled;

    return flags;
}

QStringList TableModel::mimeTypes() const
{
    return {"application/x-sound-ids"};
}

QMimeData *TableModel::mimeData(const QModelIndexList &indexes) const
{
    if (indexes.isEmpty() || !m_categories)
        return nullptr;

    QStringList soundIds;
    QSet<int> uniqueIds;

    for (const QModelIndex &index : indexes)
    {
        if (index.isValid())
        {
            QVariant idVariant = data(index, IdRole);
            if (idVariant.isValid())
            {
                int id = idVariant.toInt();
                if (id > 0 && !uniqueIds.contains(id))
                {
                    uniqueIds.insert(id);
                    soundIds.append(QString::number(id));
                    qDebug() << "Added sound ID:" << id;
                }
            }
        }
    }

    if (soundIds.isEmpty())
        return nullptr;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-sound-ids", soundIds.join("|").toUtf8());
    return mimeData;
}

bool TableModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
                              int row, int column, const QModelIndex &parent)
{
    qDebug() << "TableModel::dropMimeData START";
    qDebug() << "  row:" << row << "column:" << column;
    qDebug() << "  currentCategory:" << m_currentCategory;
    qDebug() << "  parent valid:" << parent.isValid();

    if (action == Qt::IgnoreAction)
        return true;

    if (!data || !data->hasFormat("application/x-sound-ids") || !m_categories)
        return false;

    QStringList soundIds = QString::fromUtf8(
                               data->data("application/x-sound-ids")).split("|");

    if (soundIds.isEmpty())
        return false;

    int targetRow = row;
    if (parent.isValid())
    {
        targetRow = parent.row();
    }

    if (row == rowCount() || (parent.isValid() && parent.row() == rowCount() - 1))
    {
        targetRow = rowCount();
    }

    if (targetRow < 0)
        targetRow = rowCount();

    qDebug() << "  Final targetRow:" << targetRow;

    QString targetCategory = m_currentCategory;

    bool allInCurrentCategory = true;
    QList<int> sourceRows;

    for (const QString& idStr : soundIds)
    {
        bool ok;
        int id = idStr.toInt(&ok);
        if (!ok)
        {
            allInCurrentCategory = false;
            break;
        }

        int sourceRow = findSoundIndex(id);
        qDebug() << "  Sound ID:" << id << "found at row:" << sourceRow;

        if (sourceRow == -1)
        {
            allInCurrentCategory = false;
            break;
        }
        sourceRows.append(sourceRow);
    }

    if (allInCurrentCategory)
    {
        qDebug() << "Moving within category";
        std::sort(sourceRows.begin(), sourceRows.end(), std::greater<int>());

        int minSourceRow = sourceRows.last();

        qDebug() << "  Adjusted target row:" << targetRow;

        if (targetRow != minSourceRow && targetRow >= 0)
        {
            emit soundMovedWithinCategory(soundIds, targetRow);
            qDebug() << "Emitted soundMovedWithinCategory";
            return true;
        }
        else
        {
            qDebug() << "No actual move needed";
            return false;
        }
    }

    qDebug() << "Moving between categories";
    emit soundsMoved(soundIds, m_currentCategory, targetCategory, targetRow);
    return true;
}

Qt::DropActions TableModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

void TableModel::moveSoundsWithinCategory(const QStringList& soundIds, int toRow)
{
    if (!m_categories || !Repository::instance().hasCategory(m_currentCategory)
        || soundIds.isEmpty())
        return;

    QVector<SoundT>& sounds =
        Repository::instance().soundsForCategory(m_currentCategory);

    QList<int> sourceRows;
    for (const QString& idStr : soundIds)
    {
        bool ok;
        int id = idStr.toInt(&ok);
        if (!ok) continue;

        for (int i = 0; i < sounds.size(); ++i)
        {
            if (sounds[i].id == id)
            {
                sourceRows.append(i);
                break;
            }
        }
    }

    if (sourceRows.isEmpty())
        return;

    std::sort(sourceRows.begin(), sourceRows.end(), std::greater<int>());

    QVector<SoundT> soundsToMove;
    for (int row : sourceRows)
    {
        soundsToMove.prepend(sounds[row]);
        sounds.removeAt(row);
    }

    int insertPos = toRow;
    if (insertPos < 0) insertPos = 0;
    if (insertPos > sounds.size()) insertPos = sounds.size();

    qDebug() << "  insertPos:" << insertPos;

    for (int i = 0; i < soundsToMove.size(); ++i)
    {
        sounds.insert(insertPos + i, soundsToMove[i]);
    }

    qDebug() << "  Sounds moved successfully";
}

void TableModel::refreshModel()
{
    beginResetModel();
    setCategories(&Repository::instance().getCategoriesData());
    setCurrentCategory(Repository::instance().getCurrCategoryData());
    endResetModel();
}

void TableModel::addSound(const SoundT& sound)
{
    if (!m_categories)
        return;

    if (Repository::instance().hasCategory(ALL_SOUNDS))
    {
        QVector<SoundT> &allSounds =
            Repository::instance().soundsForCategory(ALL_SOUNDS);
        int row = allSounds.size();
        beginInsertRows(QModelIndex(), row, row);
        allSounds.append(sound);
        endInsertRows();
    }

    if (m_currentCategory != ALL_SOUNDS
        && Repository::instance().hasCategory(m_currentCategory))
    {
        QVector<SoundT> &sounds =
            Repository::instance().soundsForCategory(m_currentCategory);
        int row = sounds.size();
        beginInsertRows(QModelIndex(), row, row);
        sounds.append(sound);
        endInsertRows();
    }
}

void TableModel::removeSounds(const QStringList& soundIds)
{
    if (!m_categories || soundIds.isEmpty())
        return;

    if (Repository::instance().hasCategory(ALL_SOUNDS))
    {
        QVector<SoundT>& allSounds =
            Repository::instance().soundsForCategory(ALL_SOUNDS);
        for (const QString& idStr : soundIds)
        {
            bool ok;
            int id = idStr.toInt(&ok);
            if (!ok) continue;

            for (int i = 0; i < allSounds.size(); ++i)
            {
                if (allSounds[i].id == id)
                {
                    beginRemoveRows(QModelIndex(), i, i);
                    allSounds.removeAt(i);
                    endRemoveRows();
                    break;
                }
            }
        }
    }

    for (SoundCategory &category : *m_categories)
    {
        if (category.name == ALL_SOUNDS) continue;

        QVector<SoundT>& sounds = category.sounds;
        for (const QString& idStr : soundIds)
        {
            bool ok;
            int id = idStr.toInt(&ok);
            if (!ok) continue;

            for (int i = 0; i < sounds.size(); ++i)
            {
                if (sounds[i].id == id)
                {
                    sounds.removeAt(i);
                    break;
                }
            }
        }
    }
}

void TableModel::setCategories(QVector<SoundCategory> *categories)
{
    beginResetModel();
    m_categories = categories;
    endResetModel();
}

void TableModel::setCurrentCategory(const QString &category)
{
    if (m_currentCategory == category)
        return;

    beginResetModel();
    m_currentCategory = category;
    endResetModel();
}

QString TableModel::getCurrentCategory() const
{
    return m_currentCategory;
}

enum Columns
{
    TagCol = 1,
    DurationCol,
    HotKeysCol
};

void TableModel::sort(int column, Qt::SortOrder order)
{
    if (column == 0 || m_currentCategory == ALL_SOUNDS)
        return;

    beginResetModel();

    auto &list = Repository::instance().soundsForCategory(m_currentCategory);

    std::sort(list.begin(), list.end(),
              [column, order](const SoundT &a, const SoundT &b)
              {
                  bool asc = (order == Qt::AscendingOrder);

                  switch (column)
                  {
                  case TagCol:
                      return asc ? a.nameOfMusic < b.nameOfMusic
                                 : a.nameOfMusic > b.nameOfMusic;

                  case DurationCol:
                      return asc ? a.duration < b.duration
                                 : a.duration > b.duration;

                  case HotKeysCol:
                      return asc ? a.hotkey < b.hotkey
                                 : a.hotkey > b.hotkey;
                  }

                  return false;
              });

    endResetModel();
}
#include "models/TableModel.h"
#include <QMimeData>

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel(parent),
    m_categories(nullptr)
{
}

int TableModel::rowCount(const QModelIndex &) const
{
    if (!m_categories) return 0;

    if (m_currentCategory == ALL_SOUNDS)
    {
        return m_categories->value(ALL_SOUNDS).size();
    }

    return m_categories->value(m_currentCategory).size();
}

int TableModel::columnCount(const QModelIndex &) const
{
    return 4;
}

const SoundT* TableModel::soundAt(int row) const
{
    const auto &categories = Repository::instance().getCategoriesData();
    const QString &category = Repository::instance().getCurrCategoryData();

    auto categoryIt = categories.constFind(category);

    if (categoryIt == categories.constEnd())
        return nullptr;

    const QVector<SoundT> &sounds = categoryIt.value();

    if (row < 0 || row >= sounds.size())
        return nullptr;

    return &sounds[row];
}

SoundT TableModel::getSoundAt(int row) const
{
    if (!m_categories) return SoundT{};

    if (m_currentCategory == ALL_SOUNDS)
    {
        const QVector<SoundT>& allSounds = m_categories->value(ALL_SOUNDS);
        if (row >= 0 && row < allSounds.size())
            return allSounds.at(row);
    }
    else
    {
        if (m_categories->contains(m_currentCategory))
        {
            const QVector<SoundT>& sounds = m_categories->value(m_currentCategory);
            if (row >= 0 && row < sounds.size())
                return sounds.at(row);
        }
    }

    return SoundT{};
}

int TableModel::findSoundIndex(int soundId) const
{
    if (!m_categories || !m_categories->contains(m_currentCategory))
        return -1;

    const QVector<SoundT>& sounds = m_categories->value(m_currentCategory);
    for (int i = 0; i < sounds.size(); ++i)
    {
        if (sounds[i].id == soundId)
            return i;
    }
    return -1;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_categories)
        return {};

    const SoundT &sound = getSoundAt(index.row());

    if (role == IdRole)
        return sound.id;

    if (role != Qt::DisplayRole)
        return {};


    switch(index.column())
    {
    case 0:
        return index.row() + 1;

    case 1:
        return sound.nameOfMusic;

    case 2:
        return ModelformatDuration(sound.duration);

    case 3:
        return sound.hotkey;
    default:
        return {};
    }

    return {};
}

QVariant TableModel::headerData(int section,
                                Qt::Orientation orientation,
                                int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch (section) {
            case 0: return "№";
            case 1: return "Tag";
            case 2: return "Duration";
            case 3: return "HotKeys";
            }
        }

        if (role == Qt::TextAlignmentRole)
            return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = Qt::ItemIsSelectable |
                          Qt::ItemIsEnabled |
                          Qt::ItemIsDragEnabled |
                          Qt::ItemIsDropEnabled;

    return flags;
}

QStringList TableModel::mimeTypes() const
{
    return {"application/x-sound-ids"};
}

QMimeData *TableModel::mimeData(const QModelIndexList &indexes) const
{
    if (indexes.isEmpty() || !m_categories)
        return nullptr;

    QStringList soundIds;
    QSet<int> uniqueIds;

    for (const QModelIndex &index : indexes)
    {
        if (index.isValid())
        {
            QVariant idVariant = data(index, IdRole);
            if (idVariant.isValid())
            {
                int id = idVariant.toInt();
                if (id > 0 && !uniqueIds.contains(id))
                {
                    uniqueIds.insert(id);
                    soundIds.append(QString::number(id));
                    qDebug() << "Added sound ID:" << id;
                }
            }
        }
    }

    if (soundIds.isEmpty())
        return nullptr;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-sound-ids", soundIds.join("|").toUtf8());
    return mimeData;
}

bool TableModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
                              int row, int column, const QModelIndex &parent)
{
    qDebug() << "TableModel::dropMimeData START";
    qDebug() << "  row:" << row << "column:" << column;
    qDebug() << "  currentCategory:" << m_currentCategory;
    qDebug() << "  parent valid:" << parent.isValid();

    if (action == Qt::IgnoreAction)
        return true;

    if (!data || !data->hasFormat("application/x-sound-ids") || !m_categories)
        return false;

    QStringList soundIds = QString::fromUtf8(
                               data->data("application/x-sound-ids")).split("|");

    if (soundIds.isEmpty())
        return false;

    int targetRow = row;
    if (parent.isValid())
    {
        targetRow = parent.row();
    }

    if (row == rowCount() || (parent.isValid() && parent.row() == rowCount() - 1))
    {
        targetRow = rowCount();
    }

    if (targetRow < 0)
        targetRow = rowCount();

    qDebug() << "  Final targetRow:" << targetRow;

    QString targetCategory = m_currentCategory;

    bool allInCurrentCategory = true;
    QList<int> sourceRows;

    for (const QString& idStr : soundIds)
    {
        bool ok;
        int id = idStr.toInt(&ok);
        if (!ok)
        {
            allInCurrentCategory = false;
            break;
        }

        int sourceRow = findSoundIndex(id);
        qDebug() << "  Sound ID:" << id << "found at row:" << sourceRow;

        if (sourceRow == -1)
        {
            allInCurrentCategory = false;
            break;
        }
        sourceRows.append(sourceRow);
    }

    if (allInCurrentCategory)
    {
        qDebug() << "Moving within category";
        std::sort(sourceRows.begin(), sourceRows.end(), std::greater<int>());

        int minSourceRow = sourceRows.last();

        qDebug() << "  Adjusted target row:" << targetRow;

        if (targetRow != minSourceRow && targetRow >= 0)
        {
            emit soundMovedWithinCategory(soundIds, targetRow);
            qDebug() << "Emitted soundMovedWithinCategory";
            return true;
        }
        else
        {
            qDebug() << "No actual move needed";
            return false;
        }
    }

    qDebug() << "Moving between categories";
    emit soundsMoved(soundIds, m_currentCategory, targetCategory, targetRow);
    return true;
}

Qt::DropActions TableModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

void TableModel::moveSoundsWithinCategory(const QStringList& soundIds, int toRow)
{
    if (!m_categories || !m_categories->contains(m_currentCategory) || soundIds.isEmpty())
        return;

    QVector<SoundT>& sounds = m_categories->operator[](m_currentCategory);

    QList<int> sourceRows;
    for (const QString& idStr : soundIds)
    {
        bool ok;
        int id = idStr.toInt(&ok);
        if (!ok) continue;

        for (int i = 0; i < sounds.size(); ++i)
        {
            if (sounds[i].id == id)
            {
                sourceRows.append(i);
                break;
            }
        }
    }

    if (sourceRows.isEmpty())
        return;

    std::sort(sourceRows.begin(), sourceRows.end(), std::greater<int>());

    QVector<SoundT> soundsToMove;
    for (int row : sourceRows)
    {
        soundsToMove.prepend(sounds[row]);
        sounds.removeAt(row);
    }

    int insertPos = toRow;
    if (insertPos < 0) insertPos = 0;
    if (insertPos > sounds.size()) insertPos = sounds.size();

    qDebug() << "  insertPos:" << insertPos;

    for (int i = 0; i < soundsToMove.size(); ++i)
    {
        sounds.insert(insertPos + i, soundsToMove[i]);
    }

    qDebug() << "  Sounds moved successfully";
}

void TableModel::refreshModel()
{
    beginResetModel();
    setCategories(&Repository::instance().getCategoriesData());
    setCurrentCategory(Repository::instance().getCurrCategoryData());
    endResetModel();
}

void TableModel::addSound(const SoundT& sound)
{
    if (!m_categories)
        return;

    if (m_categories->contains(ALL_SOUNDS))
    {
        int row = m_categories->value(ALL_SOUNDS).size();
        beginInsertRows(QModelIndex(), row, row);
        m_categories->operator[](ALL_SOUNDS).append(sound);
        endInsertRows();
    }

    if (m_currentCategory != ALL_SOUNDS && m_categories->contains(m_currentCategory))
    {
        int row = m_categories->value(m_currentCategory).size();
        beginInsertRows(QModelIndex(), row, row);
        m_categories->operator[](m_currentCategory).append(sound);
        endInsertRows();
    }
}

void TableModel::removeSounds(const QStringList& soundIds)
{
    if (!m_categories || soundIds.isEmpty())
        return;

    if (m_categories->contains(ALL_SOUNDS))
    {
        QVector<SoundT>& allSounds = m_categories->operator[](ALL_SOUNDS);
        for (const QString& idStr : soundIds)
        {
            bool ok;
            int id = idStr.toInt(&ok);
            if (!ok) continue;

            for (int i = 0; i < allSounds.size(); ++i)
            {
                if (allSounds[i].id == id)
                {
                    beginRemoveRows(QModelIndex(), i, i);
                    allSounds.removeAt(i);
                    endRemoveRows();
                    break;
                }
            }
        }
    }

    for (auto it = m_categories->begin(); it != m_categories->end(); ++it)
    {
        if (it.key() == ALL_SOUNDS) continue;

        QVector<SoundT>& sounds = it.value();
        for (const QString& idStr : soundIds)
        {
            bool ok;
            int id = idStr.toInt(&ok);
            if (!ok) continue;

            for (int i = 0; i < sounds.size(); ++i)
            {
                if (sounds[i].id == id)
                {
                    sounds.removeAt(i);
                    break;
                }
            }
        }
    }
}

void TableModel::setCategories(QMap<QString, QVector<SoundT>> *categories)
{
    beginResetModel();
    m_categories = categories;
    endResetModel();
}

void TableModel::setCurrentCategory(const QString &category)
{
    if (m_currentCategory == category)
        return;

    beginResetModel();
    m_currentCategory = category;
    endResetModel();
}

QString TableModel::getCurrentCategory() const
{
    return m_currentCategory;
}

enum Columns
{
    TagCol = 1,
    DurationCol,
    HotKeysCol
};

void TableModel::sort(int column, Qt::SortOrder order)
{
    if (column == 0 || m_currentCategory == ALL_SOUNDS)
        return;

    beginResetModel();

    auto &list = Repository::instance().getCategoriesData()[m_currentCategory];

    std::sort(list.begin(), list.end(),
              [column, order](const SoundT &a, const SoundT &b)
              {
                  bool asc = (order == Qt::AscendingOrder);

                  switch (column)
                  {
                  case TagCol:
                      return asc ? a.nameOfMusic < b.nameOfMusic
                                 : a.nameOfMusic > b.nameOfMusic;

                  case DurationCol:
                      return asc ? a.duration < b.duration
                                 : a.duration > b.duration;

                  case HotKeysCol:
                      return asc ? a.hotkey < b.hotkey
                                 : a.hotkey > b.hotkey;
                  }

                  return false;
              });

    endResetModel();
}
