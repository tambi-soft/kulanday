#ifndef Q_DECK_ITEM_WIDGET_H
#define Q_DECK_ITEM_WIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>

#include "q_audio_list_widget.h"

class QDeckItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QDeckItemWidget(QString deck_name, QWidget *parent = nullptr);

private:
    QGridLayout *grid;
    QLabel *image_view;
    QLineEdit *name_line;
    QLineEdit *word_line;
    QLineEdit *phonetical_line;
    QLineEdit *translation_line;
    QAudioListWidget *audio_list_widget;
    
    QString randomword(int length);
    
signals:
    
public slots:
    void importImageClicked();
    void deleteImageClicked();
    
    void newAudioButtonClicked();
    void saveButtonClicked();
};

#endif // Q_DECK_ITEM_WIDGET_H
