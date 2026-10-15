#ifndef SEEKSLIDER_H
#define SEEKSLIDER_H

#include <QSlider>

class SeekSlider : public QSlider
{
    Q_OBJECT

public:
    explicit SeekSlider(QWidget *parent = nullptr);

signals:
    void seekRequested(int position);

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // SEEKSLIDER_H
