#pragma once

#include <QByteArray>
#include <QPixmap>
#include <QSize>
#include <QString>

QPixmap createCategoryIcon(const QString &categoryName,
                           const QByteArray &customIcon,
                           const QSize &size);
