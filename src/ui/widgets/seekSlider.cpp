#include "ui/widgets/seekSlider.h"

#include <QMouseEvent>
#include <QStyle>
#include <QStyleOptionSlider>

SeekSlider::SeekSlider(QWidget *parent)
    : QSlider(parent)
{
}

void SeekSlider::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
    {
        QSlider::mousePressEvent(event);
        return;
    }

    QStyleOptionSlider option;
    initStyleOption(&option);

    const QRect handle = style()->subControlRect(
        QStyle::CC_Slider,
        &option,
        QStyle::SC_SliderHandle,
        this);

    if (handle.contains(event->position().toPoint()))
    {
        QSlider::mousePressEvent(event);
        return;
    }

    const QRect groove = style()->subControlRect(
        QStyle::CC_Slider,
        &option,
        QStyle::SC_SliderGroove,
        this);

    const int clickPosition = orientation() == Qt::Horizontal
        ? event->position().toPoint().x() - groove.left()
        : groove.bottom() - event->position().toPoint().y();
    const int span = orientation() == Qt::Horizontal
        ? groove.width()
        : groove.height();
    const int newValue = QStyle::sliderValueFromPosition(
        minimum(), maximum(), clickPosition, span, option.upsideDown);

    setValue(newValue);
    emit seekRequested(newValue);
    event->accept();
}
