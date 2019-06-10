#ifndef TEXT_EDIT_MARKERS_H
#define TEXT_EDIT_MARKERS_H

// https://doc.qt.io/qt-5/qtwidgets-widgets-codeeditor-example.html
// https://stackoverflow.com/questions/2443358/how-to-add-lines-numbers-to-qtextedit

#include <QObject>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QHBoxLayout>

class MarkersTextEdit;
class PayloadTextEdit;

class TextEditMarkers : public QWidget
{
    Q_OBJECT
    
public:
    TextEditMarkers(QWidget *parent = nullptr);
    
    MarkersTextEdit *markers;
    PayloadTextEdit *payload;
    
private:
    QHBoxLayout *layout;
    
private slots:
    
    
signals:
    void textChanged();
};


class MarkersTextEdit : public QTextEdit
{
    Q_OBJECT
    
public:
    MarkersTextEdit(QWidget *parent = nullptr);
    
private:
    
    
private slots:
    
    
};

class PayloadTextEdit : public QTextEdit
{
    Q_OBJECT
    
public:
    PayloadTextEdit(QWidget *parent = nullptr);
    
private:
    
    
private slots:
    
    
};
        

#endif // TEXT_EDIT_MARKERS_H
