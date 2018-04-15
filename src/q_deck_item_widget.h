#ifndef Q_DECK_ITEM_WIDGET_H
#define Q_DECK_ITEM_WIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QDir>
#include <QSize>
#include <QSysInfo>
#include <QSvgWidget>

#include "db_adapter.h"
#include "q_audio_list_table.h"

class QDeckItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QDeckItemWidget(QString deck_name, QWidget *parent = nullptr);
    explicit QDeckItemWidget(QString deck_name, int rowid, QWidget *parent = nullptr);
    
private:
    int rowid;
    QGridLayout *grid;
    QLabel *image_view;
    QLineEdit *name_line;
    QLineEdit *word_line;
    QLineEdit *phonetical_line;
    QLineEdit *translation_line;
    QAudioListTable *audio_list_widget;
    DbAdapter *database = nullptr;
    
    bool ignore_item_changes;
    
    QSize IMAGE_SIZE = QSize(300, 150); // 600 300
    
    void initializeGui(QString deck_name, int rowid);
    
    QString randomword(int length);
    
signals:
    
public slots:
    void importImageClicked();
    void deleteImageClicked();
    
    void newAudioButtonClicked();
    void onItemChanged();
};

#endif // Q_DECK_ITEM_WIDGET_H
