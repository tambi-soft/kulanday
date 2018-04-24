#include "q_click_label.h"

QClickLabel::QClickLabel(QLabel *parent) : QLabel(parent)
{
    
}

void QClickLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit QClickLabel::clicked();
    }
    else if (event->button() == Qt::RightButton)
    {
        emit QClickLabel::rightClicked();
    }
}

