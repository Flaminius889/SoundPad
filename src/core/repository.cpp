<<<<<<< HEAD
#include "core/repository.h"

#include "services/FileManager.h"

#include <algorithm>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>
#include <QStandardPaths>
#include <QTextStream>

namespace
{
QJsonObject soundToJson(const SoundT &sound)
{
    return {{"category", sound.category}, {"id", sound.id},
            {"name", sound.nameOfMusic},
            {"duration", static_cast<double>(sound.duration)},
            {"hotkey", sound.hotkey}, {"path", sound.path},
            {"data", sound.data}};
}

SoundT soundFromJson(const QJsonObject &object)
{
    SoundT sound;
    sound.category = object.value("category").toString();
    sound.id = object.value("id").toInt();
    sound.nameOfMusic = object.value("name").toString();
    sound.duration = static_cast<qint64>(object.value("duration").toDouble());
    sound.hotkey = object.value("hotkey").toString();
    sound.path = object.value("path").toString();
    sound.data = object.value("data").toString();
    return sound;
}
}

Repository::Repository()
    : currentCategory(ALL_SOUNDS)
{
    SoundCategory category;
    category.name = ALL_SOUNDS;
    categories.append(category);
}

Repository& Repository::instance()
{
    static Repository instance;
    return instance;
}

QVector<SoundCategory> &Repository::getCategoriesData()
{
    return categories;
}

const QVector<SoundCategory> &Repository::getCategoriesData() const
{
    return categories;
}

QString &Repository::getCurrCategoryData()
{
    return currentCategory;
}

QStringList Repository::getCategoryOrder() const
{
    QStringList result;
    for (const SoundCategory &category : categories)
        result.append(category.name);
    return result;
}

bool Repository::hasCategory(const QString &name) const
{
    return categoryByName(name) != nullptr;
}

SoundCategory *Repository::categoryByName(const QString &name)
{
    for (SoundCategory &category : categories)
        if (category.name == name)
            return &category;
    return nullptr;
}

const SoundCategory *Repository::categoryByName(const QString &name) const
{
    for (const SoundCategory &category : categories)
        if (category.name == name)
            return &category;
    return nullptr;
}

SoundCategory *Repository::categoryById(const QUuid &id)
{
    for (SoundCategory &category : categories)
        if (category.id == id)
            return &category;
    return nullptr;
}

const SoundCategory *Repository::categoryById(const QUuid &id) const
{
    for (const SoundCategory &category : categories)
        if (category.id == id)
            return &category;
    return nullptr;
}

QVector<SoundT> &Repository::soundsForCategory(const QString &name)
{
    SoundCategory *category = categoryByName(name);
    if (!category)
    {
        addCategory(name);
        category = categoryByName(name);
    }
    if (!category)
        category = categoryByName(ALL_SOUNDS);
    return category->sounds;
}

const QVector<SoundT> &Repository::soundsForCategory(const QString &name) const
{
    static const QVector<SoundT> empty;
    const SoundCategory *category = categoryByName(name);
    return category ? category->sounds : empty;
}

void Repository::setCurrCategoryData(const QString &name)
{
    if (hasCategory(name))
        currentCategory = name;
}

void Repository::setCategoryOrder(const QStringList &order)
{
    QVector<SoundCategory> reordered;
    reordered.reserve(categories.size());

    for (const QString &name : order)
    {
        const SoundCategory *category = categoryByName(name);
        if (category)
            reordered.append(*category);
    }

    for (const SoundCategory &category : std::as_const(categories))
    {
        const auto found = std::find_if(
            reordered.cbegin(), reordered.cend(),
            [&category](const SoundCategory &item) { return item.id == category.id; });
        if (found == reordered.cend())
            reordered.append(category);
    }

    categories = std::move(reordered);
}

void Repository::renameCategory(const QString &oldName, const QString &newName)
{
    if (oldName == ALL_SOUNDS || newName.isEmpty() || hasCategory(newName))
        return;

    SoundCategory *category = categoryByName(oldName);
    if (!category)
        return;

    category->name = newName;
    for (SoundT &sound : category->sounds)
        sound.category = newName;

    if (currentCategory == oldName)
        currentCategory = newName;
}

void Repository::addSound(const QString &categoryName, const SoundT &sound)
{
    if (SoundCategory *category = categoryByName(categoryName))
        category->sounds.append(sound);
}

void Repository::addCategory(const QString &name, const QByteArray &icon)
{
    if (name.isEmpty() || hasCategory(name))
        return;

    SoundCategory category;
    category.name = name;
    category.icon = icon;
    categories.append(category);
}

void Repository::setCategoryIcon(const QString &name, const QByteArray &icon)
{
    if (SoundCategory *category = categoryByName(name))
        category->icon = icon;
}

void Repository::removeCategory(const QString &name)
{
    if (name == ALL_SOUNDS)
        return;

    for (int i = 0; i < categories.size(); ++i)
        if (categories[i].name == name)
            return categories.removeAt(i);
}

void Repository::removeSound(const QString &categoryName, int index)
{
    SoundCategory *category = categoryByName(categoryName);
    if (category && index >= 0 && index < category->sounds.size())
        category->sounds.removeAt(index);
}

void Repository::setSoundHotkey(int soundId, const QKeySequence &hotkey)
{
    for (SoundCategory &category : categories)
        for (SoundT &sound : category.sounds)
            if (sound.id == soundId)
                sound.hotkey = hotkey.toString();
}

QKeySequence Repository::getSoundHotkey(int soundId) const
{
    for (const SoundCategory &category : categories)
        for (const SoundT &sound : category.sounds)
            if (sound.id == soundId)
                return sound.hotkey;
    return {};
}

void Repository::moveCategory(int fromRow, int toRow)
{
    if (fromRow >= 0 && fromRow < categories.size()
        && toRow >= 0 && toRow < categories.size())
        categories.move(fromRow, toRow);
}

QString Repository::dataPath()
{
    const QString path = QStandardPaths::writableLocation(
        QStandardPaths::AppDataLocation);
    QDir().mkpath(path);
    return path;
}

void Repository::deleteFiles()
{
    QFile::remove(dataPath() + "/categories.json");
}

void Repository::SaveData()
{
    QJsonArray categoryArray;
    for (const SoundCategory &category : std::as_const(categories))
    {
        QJsonArray sounds;
        for (const SoundT &sound : category.sounds)
            sounds.append(soundToJson(sound));

        QJsonObject object;
        object.insert("id", category.id.toString(QUuid::WithoutBraces));
        object.insert("name", category.name);
        object.insert("icon", QString::fromLatin1(category.icon.toBase64()));
        object.insert("sounds", sounds);
        categoryArray.append(object);
    }

    QJsonObject root;
    root.insert("version", 2);
    root.insert("categories", categoryArray);

    QSaveFile file(dataPath() + "/categories.json");
    if (!file.open(QIODevice::WriteOnly))
        return;
    file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    file.commit();
}

bool Repository::ReadData()
{
    QFile jsonFile(dataPath() + "/categories.json");
    if (jsonFile.open(QIODevice::ReadOnly))
    {
        const QJsonArray array = QJsonDocument::fromJson(jsonFile.readAll())
                                     .object().value("categories").toArray();
        QVector<SoundCategory> loaded;
        for (const QJsonValue &value : array)
        {
            const QJsonObject object = value.toObject();
            SoundCategory category;
            category.id = QUuid(object.value("id").toString());
            if (category.id.isNull())
                category.id = QUuid::createUuid();
            category.name = object.value("name").toString();
            category.icon = QByteArray::fromBase64(
                object.value("icon").toString().toLatin1());
            for (const QJsonValue &sound : object.value("sounds").toArray())
                category.sounds.append(soundFromJson(sound.toObject()));
            if (!category.name.isEmpty())
                loaded.append(category);
        }

        if (!loaded.isEmpty())
        {
            categories = std::move(loaded);
            if (!hasCategory(ALL_SOUNDS))
            {
                SoundCategory allSounds;
                allSounds.name = ALL_SOUNDS;
                categories.prepend(allSounds);
            }
            return true;
        }
    }

    QFile legacyFile(dataPath() + "/category_names.txt");
    if (!legacyFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    categories.clear();
    QTextStream in(&legacyFile);
    while (!in.atEnd())
    {
        const QString name = in.readLine().trimmed();
        if (name.isEmpty() || hasCategory(name))
            continue;

        SoundCategory category;
        category.name = name;
        FileManager::readFile(dataPath() + "/" + name + "-sound_list.txt",
                              category.sounds);
        categories.append(category);
    }

    if (!hasCategory(ALL_SOUNDS))
    {
        SoundCategory allSounds;
        allSounds.name = ALL_SOUNDS;
        categories.prepend(allSounds);
    }

    return !categories.isEmpty();
}
=======
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
>>>>>>> origin/main
