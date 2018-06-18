#ifndef RESIZING_TEXT_EDIT_H
#define RESIZING_TEXT_EDIT_H

#include <QTextEdit>
#include <QSize>
#include <QMargins>
#include <QFontMetrics>
#include <QTextDocument>
#include <QSizePolicy>
#include <QDebug>

class ResizingTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit ResizingTextEdit(QWidget *parent = nullptr);

    void updateGeometry();
    
protected:
    void paintEvent(QPaintEvent *e);
    
signals:
    
public slots:
};

#endif // RESIZING_TEXT_EDIT_H
