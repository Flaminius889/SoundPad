#ifndef SOUNDCATEGORY_H
#define SOUNDCATEGORY_H

#include <QByteArray>
#include <QString>
#include <QUuid>
#include <QVector>

#include "core/Sound.h"

struct SoundCategory
{
    QUuid id = QUuid::createUuid();
    QString name;
    QByteArray icon;
    QVector<SoundT> sounds;
};

#endif // SOUNDCATEGORY_H
