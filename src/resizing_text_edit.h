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
    explicit ResizingTextEdit(QTextEdit *parent = nullptr);
    /*
    void setMinimumLines(int num_lines);
    bool hasHeightForWidth();
    int heightForWidth(int width);
    QSize sizeHint();
    int lineCountToWidgetHeight(int num_lines);
    */
protected:
    //void paintEvent(QPaintEvent *e);
    
signals:
    
public slots:
};

#endif // RESIZING_TEXT_EDIT_H
