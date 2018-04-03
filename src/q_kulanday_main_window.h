
#include <QMainWindow>
#include <QGridLayout>

#include <src/q_decks_overview.h>

class QKulandayMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    QKulandayMainWindow(QWidget *parent = 0);
    
private:
    QGridLayout *grid;
    
    int SELECT_DECK_INDEX = 0;
    int DECK_OVERVIEW_INDEX = 1;
    int NEW_DECK_INDEX = 2;
    int DECK_LEARN_INDEX = 3;
    int DECK_DIRTY_DOZEN_INDEX = 4;
    
private slots:
    
    
};
