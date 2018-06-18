
#include "main_window.h"

QKulandayMainWindow::QKulandayMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , tab_widget (new QTabWidget)
    , menu_bar (new QKulandayMenuBar)
{
    resize(800, 700);
    
    Config *config = new Config();
    this->deckpath = config->getDecksPath();
    
    setCentralWidget(tab_widget);
    tab_widget->setTabsClosable(true);
    tab_widget->setMovable(true);
    
    setMenuBar(menu_bar);
    connect(menu_bar, &QKulandayMenuBar::newDecksOverviewTab, this, &QKulandayMainWindow::showDecksOverviewTab);
    connect(menu_bar, &QKulandayMenuBar::newSearchTab, this, &QKulandayMainWindow::showSearchWidget);
    connect(menu_bar, &QKulandayMenuBar::newAboutTab, this, &QKulandayMainWindow::showAboutWidget);
    
    connect(tab_widget, &QTabWidget::tabCloseRequested, this, &QKulandayMainWindow::closeTab);
    
    QTabBar *tab_bar = tab_widget->tabBar();
    connect(tab_bar, &QTabBar::tabMoved, this, &QKulandayMainWindow::onTabMoved);
    
    tab_bar->setMouseTracking(true);
    tab_bar->installEventFilter(this);
    
    tab_bar->setStyleSheet("QTabBar::tab {\
                           border-left: 1px solid #9B9B9B;\
                           border-right: 1px solid #9B9B9B;\
                           border-bottom: 1px solid #9B9B9B;\
                           border-top-color: #5A5A5A;\
                           /*min-width: 20ex;*/\
                           max-width: 15ex;\
                           font: 8px;\
                           padding: 1px;\
                           }\
                           QTabBar::tab:hover {\
                           background: #c0c0c0;\
                           }\
                           QTabBar::tab:selected {\
                           background: #fcfcfc;\
                           border-color: #9B9B9B;\
                           border-bottom: 0px;\
                           }\
                           QTabBar::tab:!selected {\
                           margin-top: 3px;\
                           background: #e7e7e7;\
                           }");
    
    showDecksOverviewTab();
    deactivateDecksOverviewCloseButton();
}

void QKulandayMainWindow::showDecksOverviewTab()
{
    QDecksOverviewWidget *decks = new QDecksOverviewWidget(this->deckpath);
    tab_widget->addTab(decks, "decks");
    connect(decks, &QDecksOverviewWidget::deckDirtyDozenClicked, this, &QKulandayMainWindow::showDirtyDozenWidget);
    connect(decks, &QDecksOverviewWidget::deckLearnClicked, this, &QKulandayMainWindow::showLearnWidget);
    connect(decks, &QDecksOverviewWidget::deckViewClicked, this, &QKulandayMainWindow::showDeckWidget);
    connect(decks, &QDecksOverviewWidget::createNewDeck, this, &QKulandayMainWindow::createNewDeck);
    connect(decks, &QDecksOverviewWidget::deleteDeck, this, &QKulandayMainWindow::deleteDeck);
    
    activateNewTab();
}

void QKulandayMainWindow::showDirtyDozenWidget(QString deck_name)
{
    QDirtyDozenWidget *dd = new QDirtyDozenWidget(this->deckpath, deck_name);
    tab_widget->addTab(dd, deck_name);
    
    activateNewTab();
    
    this->tab_widget->setTabIcon(this->tab_widget->currentIndex(), QIcon::fromTheme("image-loading"));
}

void QKulandayMainWindow::showLearnWidget(QString deck_name)
{
    QInvDirtyDozenWidget *learn = new QInvDirtyDozenWidget(this->deckpath, deck_name);
    tab_widget->addTab(learn, deck_name);
    
    activateNewTab();
    
    this->tab_widget->setTabIcon(this->tab_widget->currentIndex(), QIcon::fromTheme("image-x-generic"));
}

void QKulandayMainWindow::showDeckWidget(QString deck_name)
{   
    if (this->deck_item_widgets.contains(deck_name))
    {
        int index = this->deck_item_widgets[deck_name];
        this->tab_widget->setCurrentIndex(index);
    }
    else
    {
        QDeckOverviewWidget *deck = new QDeckOverviewWidget(this->deckpath, deck_name);
        connect(deck, &QDeckOverviewWidget::newDeckItemRequested, this, &QKulandayMainWindow::createNewDeckItem);
        connect(deck, &QDeckOverviewWidget::showDeckItemRequested, this, &QKulandayMainWindow::showDeckItem);
        
        tab_widget->addTab(deck, deck_name);
        activateNewTab();
        this->tab_widget->setTabIcon(this->tab_widget->currentIndex(), QIcon::fromTheme("folder-open"));
        
        this->deck_item_widgets[deck_name] = this->tab_widget->currentIndex();
    }
}

void QKulandayMainWindow::createNewDeck(QUrl deck_url)
{
    if (! deck_url.isEmpty())
    {
        showDeckWidget(deck_url.fileName());
    }
}

void QKulandayMainWindow::deleteDeck(QString deck_name)
{
    QString delpath = this->deckpath->absolutePath() + "/" + deck_name;
    QDir *dir = new QDir(delpath);
    dir->removeRecursively();
}

void QKulandayMainWindow::createNewDeckItem(QString deck_name)
{
    QDeckItemWidget *deck_item = new QDeckItemWidget(this->deckpath, deck_name);
    connect(deck_item, &QDeckItemWidget::contentsUpdated, this, &QKulandayMainWindow::onDeckItemContentsUpdated);
    tab_widget->addTab(deck_item, deck_name);
    
    activateNewTab();
    this->tab_widget->setTabIcon(this->tab_widget->currentIndex(), QIcon::fromTheme("document-properties"));
    
    int rowid = deck_item->rowid;
    this->deck_item_widgets[deck_name + "_" + QString::number(rowid)] = this->tab_widget->currentIndex();
}

void QKulandayMainWindow::showDeckItem(QString deck_name, int rowid)
{
    if (this->deck_item_widgets.contains(deck_name + "_" + QString::number(rowid)))
    {
        int index = this->deck_item_widgets[deck_name + "_" + QString::number(rowid)];
        this->tab_widget->setCurrentIndex(index);
    }
    else
    {
        QDeckItemWidget *deck_item = new QDeckItemWidget(this->deckpath, deck_name, rowid);
        connect(deck_item, &QDeckItemWidget::contentsUpdated, this, &QKulandayMainWindow::onDeckItemContentsUpdated);
        tab_widget->addTab(deck_item, deck_name);
        
        activateNewTab();
        this->tab_widget->setTabIcon(this->tab_widget->currentIndex(), QIcon::fromTheme("document-properties"));
        
        this->deck_item_widgets[deck_name + "_" + QString::number(rowid)] = this->tab_widget->currentIndex();
    }
}

void QKulandayMainWindow::onDeckItemContentsUpdated(QString deck_name)
{
    int deck_id = this->deck_item_widgets[deck_name];
    QDeckOverviewWidget *deck = (QDeckOverviewWidget*) tab_widget->widget(deck_id);
    deck->refresh();
}

void QKulandayMainWindow::activateNewTab()
{
    tab_widget->setCurrentIndex(tab_widget->count()-1);
}

void QKulandayMainWindow::closeTab(int tab_id)
{
    QWidget *tab_to_delete = tab_widget->widget(tab_id);
    tab_widget->removeTab(tab_id);
    tab_to_delete->deleteLater();
    
    for (auto k : this->deck_item_widgets.keys())
    {
        int value = this->deck_item_widgets[k];
        // remove key->value pair from the map
        if (value == tab_id)
        {
            this->deck_item_widgets.remove(k);
        }
        // adjust the map to match the new tab ids
        else if (value > tab_id)
        {
            this->deck_item_widgets[k] = value -1;
        }
    }
}

void QKulandayMainWindow::onTabMoved(int from, int to)
{
    // adjust the map to match the new tab ids
    for (auto k : this->deck_item_widgets.keys())
    {
        int value = this->deck_item_widgets[k];
        
        if (value == from)
        {
            this->deck_item_widgets[k] = to;
        }
        if (value == to)
        {
            this->deck_item_widgets[k] = from;
        }
        
    }
}

void QKulandayMainWindow::deactivateDecksOverviewCloseButton()
{
    QTabBar *tb = this->tab_widget->tabBar();
    tb->tabButton(0, QTabBar::RightSide)->hide();
}

void QKulandayMainWindow::showSearchWidget()
{
    QSearchWidget *widget = new QSearchWidget();
    
    this->tab_widget->addTab(widget, "search");
    
    activateNewTab();
}

void QKulandayMainWindow::showAboutWidget()
{
    AboutWidget *widget = new AboutWidget();
    
    this->tab_widget->addTab(widget, "about");
    
    activateNewTab();
}

bool QKulandayMainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this->tab_widget->tabBar())
    {
        if (event->type() == QEvent::MouseMove)
        {
            QMouseEvent* mev = static_cast<QMouseEvent*>(event);
            int index = this->tab_widget->tabBar()->tabAt(mev->pos());
            
            if (index != this->tooltip_last_index)
            {
                this->tooltip_last_index = index;
                QString name = this->tab_widget->tabBar()->tabText(index);
                
                QPoint pos = QPoint(mev->x(), -20);
                QToolTip::showText(this->mapToParent(pos), name);
            }
        }
    }
    
    return QWidget::eventFilter(watched, event);
}