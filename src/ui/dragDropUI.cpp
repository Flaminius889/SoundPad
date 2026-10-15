<<<<<<< HEAD
#include "ui/mainwindow.h"

//Drag/drop settings
void MainWindow::setupDragDrop()
{
    ui->soundsTable->setModel(tableModel);

    ui->soundsTable->setDragEnabled(true);
    ui->soundsTable->viewport()->setAcceptDrops(true);
    ui->soundsTable->setDragDropMode(QAbstractItemView::DragDrop);
    ui->soundsTable->setDefaultDropAction(Qt::MoveAction);
    ui->soundsTable->setDropIndicatorShown(true);

    ui->categoriesTree->setModel(categoryModel);
    ui->categoriesTree->setDragEnabled(true);
    ui->categoriesTree->viewport()->setAcceptDrops(true);
    ui->categoriesTree->setDragDropMode(QAbstractItemView::DragDrop);
    ui->categoriesTree->setDefaultDropAction(Qt::MoveAction);
    ui->categoriesTree->setDropIndicatorShown(true);

    ui->categoriesTree->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->categoriesTree->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->categoriesTree->setUniformRowHeights(true);

    ui->categoriesTree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->categoriesTree->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
}

//Drag/drop logic

void MainWindow::onSoundsMoved(const QStringList& soundIds,
                               const QString& sourceCategory,
                               const QString& targetCategory,
                               int targetRow)
{
    qDebug() << "=== onSoundsMoved ===";
    qDebug() << "  soundIds:" << soundIds;
    qDebug() << "  sourceCategory:" << sourceCategory;
    qDebug() << "  targetCategory:" << targetCategory;
    qDebug() << "target =" << "[" << targetCategory << "]";
    qDebug() << "  targetRow:" << targetRow;

    if (soundIds.isEmpty() || sourceCategory == targetCategory)
        return;

    QVector<SoundT> soundsToMove;
    auto& categories = Repository::instance().getCategoriesData();

    if (!Repository::instance().hasCategory(sourceCategory)
        || !Repository::instance().hasCategory(targetCategory))
        return;

    QVector<SoundT>& sourceSounds =
        Repository::instance().soundsForCategory(sourceCategory);

    QList<int> indicesToRemove;
    for (const QString& idStr : soundIds)
    {
        bool ok;
        int id = idStr.toInt(&ok);
        if (!ok) continue;

        for (int i = 0; i < sourceSounds.size(); ++i)
        {
            if (sourceSounds[i].id == id)
            {
                soundsToMove.append(sourceSounds[i]);
                indicesToRemove.append(i);
                break;
            }
        }
    }

    if (soundsToMove.isEmpty())
        return;

    std::sort(indicesToRemove.begin(), indicesToRemove.end(), std::greater<int>());
    for (int idx : indicesToRemove)
    {
        sourceSounds.removeAt(idx);
    }

    QVector<SoundT>& targetSounds =
        Repository::instance().soundsForCategory(targetCategory);

    if (targetRow < 0 || targetRow > targetSounds.size())
        targetRow = targetSounds.size();

    for (int i = 0; i < soundsToMove.size(); ++i)
    {
        targetSounds.insert(targetRow + i, soundsToMove[i]);
    }

    if (Repository::instance().hasCategory(ALL_SOUNDS))
    {
        QVector<SoundT>& allSounds =
            Repository::instance().soundsForCategory(ALL_SOUNDS);
        for (const SoundT& sound : soundsToMove)
        {
            for (int i = 0; i < allSounds.size(); ++i)
            {
                if (allSounds[i].id == sound.id)
                {
                    allSounds.removeAt(i);
                    break;
                }
            }
        }

        for (const SoundT& sound : soundsToMove)
        {
            allSounds.append(sound);
        }
    }

    refreshAllModels();
}

void MainWindow::onCategoryMoved(const QString& category, int fromRow, int toRow)
{
    categoryModel->moveCategory(fromRow, toRow);
}

void MainWindow::onSoundsMovedToCategory(const QStringList& soundIds,
                                         const QString& targetCategory)
{
    if (soundIds.isEmpty() || targetCategory.isEmpty() ||
        targetCategory == ALL_SOUNDS)
        return;

    QString sourceCategory = tableModel->getCurrentCategory();

    if (sourceCategory == ALL_SOUNDS)
    {
        auto& categories = Repository::instance().getCategoriesData();
        QSet<int> soundIdSet;
        for (const QString& idStr : soundIds)
        {
            bool ok;
            int id = idStr.toInt(&ok);
            if (ok) soundIdSet.insert(id);
        }

        for (const SoundCategory &category : categories)
        {
            if (category.name == ALL_SOUNDS) continue;

            const QVector<SoundT>& sounds = category.sounds;
            for (const SoundT& sound : sounds)
            {
                if (soundIdSet.contains(sound.id))
                {
                    sourceCategory = category.name;
                    break;
                }
            }
            if (sourceCategory != ALL_SOUNDS) break;
        }
    }

    onSoundsMoved(soundIds, sourceCategory, targetCategory, -1);
}

void MainWindow::onSoundMovedWithinCategory(const QStringList& soundIds, int toRow)
{
    qDebug() << "onSoundMovedWithinCategory called";
    qDebug() << "  soundIds:" << soundIds;
    qDebug() << "  toRow:" << toRow;

    tableModel->moveSoundsWithinCategory(soundIds, toRow);

    refreshAllModels();
}

void MainWindow::removeSounds(const QStringList& soundIds)
{
    if (soundIds.isEmpty())
        return;

    auto& categories = Repository::instance().getCategoriesData();
    QString currentCategory = tableModel->getCurrentCategory();

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
                    allSounds.removeAt(i);
                    break;
                }
            }
        }
    }

    if (currentCategory != ALL_SOUNDS
        && Repository::instance().hasCategory(currentCategory))
    {
        QList<int> idsToRemove;

        for (const QString &id : soundIds)
        {
            bool ok;
            int value = id.toInt(&ok);

            if (ok)
                idsToRemove.append(value);
        }

        QVector<SoundT>& sounds =
            Repository::instance().soundsForCategory(currentCategory);
        for (int i = sounds.size() - 1; i >= 0; --i)
        {
            if (idsToRemove.contains(sounds[i].id))
                sounds.removeAt(i);
        }
    }

    refreshAllModels();
}
=======
#include "ui/mainwindow.h"

//Drag/drop settings
void MainWindow::setupDragDrop()
{
    ui->soundsTable->setModel(tableModel);

    ui->soundsTable->setDragEnabled(true);
    ui->soundsTable->viewport()->setAcceptDrops(true);
    ui->soundsTable->setDragDropMode(QAbstractItemView::DragDrop);
    ui->soundsTable->setDefaultDropAction(Qt::MoveAction);
    ui->soundsTable->setDropIndicatorShown(true);

    ui->categoriesTree->setModel(categoryModel);
    ui->categoriesTree->setDragEnabled(true);
    ui->categoriesTree->viewport()->setAcceptDrops(true);
    ui->categoriesTree->setDragDropMode(QAbstractItemView::DragDrop);
    ui->categoriesTree->setDefaultDropAction(Qt::MoveAction);
    ui->categoriesTree->setDropIndicatorShown(true);

    ui->categoriesTree->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->categoriesTree->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->categoriesTree->setUniformRowHeights(true);

    ui->categoriesTree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->categoriesTree->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
}

//Drag/drop logic

void MainWindow::onSoundsMoved(const QStringList& soundIds,
                               const QString& sourceCategory,
                               const QString& targetCategory,
                               int targetRow)
{
    qDebug() << "=== onSoundsMoved ===";
    qDebug() << "  soundIds:" << soundIds;
    qDebug() << "  sourceCategory:" << sourceCategory;
    qDebug() << "  targetCategory:" << targetCategory;
    qDebug() << "target =" << "[" << targetCategory << "]";
    qDebug() << "  targetRow:" << targetRow;

    if (soundIds.isEmpty() || sourceCategory == targetCategory)
        return;

    QVector<SoundT> soundsToMove;
    auto& categories = Repository::instance().getCategoriesData();

    if (!categories.contains(sourceCategory))
        return;

    QVector<SoundT>& sourceSounds = categories[sourceCategory];

    QList<int> indicesToRemove;
    for (const QString& idStr : soundIds)
    {
        bool ok;
        int id = idStr.toInt(&ok);
        if (!ok) continue;

        for (int i = 0; i < sourceSounds.size(); ++i)
        {
            if (sourceSounds[i].id == id)
            {
                soundsToMove.append(sourceSounds[i]);
                indicesToRemove.append(i);
                break;
            }
        }
    }

    if (soundsToMove.isEmpty())
        return;

    std::sort(indicesToRemove.begin(), indicesToRemove.end(), std::greater<int>());
    for (int idx : indicesToRemove)
    {
        sourceSounds.removeAt(idx);
    }

    QVector<SoundT>& targetSounds = categories[targetCategory];

    if (targetRow < 0 || targetRow > targetSounds.size())
        targetRow = targetSounds.size();

    for (int i = 0; i < soundsToMove.size(); ++i)
    {
        targetSounds.insert(targetRow + i, soundsToMove[i]);
    }

    if (categories.contains(ALL_SOUNDS))
    {
        QVector<SoundT>& allSounds = categories[ALL_SOUNDS];
        for (const SoundT& sound : soundsToMove)
        {
            for (int i = 0; i < allSounds.size(); ++i)
            {
                if (allSounds[i].id == sound.id)
                {
                    allSounds.removeAt(i);
                    break;
                }
            }
        }

        for (const SoundT& sound : soundsToMove)
        {
            allSounds.append(sound);
        }
    }

    refreshAllModels();
}

void MainWindow::onCategoryMoved(const QString& category, int fromRow, int toRow)
{
    categoryModel->moveCategory(fromRow, toRow);
}

void MainWindow::onSoundsMovedToCategory(const QStringList& soundIds,
                                         const QString& targetCategory)
{
    if (soundIds.isEmpty() || targetCategory.isEmpty() ||
        targetCategory == ALL_SOUNDS)
        return;

    QString sourceCategory = tableModel->getCurrentCategory();

    if (sourceCategory == ALL_SOUNDS)
    {
        auto& categories = Repository::instance().getCategoriesData();
        QSet<int> soundIdSet;
        for (const QString& idStr : soundIds)
        {
            bool ok;
            int id = idStr.toInt(&ok);
            if (ok) soundIdSet.insert(id);
        }

        for (auto it = categories.begin(); it != categories.end(); ++it)
        {
            if (it.key() == ALL_SOUNDS) continue;

            const QVector<SoundT>& sounds = it.value();
            for (const SoundT& sound : sounds)
            {
                if (soundIdSet.contains(sound.id))
                {
                    sourceCategory = it.key();
                    break;
                }
            }
            if (sourceCategory != ALL_SOUNDS) break;
        }
    }

    onSoundsMoved(soundIds, sourceCategory, targetCategory, -1);
}

void MainWindow::onSoundMovedWithinCategory(const QStringList& soundIds, int toRow)
{
    qDebug() << "onSoundMovedWithinCategory called";
    qDebug() << "  soundIds:" << soundIds;
    qDebug() << "  toRow:" << toRow;

    tableModel->moveSoundsWithinCategory(soundIds, toRow);

    refreshAllModels();
}

void MainWindow::removeSounds(const QStringList& soundIds)
{
    if (soundIds.isEmpty())
        return;

    auto& categories = Repository::instance().getCategoriesData();
    QString currentCategory = tableModel->getCurrentCategory();

    if (categories.contains(ALL_SOUNDS))
    {
        QVector<SoundT>& allSounds = categories[ALL_SOUNDS];
        for (const QString& idStr : soundIds)
        {
            bool ok;
            int id = idStr.toInt(&ok);
            if (!ok) continue;

            for (int i = 0; i < allSounds.size(); ++i)
            {
                if (allSounds[i].id == id)
                {
                    allSounds.removeAt(i);
                    break;
                }
            }
        }
    }

    if (currentCategory != ALL_SOUNDS && categories.contains(currentCategory))
    {
        QList<int> idsToRemove;

        for (const QString &id : soundIds)
        {
            bool ok;
            int value = id.toInt(&ok);

            if (ok)
                idsToRemove.append(value);
        }

        QVector<SoundT>& sounds = categories[currentCategory];
        for (int i = sounds.size() - 1; i >= 0; --i)
        {
            if (idsToRemove.contains(sounds[i].id))
                sounds.removeAt(i);
        }
    }

    refreshAllModels();
}
>>>>>>> origin/main
