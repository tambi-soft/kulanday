
#include <QDebug>
#include <QWidget>
#include <QGridLayout>
#include <QTableWidget>
#include <QHeaderView> // for hiding headers
#include <QPushButton>
#include <QDir>
#include <QFileDialog>
#include <QComboBox>

#include "q_create_new_deck_dialog.h"

class QDecksOverviewWidget : public QWidget
{
    Q_OBJECT

public:
    QDecksOverviewWidget(QDir *decks_path, QWidget *parent = 0);

private:
    QGridLayout *layout;
    QComboBox *combo;
    QTableWidget *table;
    
    QDir *decks_path;
    
    void populateComboBox();
    void populateDecksOverview();
    
private slots:
    void createNewDeckClicked();
    void refreshTable();
    void tableButtonDirtyDozenClicked(QString deck_name);
    void tableButtonLearnClicked(QString deck_name);
    void tableButtonViewDeckClicked(QString deck_name);
    void onComboTextChanged(QString text);
    
signals:
    void deckDirtyDozenClicked(QString deck_name);
    void deckLearnClicked(QString deck_name);
    void deckViewClicked(QString deck_name);
    void createNewDeck(QUrl url);
};
