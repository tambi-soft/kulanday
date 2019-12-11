
#include <QDebug>
#include <QWidget>
#include <QGridLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QDir>
#include <QFileDialog>
#include <QListView>
#include <QIcon>
#include <QColor>
#include <QRgb>
#include <QMessageBox>
#include <QLabel>

#include "create_new_deck_dialog.h"
#include "db_adapter_meta.h"
#include "config.h"
#include "filter_language_combo.h"

class QDecksOverviewWidget : public QWidget
{
    Q_OBJECT

public:
    QDecksOverviewWidget(Config *config, QDir *decks_path, QWidget *parent = nullptr);

private:
    DbAdapterMeta *database;
    QGridLayout *layout;
    FilterLanguageCombo *combo_name_filter;
    QStringList COMBO_STATI_LABELS;
    QList<QColor> COMBO_STATI_COLORS;
    QStringList style_list;
    QComboBox *combo_status_filter;
    QTableWidget *table;
    
    Config *config;
    QDir *decks_path;
    
    void populateDecksOverview();
    QComboBox *populateComboStatus(QString deck_name);
    void populateComboStatusFilter(QComboBox *combo);
    void comboColorAdjust(QComboBox *combo);
    void comboColorAdjustBuildStylelist();
    
public slots:
    void refreshTable();
    
private slots:
    void createNewSimpleDeckClicked();
    void createNewMarkersDeckClicked();
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
