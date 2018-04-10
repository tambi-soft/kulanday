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

#include "db_adapter.h"

class QDeckOverviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QDeckOverviewWidget(QString deck_name, QWidget *parent = nullptr);
    
private:
    QLayout *layout;
    QTableWidget *table;
    QString deck_name;
    
    int COLUMN_OFFSET = 9;
    
    void initTableWidget(QString deck_name);
    void appendPlayButtons(int table_rowid, QList<QMap<QString,QVariant>> audio_filenames);
    
signals:
    void newDeckItemRequested(QString deck_name);
    void showDeckItemRequested(int deck_id);
    
public slots:
    
private slots:
    void newItemButtonClicked();
    void editRowButtonClicked(int rowid);
    void deleteRowButtonClicked(int rowid);
};

#endif // Q_DECK_OVERVIEW_H
