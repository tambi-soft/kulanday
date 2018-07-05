
#include <QDebug>
#include <QWidget>
#include <QGridLayout>
#include <QTableWidget>
#include <QHeaderView> // for hiding headers
#include <QPushButton>
#include <QDir>
#include <QFileDialog>
#include <QComboBox>
#include <QListView>
#include <QIcon>
#include <QColor>
#include <QMessageBox>

#include "create_new_deck_dialog.h"
#include "db_adapter_meta.h"

class QDecksOverviewWidget : public QWidget
{
    Q_OBJECT

public:
    QDecksOverviewWidget(QDir *decks_path, QWidget *parent = 0);

private:
    DbAdapterMeta *database;
    QGridLayout *layout;
    QComboBox *combo_name_filter;
    QStringList COMBO_STATI;
    QStringList COMBO_STATI_FILTER;
    QComboBox *combo_status_filter;
    QTableWidget *table;
    
    QDir *decks_path;
    
    void populateComboNameFilterBox();
    void populateDecksOverview();
    QComboBox *populateComboStatus(QString deck_name);
    void populateComboStatusFilter(QComboBox *combo);
    void comboColorAdjust(QComboBox *combo);
    void comboFilterColorAdjust(QComboBox *combo);
    
public slots:
    void refreshTable();
    
private slots:
    void createNewDeckClicked();
    void tableButtonDirtyDozenClicked(QString deck_name);
    void tableButtonLearnClicked(QString deck_name);
    void tableButtonViewDeckClicked(QString deck_name);
    void tableButtonDeleteDeckClicked(QString deck_name);
    void onComboNameFilterTextChanged(QString text);
    void onComboStatusFilterTextChanged(QString text);
    void onComboStatusTextChanged(QString deck_name, QComboBox *combo_status);
    
signals:
    void deckDirtyDozenClicked(QString deck_name);
    void deckLearnClicked(QString deck_name);
    void deckViewClicked(QString deck_name);
    void createNewDeck(QUrl url);
    void deleteDeck(QString deck_name);
};
