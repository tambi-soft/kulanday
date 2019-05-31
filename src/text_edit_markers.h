#ifndef TEXT_EDIT_MARKERS_H
#define TEXT_EDIT_MARKERS_H

#include <QWidget>
#include <QTextEdit>
#include <QScrollBar>
#include <QTextBlock>
#include <QAbstractTextDocumentLayout>
#include <QPainter>
#include <QRectF>

// https://doc.qt.io/qt-5/qtwidgets-widgets-codeeditor-example.html
// https://stackoverflow.com/questions/2443358/how-to-add-lines-numbers-to-qtextedit

class TextEditMarkers : public QTextEdit
{
    Q_OBJECT
public:
    explicit TextEditMarkers(QWidget *parent = nullptr);
    
    int getFirstVisibleBlockId();
        void lineNumberAreaPaintEvent(QPaintEvent *event);
        int lineNumberAreaWidth();
    
    signals:
    
    
    public slots:
    
        void resizeEvent(QResizeEvent *e);
    
    private slots:
    
        void updateLineNumberAreaWidth(int newBlockCount);
        void updateLineNumberArea(QRectF /*rect_f*/);
        void updateLineNumberArea(int /*slider_pos*/);
        void updateLineNumberArea();
    
    private:
    
        QWidget *lineNumberArea;
    
};



class LineNumberArea : public QWidget
{
    Q_OBJECT

public:
    LineNumberArea(QTextEdit *editor);

    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);

private:
    QTextEdit *codeEditor;
};

#endif // TEXT_EDIT_MARKERS_H
