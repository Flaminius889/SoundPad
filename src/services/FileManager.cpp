#include "services/FileManager.h"
#include <QFile>
#include <QTextStream>
#include <QSet>
#include <QDebug>

void FileManager::saveList(const QString& filename,
                           const QVector<SoundT>& sounds)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file:" << filename;
        return;
    }

    QTextStream out(&file);
    for (const auto& sound : sounds)
    {
        out << sound.category << ';'
            << sound.id << ';'
            << sound.nameOfMusic << ';'
            << sound.duration << ';'
            << sound.path << ';'
            << sound.data << ';'
            << sound.hotkey << '\n';
    }
    file.close();
}

void FileManager::readFile(const QString &filename,
                           QVector<SoundT> &sounds)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file in readFile:" << filename;
        return;
    }

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line = in.readLine();

        if (line.isEmpty())
            continue;

        SoundT sound;

        parseLine(line, sound);

        sounds.push_back(sound);
    }

    file.close();
}

void  FileManager::parseLine(const QString& line,
                            SoundT& sound)
{
    QStringList parts = line.split(';');

    if (parts.size() < 7)
        return;

    sound.category = parts[0];
    sound.id = parts[1].toInt();
    sound.nameOfMusic = parts[2];
    sound.duration = parts[3].toLongLong();
    sound.path = parts[4];
    sound.data = parts[5];
    sound.hotkey = parts[6];
}
#include "services/FileManager.h"
#include <QFile>
#include <QTextStream>
#include <QSet>
#include <QDebug>

void FileManager::saveList(const QString& filename,
                           const QVector<SoundT>& sounds)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file:" << filename;
        return;
    }

    QTextStream out(&file);
    for (const auto& sound : sounds)
    {
        out << sound.category << ';'
            << sound.id << ';'
            << sound.nameOfMusic << ';'
            << sound.duration << ';'
            << sound.path << ';'
            << sound.data << ';'
            << sound.hotkey << '\n';
    }
    file.close();
}

void  FileManager::readFile(const QString &CategoryName, const QString &filename,
                           QMap<QString, QVector<SoundT>> &categories)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file in readFile:" << filename;
        return;
    }

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line = in.readLine();

        if (line.isEmpty())
            continue;

        SoundT sound;

        parseLine(line, sound);

        categories[CategoryName].push_back(sound);
    }

    file.close();
}

void  FileManager::parseLine(const QString& line,
                            SoundT& sound)
{
    QStringList parts = line.split(';');

    if (parts.size() < 7)
        return;

    sound.category = parts[0];
    sound.id = parts[1].toInt();
    sound.nameOfMusic = parts[2];
    sound.duration = parts[3].toLongLong();
    sound.path = parts[4];
    sound.data = parts[5];
    sound.hotkey = parts[6];
}
