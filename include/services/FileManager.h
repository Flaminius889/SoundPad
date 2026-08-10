#pragma once

#include <QVector>
#include "core/Sound.h"

class FileManager
{
public:
    static void saveList(const QString& filename,
                         const QVector<SoundT>& sounds);

    static void readFile(const QString &CategoryName, const QString &filename,
                         QMap<QString, QVector<SoundT>> &categories);

    static void parseLine(const QString& line,
                          SoundT& sound);
};