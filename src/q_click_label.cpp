#include "q_click_label.h"

QClickLabel::QClickLabel(QWidget *parent) : QWidget(parent)
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

