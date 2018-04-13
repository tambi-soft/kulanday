#ifndef Q_DECK_OVERVIEW_H
#define Q_DECK_OVERVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QPixmap>

#include <QMediaPlayer>
#include <QMediaContent>
#include <QDir>
#include <QUrl>

#include "db_adapter.h"
#include "q_audio_button.h"

class QDeckOverviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QDeckOverviewWidget(QString deck_name, QWidget *parent = nullptr);
    
private:
    QLayout *layout;
    QTableWidget *table;
    QString deck_name;
    QMediaPlayer *player;
    
    int COLUMN_OFFSET = 9;
    
    void initTableWidget(QString deck_name);
    void appendPlayButtons(int table_rowid, QList<QMap<QString,QVariant>> audio_filenames, int max_audio_count);
    void audioButtonClicked(QPushButton *button, QString audio_filename);
    
signals:
    void newDeckItemRequested(QString deck_name);
    void showDeckItemRequested(QString deck_name, int deck_id);
    
public slots:
    
private slots:
    void newItemButtonClicked();
    void editRowButtonClicked(QString deck_name, int rowid);
    void deleteRowButtonClicked(int rowid);
};

#endif // Q_DECK_OVERVIEW_H
