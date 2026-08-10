#include "core/repository.h"
#include "services/FileManager.h"
#include <QStandardPaths>
#include <QDir>

Repository::Repository()
{
    currentCategory = ALL_SOUNDS;

    categories.insert(ALL_SOUNDS, {});
}

Repository& Repository::instance()
{
    static Repository instance;
    return instance;
}

QMap<QString, QVector<SoundT>> &Repository::getCategoriesData()
{
    return categories;
}

QString &Repository::getCurrCategoryData()
{
    return currentCategory;
}

QStringList &Repository::getCategoryOrder()
{
    return categoryOrder;
}

void Repository::setCurrCategoryData(const QString &CategoryName)
{
    currentCategory = CategoryName;
}

void Repository::setCategoryOrder(const QStringList &CategoryOrder)
{
    categoryOrder = CategoryOrder;
}

void Repository::renameCategory(const QString &oldName,
                                const QString &newName)
{
    if (!categories.contains(oldName))
        return;

    if (categories.contains(newName))
        return;

    categories.insert(newName, categories.take(oldName));

    int index = categoryOrder.indexOf(oldName);
    if (index != -1)
        categoryOrder[index] = newName;

    if (currentCategory == oldName)
        currentCategory = newName;
}

void Repository::addSound(const QString& category, const SoundT& sound)
{
    if (categories.contains(category))
        categories[category].append(sound);
}

void Repository::addCategory(const QString &name)
{
    if (categories.contains(name) || name.isEmpty())
        return;

    categories[name] = {};
    categoryOrder.append(name);
}

void Repository::removeCategory(const QString &name)
{
    categories.remove(name);
    categoryOrder.removeAll(name);
}

void Repository::removeSound(const QString& category, int index)
{
    if (categories.contains(category) &&
        index >= 0 && index < categories[category].size())
        categories[category].removeAt(index);
}

void Repository::setSoundHotkey(int soundId, const QKeySequence &hotkey)
{
    for (auto it = categories.begin(); it != categories.end(); ++it)
    {
        QVector<SoundT> &sounds = it.value();

        for (SoundT &sound : sounds)
        {
            if (sound.id == soundId)
                sound.hotkey =  hotkey.toString();
        }
    }
}

QKeySequence Repository::getSoundHotkey(int soundId) const
{
    for (auto it = categories.constBegin(); it != categories.constEnd(); ++it)
    {
        for (const SoundT &sound : it.value())
        {
            if (sound.id == soundId)
                return sound.hotkey;
        }
    }

    return QKeySequence();
}

void Repository::moveCategory(int fromRow, int toRow)
{
    if (fromRow < 0 || fromRow >= categoryOrder.size())
        return;

    if (toRow < 0 || toRow >= categoryOrder.size())
        return;

    categoryOrder.move(fromRow, toRow);
}

QString Repository::dataPath()
{
    QString path = QStandardPaths::writableLocation(
        QStandardPaths::AppDataLocation
        );

    QDir dir(path);

    if (!dir.exists())
        dir.mkpath(".");

    return path;
}

void Repository::deleteFiles()
{
    QDir dir(dataPath());

    QStringList filesToDelete  = {"All Sounds-sound_list.txt", "category_names.txt"};

    for(const QString &fileName : filesToDelete)
    {
        QString path = dir.filePath(fileName);
        QFile file(path);

        if(QFile::remove(path)) {
            qDebug() << "Deleted:" << path;
        } else {
            qDebug() << "Failed to delete:" << path
                     << "Reason:" << '\n' << file.errorString();
        }
    }

    QDirIterator it(dir.absolutePath(), QDir::Files);

    while(it.hasNext())
    {
        QString filePath = it.next();
        QFile file(filePath);
        QFileInfo info(filePath);

        if(info.fileName().contains("-sound_list.txt")) {
            if(QFile::remove(filePath)) {
                qDebug() << "Deleted:" << filePath;
            } else {
                qDebug() << "Failed to delete:" << filePath
                         << "Reason:" << '\n' << file.errorString();
            }
        }
    }
}

void Repository::SaveData()
{
    deleteFiles();

    QFile writeFile(dataPath() + "/category_names.txt");
    qDebug() << dataPath();

    if (writeFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&writeFile);

        for (auto it = categories.begin(); it != categories.end(); ++it)
        {
            out << it.key() << '\n';
            qDebug() << it.key();

            QString fileName = dataPath() + "/"
                               + it.key()
                               + "-sound_list.txt";
            FileManager::saveList(fileName, it.value());
        }

        writeFile.close();
    }
}
bool Repository::ReadData()
{
    QFile file(dataPath() + "/category_names.txt");
    qDebug() << dataPath();

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);

        categories.clear();
        categoryOrder.clear();

        while (!in.atEnd())
        {
            QString categoryName = in.readLine();
            QString fileName = dataPath() + "/" + categoryName + "-" + "sound_list.txt";
            FileManager::readFile(categoryName, fileName, categories);
        }

        file.close();
        return true;

    } else {
        qDebug() << "Failed to open file:" << "category_names.txt";
        return false;
    }

    return !categories.isEmpty();
}