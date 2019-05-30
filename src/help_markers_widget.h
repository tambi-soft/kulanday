#ifndef HELP_MARKERS_WIDGET_H
#define HELP_MARKERS_WIDGET_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>

class HelpMarkersWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HelpMarkersWidget(QWidget *parent = nullptr);
    
private:
    QVBoxLayout *layout;
    QTextEdit *show_text;
    
signals:
    
public slots:
};

#endif // HELP_MARKERS_WIDGET_H
