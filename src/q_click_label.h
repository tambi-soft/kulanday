#ifndef Q_CLICK_LABEL_H
#define Q_CLICK_LABEL_H

#include <QLabel>
#include <QMouseEvent>

class QClickLabel : public QLabel
{
    Q_OBJECT
public:
    explicit QClickLabel(QLabel *parent = nullptr);
    
signals:
    void clicked();
    void rightClicked();
    
public slots:
    void mousePressEvent(QMouseEvent *event);
};

#endif // Q_CLICK_LABEL_H
