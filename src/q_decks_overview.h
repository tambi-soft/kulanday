
#include <QDebug>
#include <QWidget>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView> // for hiding headers
#include <QPushButton>
#include <QDir>

class QDecksOverviewWidget : public QWidget
{
    Q_OBJECT

public:
    QDecksOverviewWidget(QWidget *parent = 0);

private:
    QVBoxLayout *layout;
    QTableWidget *table;
    QPushButton *new_deck_button;
    
    void populateDecksOverview();
    
private slots:
    void createNewDeck();
    void tableButtonDirtyDozenClicked();
    void tableButtonViewDeckClicked();
};
