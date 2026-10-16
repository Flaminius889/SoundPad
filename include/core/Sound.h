#pragma once
#include <QtCore>

struct SoundT
{
    QString category;
    int id;
    QString nameOfMusic;
    qint64 duration = 0;
    QString hotkey;
    QString path;
    QString data;

    SoundT() {};
    SoundT(QString categ,int index, QString n, QString hk, QString p, QString d) : category(categ), id(index),
                                                                nameOfMusic(std::move(n)), hotkey(hk), path(std::move(p)), data(std::move(d)) {};

    bool operator==(const SoundT& other) const {
        return id == other.id;
    }
#pragma once
#include <QtCore>

struct SoundT
{
    QString category;
    int id;
    QString nameOfMusic;
    qint64 duration = 0;
    QString hotkey;
    QString path;
    QString data;

    SoundT() {};
    SoundT(QString categ,int index, QString n, QString hk, QString p, QString d) : category(categ), id(index),
                                                                nameOfMusic(std::move(n)), hotkey(hk), path(std::move(p)), data(std::move(d)) {};

    bool operator==(const SoundT& other) const {
        return id == other.id;
    }
};