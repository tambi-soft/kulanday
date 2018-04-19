#ifndef Q_CLICK_LABEL_H
#define Q_CLICK_LABEL_H

#include <QWidget>
#include <QMouseEvent>

class QClickLabel : public QWidget
{
    Q_OBJECT
public:
    explicit QClickLabel(QWidget *parent = nullptr);
    
signals:
    void clicked();
    void rightClicked();
    
public slots:
    void mousePressEvent(QMouseEvent *event);
};

#endif // Q_CLICK_LABEL_H
