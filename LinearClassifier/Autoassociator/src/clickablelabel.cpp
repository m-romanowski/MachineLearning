#include "clickablelabel.h"

ClickableLabel::ClickableLabel(QWidget* _parent, Qt::WindowFlags _flags)
    : QLabel(_parent)
{
    this->setWindowFlags(_flags);
}

void ClickableLabel::mousePressEvent(QMouseEvent* _event)
{
    Q_UNUSED(_event);
    emit clicked();
}
