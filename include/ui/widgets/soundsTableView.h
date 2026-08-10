#ifndef SOUNDSTABLEVIEW_H
#define SOUNDSTABLEVIEW_H

#include <QTableView>
#include <QMouseEvent>

class SoundsTableView : public QTableView
{
    Q_OBJECT

public:
    using QTableView::QTableView;
    explicit SoundsTableView(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
};

#endif // SOUNDSTABLEVIEW_H
