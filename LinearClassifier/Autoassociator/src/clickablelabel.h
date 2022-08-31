#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QWidget>
#include <QLabel>
#include <Qt>

class ClickableLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget* _parent = nullptr, Qt::WindowFlags _flags = Qt::WindowFlags());

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* _event);
};

#endif // CLICKABLELABEL_H
