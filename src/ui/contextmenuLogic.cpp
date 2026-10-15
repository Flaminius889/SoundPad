<<<<<<< HEAD
#include "ui/mainwindow.h"
#include "ui/categoryAdd.h"
#include "ui/categoryEdit.h"
#include "ui/addhotkey.h"
#include <QMessageBox>
#include <QKeySequence>
#include <QFileInfo>
#include <QDir>
#include <QSet>

void MainWindow::handleAddCategory()
{
    CategoryAddDialog  dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        QString categoryName = dialog.getCategoryName();
        if (!categoryName.isEmpty())
        {
            Repository::instance().addCategory(
                categoryName, dialog.getCategoryIcon());
            Repository::instance().getCurrCategoryData() = categoryName;
            refreshAllModels();
            ui->CategoryName->setText(categoryName);
        }
    }
}

void MainWindow::handleDeleteSounds(const QStringList &soundIds)
{
    if (soundIds.isEmpty())
        return;

    QString message = (soundIds.size() == 1)
                          ? "You want to delete this sound?"
                          : QString("You want to delete this %1 sounds?")
                                .arg(soundIds.size());

    if (QMessageBox::question(
            this,
            "Delete confirmation",
            message,
            QMessageBox::Yes | QMessageBox::No)
        != QMessageBox::Yes)
    {
        return;
    }

    auto& categories =
        Repository::instance().getCategoriesData();

    QString currentCategory =
        Repository::instance().getCurrCategoryData();

    QSet<int> idsToRemove;

    for (const QString& idStr : soundIds)
    {
        bool ok = false;
        int id = idStr.toInt(&ok);

        if (ok)
            idsToRemove.insert(id);
    }

    if (idsToRemove.isEmpty())
        return;

    // =====================================================
    // 1. Find sounds BEFORE deleting them
    // =====================================================

    QVector<SoundT> soundsToRemove;
    QSet<int> foundIds;

    for (const SoundCategory &category : categories)
    {
        for (const SoundT& sound : category.sounds)
        {
            if (idsToRemove.contains(sound.id) &&
                !foundIds.contains(sound.id))
            {
                soundsToRemove.append(sound);
                foundIds.insert(sound.id);
            }
        }
    }

    // Stop player + unregister hotkeys
    cleanupSoundsBeforeRemoval(soundsToRemove);

    // =====================================================
    // 2. Remove from ALL_SOUNDS
    // =====================================================

    if (Repository::instance().hasCategory(ALL_SOUNDS))
    {
        QVector<SoundT>& allSounds =
            Repository::instance().soundsForCategory(ALL_SOUNDS);

        for (int i = allSounds.size() - 1; i >= 0; --i)
        {
            if (idsToRemove.contains(allSounds[i].id))
                allSounds.removeAt(i);
        }
    }

    // =====================================================
    // 3. If deleting from ALL_SOUNDS,
    //    remove from every category
    // =====================================================

    if (currentCategory == ALL_SOUNDS)
    {
        for (SoundCategory &category : categories)
        {
            QVector<SoundT>& sounds = category.sounds;

            for (int i = sounds.size() - 1; i >= 0; --i)
            {
                if (idsToRemove.contains(sounds[i].id))
                    sounds.removeAt(i);
            }
        }
    }

    // =====================================================
    // 4. Otherwise remove from current category
    // =====================================================

    else if (Repository::instance().hasCategory(currentCategory))
    {
        QVector<SoundT>& sounds =
            Repository::instance().soundsForCategory(currentCategory);

        for (int i = sounds.size() - 1; i >= 0; --i)
        {
            if (idsToRemove.contains(sounds[i].id))
                sounds.removeAt(i);
        }
    }

    ui->countOfSelected->setText("Choosed: 0");

    refreshAllModels();
}

void MainWindow::handleDeleteCategory(const QString &categoryName)
{
    if (categoryName.isEmpty())
        return;

    if (QMessageBox::question(
            this,
            "Delete confirmation",
            QString("Delete category \"%1\"?")
                .arg(categoryName),
            QMessageBox::Yes | QMessageBox::No)
        != QMessageBox::Yes)
    {
        return;
    }

    QVector<SoundT> soundsToRemove;

    if (Repository::instance().hasCategory(categoryName))
        soundsToRemove =
            Repository::instance().soundsForCategory(categoryName);

    cleanupSoundsBeforeRemoval(soundsToRemove);

    Repository::instance().removeCategory(categoryName);

    if (Repository::instance().getCurrCategoryData()
        == categoryName)
    {
        Repository::instance()
        .setCurrCategoryData(ALL_SOUNDS);
    }

    refreshAllModels();
}

void MainWindow::handleShowAllSounds()
{
    Repository::instance().getCurrCategoryData() = ALL_SOUNDS;
    refreshAllModels();

    ui->countOfSelected->setText("Choosed: 0");
    ui->CategoryName->setText(ALL_SOUNDS);
}

void MainWindow::handleRemoveHotkey(const QString &soundId)
{
    bool ok = false;
    int id = soundId.toInt(&ok);

    if (!ok)
        return;

    m_hotkeyManager->unregisterHotkey(id);

    Repository::instance().setSoundHotkey(id, QKeySequence());

    refreshAllModels();
}

void MainWindow::handleAddHotkey(const QString &soundId)
{
    bool ok = false;
    int id = soundId.toInt(&ok);

    if (!ok)
        return;

    addHotkey dialog(id, this);

    if (dialog.exec() != QDialog::Accepted)
        return;

    QKeySequence key = dialog.keySequence();

    if (key.isEmpty())
        return;

    if (!m_hotkeyManager->registerHotkey(id, key))
    {
        QMessageBox::warning(this, "Hotkey",
                             "Unable to register this key combination "
                             "(t may already be in use by another application.).");
        return;
    }

    Repository::instance().setSoundHotkey(id, key);
}

void MainWindow::handleChangeCategory()
{
    categoryEditDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted)
    {
        QString newName = dialog.getCategoryName();

        if (!newName.isEmpty())
        {
            QString oldName = Repository::instance().getCurrCategoryData();

            if (newName != oldName)
                Repository::instance().renameCategory(oldName, newName);
            const QString resultingName =
                Repository::instance().getCurrCategoryData();
            Repository::instance().setCategoryIcon(
                resultingName, dialog.getCategoryIcon());

            refreshAllModels();
            ui->CategoryName->setText(resultingName);
        }
    }
}
=======
#include "ui/mainwindow.h"
#include "ui/categoryAdd.h"
#include "ui/categoryEdit.h"
#include "ui/addhotkey.h"
#include <QMessageBox>
#include <QKeySequence>
#include <QFileInfo>
#include <QDir>
#include <QSet>

void MainWindow::handleAddCategory()
{
    CategoryAddDialog  dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        QString categoryName = dialog.getCategoryName();
        if (!categoryName.isEmpty())
        {
            Repository::instance().addCategory(categoryName);
            Repository::instance().getCurrCategoryData() = categoryName;
            refreshAllModels();
            ui->CategoryName->setText(categoryName);
        }
    }
}

void MainWindow::handleDeleteSounds(const QStringList &soundIds)
{
    if (soundIds.isEmpty())
        return;

    QString message = (soundIds.size() == 1)
                          ? "You want to delete this sound?"
                          : QString("You want to delete this %1 sounds?")
                                .arg(soundIds.size());

    if (QMessageBox::question(
            this,
            "Delete confirmation",
            message,
            QMessageBox::Yes | QMessageBox::No)
        != QMessageBox::Yes)
    {
        return;
    }

    auto& categories =
        Repository::instance().getCategoriesData();

    QString currentCategory =
        Repository::instance().getCurrCategoryData();

    QSet<int> idsToRemove;

    for (const QString& idStr : soundIds)
    {
        bool ok = false;
        int id = idStr.toInt(&ok);

        if (ok)
            idsToRemove.insert(id);
    }

    if (idsToRemove.isEmpty())
        return;

    // =====================================================
    // 1. Find sounds BEFORE deleting them
    // =====================================================

    QVector<SoundT> soundsToRemove;
    QSet<int> foundIds;

    for (auto it = categories.cbegin();
         it != categories.cend();
         ++it)
    {
        for (const SoundT& sound : it.value())
        {
            if (idsToRemove.contains(sound.id) &&
                !foundIds.contains(sound.id))
            {
                soundsToRemove.append(sound);
                foundIds.insert(sound.id);
            }
        }
    }

    // Stop player + unregister hotkeys
    cleanupSoundsBeforeRemoval(soundsToRemove);

    // =====================================================
    // 2. Remove from ALL_SOUNDS
    // =====================================================

    if (categories.contains(ALL_SOUNDS))
    {
        QVector<SoundT>& allSounds =
            categories[ALL_SOUNDS];

        for (int i = allSounds.size() - 1; i >= 0; --i)
        {
            if (idsToRemove.contains(allSounds[i].id))
                allSounds.removeAt(i);
        }
    }

    // =====================================================
    // 3. If deleting from ALL_SOUNDS,
    //    remove from every category
    // =====================================================

    if (currentCategory == ALL_SOUNDS)
    {
        for (auto it = categories.begin();
             it != categories.end();
             ++it)
        {
            QVector<SoundT>& sounds = it.value();

            for (int i = sounds.size() - 1; i >= 0; --i)
            {
                if (idsToRemove.contains(sounds[i].id))
                    sounds.removeAt(i);
            }
        }
    }

    // =====================================================
    // 4. Otherwise remove from current category
    // =====================================================

    else if (categories.contains(currentCategory))
    {
        QVector<SoundT>& sounds =
            categories[currentCategory];

        for (int i = sounds.size() - 1; i >= 0; --i)
        {
            if (idsToRemove.contains(sounds[i].id))
                sounds.removeAt(i);
        }
    }

    ui->countOfSelected->setText("Choosed: 0");

    refreshAllModels();
}

void MainWindow::handleDeleteCategory(const QString &categoryName)
{
    if (categoryName.isEmpty())
        return;

    if (QMessageBox::question(
            this,
            "Delete confirmation",
            QString("Delete category \"%1\"?")
                .arg(categoryName),
            QMessageBox::Yes | QMessageBox::No)
        != QMessageBox::Yes)
    {
        return;
    }

    auto& categories =
        Repository::instance().getCategoriesData();

    QVector<SoundT> soundsToRemove;

    if (categories.contains(categoryName))
        soundsToRemove = categories[categoryName];

    cleanupSoundsBeforeRemoval(soundsToRemove);

    Repository::instance().removeCategory(categoryName);

    if (Repository::instance().getCurrCategoryData()
        == categoryName)
    {
        Repository::instance()
        .setCurrCategoryData(ALL_SOUNDS);
    }

    refreshAllModels();
}

void MainWindow::handleShowAllSounds()
{
    Repository::instance().getCurrCategoryData() = ALL_SOUNDS;
    refreshAllModels();

    ui->countOfSelected->setText("Choosed: 0");
    ui->CategoryName->setText(ALL_SOUNDS);
}

void MainWindow::handleRemoveHotkey(const QString &soundId)
{
    bool ok = false;
    int id = soundId.toInt(&ok);

    if (!ok)
        return;

    m_hotkeyManager->unregisterHotkey(id);

    Repository::instance().setSoundHotkey(id, QKeySequence());

    refreshAllModels();
}

void MainWindow::handleAddHotkey(const QString &soundId)
{
    bool ok = false;
    int id = soundId.toInt(&ok);

    if (!ok)
        return;

    addHotkey dialog(id, this);

    if (dialog.exec() != QDialog::Accepted)
        return;

    QKeySequence key = dialog.keySequence();

    if (key.isEmpty())
        return;

    if (!m_hotkeyManager->registerHotkey(id, key))
    {
        QMessageBox::warning(this, "Hotkey",
                             "Unable to register this key combination "
                             "(t may already be in use by another application.).");
        return;
    }

    Repository::instance().setSoundHotkey(id, key);
}

void MainWindow::handleChangeCategory()
{
    categoryEditDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted)
    {
        QString newName = dialog.getCategoryName();

        if (!newName.isEmpty())
        {
            QString oldName = Repository::instance().getCurrCategoryData();

            Repository::instance().renameCategory(oldName, newName);

            refreshAllModels();
            ui->CategoryName->setText(newName);
        }
    }
}
>>>>>>> origin/main
