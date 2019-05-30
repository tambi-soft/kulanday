#ifndef TEXT_EDIT_MARKERS_H
#define TEXT_EDIT_MARKERS_H

#include <QWidget>
#include <QTextEdit>
#include <QHBoxLayout>

class TextEditMarkers : public QWidget
{
    Q_OBJECT
public:
    explicit TextEditMarkers(QWidget *parent = nullptr);
    
private:
    QHBoxLayout *layout;
    
signals:
    void textChanged();
    
public slots:
};

#endif // TEXT_EDIT_MARKERS_H
