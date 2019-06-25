#ifndef TEXT_EDIT_MARKERS_H
#define TEXT_EDIT_MARKERS_H

// https://doc.qt.io/qt-5/qtwidgets-widgets-codeeditor-example.html
// https://stackoverflow.com/questions/2443358/how-to-add-lines-numbers-to-qtextedit

#include <QObject>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QTextCursor>

#include <QDebug>

class MarkersTextEdit;
class PayloadTextEdit;

class TextEditMarkers : public QWidget
{
    Q_OBJECT
    
public:
    TextEditMarkers(QWidget *parent = nullptr);
    
    MarkersTextEdit *markers;
    //PayloadTextEdit *payload;
    QPlainTextEdit *payload;
    
private:
    QHBoxLayout *layout;
    void setMarker(QString marker, int position, int payload_lines_count);
    
private slots:
    /* analyze the input for markers (i.e.: something like "\lx" to create the markers entry */
    void onPayloadTextChanged();
    
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
