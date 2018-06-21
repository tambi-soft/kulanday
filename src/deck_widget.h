#ifndef Q_DECK_OVERVIEW_H
#define Q_DECK_OVERVIEW_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
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

class QDeckOverviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QDeckOverviewWidget(QString filter, QDir *decks_path, QWidget *parent = nullptr);
    explicit QDeckOverviewWidget(QDir *decks_path, QString deck_name, QWidget *parent = nullptr);
    
    void refresh();
    
private:
    bool searchMode;
    QGridLayout *layout;
    QTableWidget *table;
    QDir *decks_path;
    QString deck_name;
    QMediaPlayer *player;
    QPushButton *playing_button = nullptr;
    DbAdapter *database;
    UnicodeFonts *unicodeFonts;
    
    DbAdapter *db_adapter;
    
    int COLUMN_OFFSET = 11;
    
    QCheckBox *chk_name;
    QCheckBox *chk_word;
    QCheckBox *chk_phonetical;
    QCheckBox *chk_translation;
    QCheckBox *chk_svg;
    QCheckBox *chk_image;
    
    void initGui();
    QList<QMap<QString, QVariant> > fetchDeckData();
    void populateTableWidget(QList<QMap<QString,QVariant>> data);
    void appendPlayButtons(int table_rowid, QList<QMap<QString,QVariant>> audio_filenames, int max_audio_count, QString deck_name);
    void audioButtonClicked(QPushButton *button, QString audio_filename, QString deck_name);
    void moveItem(QString deck_name, qlonglong rowid);
    QString cropText(QString text);
    
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
    void deleteRowButtonClicked(int rowid, QMap<QString, QVariant> data_row, QString deck_name);
    void mediaPlayerStateChanged(int state);
};

#endif // Q_DECK_OVERVIEW_H
