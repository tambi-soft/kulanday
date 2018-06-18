#ifndef Q_DECK_ITEM_WIDGET_H
#define Q_DECK_ITEM_WIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QIcon>
#include <QLabel>
#include <QPixmap>
#include <QDir>
#include <QSize>
#include <QSysInfo>
#include <QSvgWidget>
#include <QMessageBox>
#include <QFileDialog>

#include "db_adapter.h"
#include "audio_list_table.h"
#include "resizing_text_edit.h"

class QDeckItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QDeckItemWidget(QDir *decks_path, QString deck_name, QWidget *parent = nullptr);
    explicit QDeckItemWidget(QDir *decks_path, QString deck_name, int rowid, QWidget *parent = nullptr);
    
    int rowid;
    
private:
    QDir *decks_path;
    QString deck_name;
    QString image_path;
    
    QGridLayout *grid;
    QLabel *image_view;
    ResizingTextEdit *name_line;
    ResizingTextEdit *word_line;
    ResizingTextEdit *phonetical_line;
    ResizingTextEdit *translation_line;
    QAudioListTable *audio_list_widget;
    DbAdapter *database = nullptr;
    QPushButton *import_image_button;
    
    bool ignore_item_changes;
    bool item_changed = false;
    
    QUrl default_import_path = QDir::homePath();
    
    QSize IMAGE_SIZE = QSize(300, 150); // 600 300
    
    void populateGui(QDir *decks_path, QString deck_name, int rowid);
    void initializeGui(QString deck_name, int rowid);
    
    QString randomword(int length);
    
protected:
    void hideEvent(QHideEvent *event);
    
signals:
    void contentsUpdated(QString deck_name);
    
public slots:
    void importImageClicked();
    void deleteImageClicked();
    
    void newAudioButtonClicked();
    void onItemChanged();
    
private slots:
    void onAudioListChanged();
};

#endif // Q_DECK_ITEM_WIDGET_H