#include "ui/categoryIcon.h"

#include <QColor>
#include <QFont>
#include <QPainter>

QPixmap createCategoryIcon(const QString &categoryName,
                           const QByteArray &customIcon,
                           const QSize &size)
{
    QPixmap customPixmap;
    if (!customIcon.isEmpty() && customPixmap.loadFromData(customIcon))
        return customPixmap.scaled(size, Qt::KeepAspectRatio,
                                   Qt::SmoothTransformation);

    QPixmap letterPixmap(size);
    letterPixmap.fill(Qt::transparent);

    QPainter painter(&letterPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#0A64D8"));
    painter.drawRoundedRect(letterPixmap.rect().adjusted(1, 1, -1, -1),
                            size.width() * 0.18,
                            size.height() * 0.18);

    QFont font("Segoe UI");
    font.setBold(true);
    font.setPixelSize(qMax(10, qRound(size.height() * 0.58)));
    painter.setFont(font);
    painter.setPen(Qt::white);

    const QString trimmedName = categoryName.trimmed();
    const QString letter = trimmedName.isEmpty()
        ? QStringLiteral("?")
        : trimmedName.left(1).toUpper();
    painter.drawText(letterPixmap.rect(), Qt::AlignCenter, letter);

    return letterPixmap;
}
