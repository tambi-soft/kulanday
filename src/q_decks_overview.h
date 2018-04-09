
#include <QDebug>
#include <QWidget>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView> // for hiding headers
#include <QPushButton>
#include <QDir>

#include "q_create_new_deck_widget.h"

class QDecksOverviewWidget : public QWidget
{
    Q_OBJECT

public:
    QDecksOverviewWidget(QWidget *parent = 0);

private:
    QVBoxLayout *layout;
    QTableWidget *table;
    QPushButton *new_deck_button;
    
    QDir *decks_path;
    
    void populateDecksOverview();
    
private slots:
    void createNewDeck();
    void tableButtonDirtyDozenClicked(QString deck_name);
    void tableButtonViewDeckClicked(QString deck_name);
    
signals:
    void deckDirtyDozenClicked(QString deck_name);
    void deckViewClicked(QString deck_name);
};
