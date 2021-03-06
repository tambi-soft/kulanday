#ifndef Q_DECK_OVERVIEW_H
#define Q_DECK_OVERVIEW_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QScrollArea>
#include <QScrollBar>
#include <QMessageBox>
#include <QIcon>
#include <QFont>
#include <QLabel>
#include <QPixmap>
#include <QSvgWidget>

#include <QMediaPlayer>
#include <QMediaContent>
#include <QDir>
#include <QUrl>

#include <QFile>

#include "db_adapter.h"
#include "unicode_fonts.h"
#include "move_item_dialog.h"

class QDeckOverviewWidget : public QWidget
{
    Q_OBJECT
public:
    /* to initialize the search result view */
    explicit QDeckOverviewWidget(QString filter, QDir *decks_path, QWidget *parent = nullptr);
    
    /* open existing deck */
    /* language_filter is for passing it to the move_item_dialog */
    explicit QDeckOverviewWidget(QDir *decks_path, QString deck_name, Config *config, QWidget *parent = nullptr);
    
    void refresh();
    
private:
    int BUTTON_WIDTH = 25;
    Config *config;
    bool searchMode;
    QGridLayout *layout;
    QGridLayout *grid; // inner layout containing the qscrollarea
    QScrollArea *scroll_area = new QScrollArea;
    QWidget *scroll_widget = nullptr;
    
    QDir *decks_path;
    QString deck_name;
    QMediaPlayer *player;
    QPushButton *playing_button = nullptr;
    DbAdapter *database;
    UnicodeFonts *unicodeFonts;
    
    DbAdapter *db_adapter;
    
    int COLUMN_OFFSET = 11;
    int max_audio_count = 0;
    int scrollbar_pos;
    
    QCheckBox *chk_name;
    QCheckBox *chk_word;
    QCheckBox *chk_phonetical;
    QCheckBox *chk_translation;
    QCheckBox *chk_svg;
    QCheckBox *chk_image;
    
    void initGui();
    QList<QMap<QString, QVariant> > fetchDeckData();
    void populateTableWidget(QList<QMap<QString,QVariant>> data);
    void appendPlayButtons(int table_rowid, QList<QMap<QString,QVariant>> data, QString deck_name);
    void audioButtonClicked(QPushButton *button, QString audio_filename, QString deck_name);
    void moveItem(QString deck_name, qlonglong rowid);
    QString cropText(QString text);
    void deleteRow(qlonglong rowid, QString deck_name);
    
protected:
    void hideEvent(QHideEvent *event);
    void showEvent(QShowEvent *event);
    
signals:
    void newDeckItemRequested(QString deck_name);
    void showDeckItemRequested(QString deck_name, int deck_id);
    
public slots:
    
private slots:
    void newItemButtonClicked();
    void refreshTable();
    void editRowButtonClicked(QString deck_name, int rowid);
    void deleteRowButtonClicked(qlonglong rowid, QMap<QString, QVariant> data_row, QString deck_name);
    void mediaPlayerStateChanged(int state);
    void scrollBarRangeChanged(int /*min*/, int max);
};

#endif // Q_DECK_OVERVIEW_H
