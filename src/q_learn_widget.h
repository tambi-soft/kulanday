#ifndef Q_LEARN_WIDGET_H
#define Q_LEARN_WIDGET_H

#include <QWidget>
#include <QDir>

class QLearnWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QLearnWidget(QDir *deckpatch, QString deck_name, QWidget *parent = nullptr);
    
signals:
    
public slots:
};

#endif // Q_LEARN_WIDGET_H
