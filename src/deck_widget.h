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
    explicit QDeckOverviewWidget(QDir *decks_path, QString deck_name, QWidget *parent = nullptr);
    
    void refresh();
    
private:
    QGridLayout *layout;
    QTableWidget *table;
    QDir *decks_path;
    QString deck_name;
    QMediaPlayer *player;
    QPushButton *playing_button = nullptr;
    DbAdapter *database;
    UnicodeFonts *unicodeFonts;
    
    int COLUMN_OFFSET = 10;
    
    QCheckBox *chk_name;
    QCheckBox *chk_word;
    QCheckBox *chk_phonetical;
    QCheckBox *chk_translation;
    QCheckBox *chk_svg;
    QCheckBox *chk_image;
    
    void initTableWidget(QString deck_name);
    void appendPlayButtons(int table_rowid, QList<QMap<QString,QVariant>> audio_filenames, int max_audio_count);
    void audioButtonClicked(QPushButton *button, QString audio_filename);
    void moveItem(QString deck_name, qlonglong rowid);
    
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
    void deleteRowButtonClicked(int rowid, QMap<QString, QVariant> data_row);
    void mediaPlayerStateChanged(int state);
};

#endif // Q_DECK_OVERVIEW_H