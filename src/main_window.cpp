
#include "main_window.h"

QKulandayMainWindow::QKulandayMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , tab_widget (new QTabWidget)
{
    resize(860, 730);
    
    this->config = new Config();
    this->deckpath = config->getDecksPath();
    
    setCentralWidget(tab_widget);
    tab_widget->setTabsClosable(true);
    tab_widget->setMovable(true);
    
    this->menu_bar = new MenuBar(this->deckpath);
    setMenuBar(menu_bar);
    connect(menu_bar, &MenuBar::newDecksOverviewTab, this, &QKulandayMainWindow::showDecksOverviewTab);
    connect(menu_bar, &MenuBar::newSearchTab, this, &QKulandayMainWindow::showSearchWidget);
    connect(menu_bar, &MenuBar::newSettingsTab, this, &QKulandayMainWindow::showSettingsWidget);
    connect(menu_bar, &MenuBar::newHelpMarkersTab, this, &QKulandayMainWindow::showHelpMarkersWidget);
    connect(menu_bar, &MenuBar::newAboutTab, this, &QKulandayMainWindow::showHelpAboutWidget);
    connect(menu_bar, &MenuBar::deckImported, this, &QKulandayMainWindow::synchronizeDecksOverviews);
    
    connect(tab_widget, &QTabWidget::tabCloseRequested, this, &QKulandayMainWindow::closeTab);
    
    QTabBar *tab_bar = tab_widget->tabBar();
    connect(tab_bar, &QTabBar::tabMoved, this, &QKulandayMainWindow::onTabMoved);
    
    tab_bar->setMouseTracking(true);
    tab_bar->installEventFilter(this);
    /*
    tab_bar->setStyleSheet("QTabBar::tab {\
                           border-left: 1px solid #9B9B9B;\
                           border-right: 1px solid #9B9B9B;\
                           border-bottom: 1px solid #9B9B9B;\
                           border-top-color: #5A5A5A;\
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
    */
    showDecksOverviewTab();
    deactivateDecksOverviewCloseButton();
}

void QKulandayMainWindow::showDecksOverviewTab()
{
    QDecksOverviewWidget *decks = new QDecksOverviewWidget(this->config, this->deckpath);
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
    QDirtyDozenWidget *dd = new QDirtyDozenWidget(this->deckpath, deck_name, this->config);
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
        QDeckOverviewWidget *deck = new QDeckOverviewWidget(this->deckpath, deck_name, this->config);
        connect(deck, &QDeckOverviewWidget::newDeckItemRequested, this, &QKulandayMainWindow::createNewDeckItem);
        //connect(deck, &QDeckOverviewWidget::showDeckItemRequested, this, &QKulandayMainWindow::showMarkersDeckItem);
        connect(deck, &QDeckOverviewWidget::showDeckItemRequested, this, &QKulandayMainWindow::showSimpleDeckItem);
        
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
    
    synchronizeDecksOverviews();
}

void QKulandayMainWindow::createNewDeckItem(QString deck_name)
{
    QDeckItemWidget *deck_item = new QDeckItemWidget(this->deckpath, deck_name, this->last_image_import_path, this->last_audio_import_path);
    connect(deck_item, &QDeckItemWidget::contentsUpdated, this, &QKulandayMainWindow::onDeckItemContentsUpdated);
    tab_widget->addTab(deck_item, deck_name);
    
    connect(deck_item, &QDeckItemWidget::imageImportPathUpdated, this, &QKulandayMainWindow::onLastImageImportPathUpdated);
    connect(deck_item, &QDeckItemWidget::audioImportPathUpdated, this, &QKulandayMainWindow::onLastAudioImportPathUpdated);
    
    activateNewTab();
    this->tab_widget->setTabIcon(this->tab_widget->currentIndex(), QIcon::fromTheme("document-properties"));
    
    int rowid = deck_item->rowid;
    this->deck_item_widgets[deck_name + "_" + QString::number(rowid)] = this->tab_widget->currentIndex();
}

void QKulandayMainWindow::showSimpleDeckItem(QString deck_name, int rowid)
{
    if (this->deck_item_widgets.contains(deck_name + "_" + QString::number(rowid)))
    {
        int index = this->deck_item_widgets[deck_name + "_" + QString::number(rowid)];
        this->tab_widget->setCurrentIndex(index);
    }
    else
    {
        QDeckItemWidget *deck_item = new QDeckItemWidget(this->deckpath, deck_name, rowid, this->last_image_import_path, this->last_audio_import_path);
        
        connect(deck_item, &QDeckItemWidget::contentsUpdated, this, &QKulandayMainWindow::onDeckItemContentsUpdated);
        tab_widget->addTab(deck_item, deck_name);
        
        connect(deck_item, &QDeckItemWidget::imageImportPathUpdated, this, &QKulandayMainWindow::onLastImageImportPathUpdated);
        connect(deck_item, &QDeckItemWidget::audioImportPathUpdated, this, &QKulandayMainWindow::onLastAudioImportPathUpdated);
        
        activateNewTab();
        this->tab_widget->setTabIcon(this->tab_widget->currentIndex(), QIcon::fromTheme("document-properties"));
        
        this->deck_item_widgets[deck_name + "_" + QString::number(rowid)] = this->tab_widget->currentIndex();
    }
}

void QKulandayMainWindow::showMarkersDeckItem(QString deck_name, int rowid)
{
    if (this->deck_item_widgets.contains(deck_name + "_" + QString::number(rowid)))
    {
        int index = this->deck_item_widgets[deck_name + "_" + QString::number(rowid)];
        this->tab_widget->setCurrentIndex(index);
    }
    else
    {
        QDeckItemMarkersWidget *deck_item = new QDeckItemMarkersWidget(this->deckpath, deck_name, rowid, this->last_image_import_path, this->last_audio_import_path);
        
        connect(deck_item, &QDeckItemMarkersWidget::contentsUpdated, this, &QKulandayMainWindow::onDeckItemContentsUpdated);
        tab_widget->addTab(deck_item, deck_name);
        
        connect(deck_item, &QDeckItemMarkersWidget::imageImportPathUpdated, this, &QKulandayMainWindow::onLastImageImportPathUpdated);
        connect(deck_item, &QDeckItemMarkersWidget::audioImportPathUpdated, this, &QKulandayMainWindow::onLastAudioImportPathUpdated);
        
        activateNewTab();
        this->tab_widget->setTabIcon(this->tab_widget->currentIndex(), QIcon::fromTheme("document-properties"));
        
        this->deck_item_widgets[deck_name + "_" + QString::number(rowid)] = this->tab_widget->currentIndex();
    }
}

void QKulandayMainWindow::onDeckItemContentsUpdated(QString deck_name)
{
    int deck_id = this->deck_item_widgets[deck_name];
    QDeckOverviewWidget *deck = dynamic_cast<QDeckOverviewWidget*>(tab_widget->widget(deck_id));
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
    SearchWidget *widget = new SearchWidget(this->deckpath);
    
    this->tab_widget->addTab(widget, "search");
    
    activateNewTab();
}

void QKulandayMainWindow::showSettingsWidget()
{
    SettingsWidget *widget = new SettingsWidget(this->config);
    
    this->tab_widget->addTab(widget, "settings");
    
    activateNewTab();
}

void QKulandayMainWindow::showHelpMarkersWidget()
{
    HelpMarkersWidget *widget = new HelpMarkersWidget();
    
    this->tab_widget->addTab(widget, "markers");
    
    activateNewTab();
}

void QKulandayMainWindow::showHelpAboutWidget()
{
    HelpAboutWidget *widget = new HelpAboutWidget();
    
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

void QKulandayMainWindow::onLastImageImportPathUpdated(QString last_image_import_path)
{
    this->last_image_import_path = last_image_import_path;
}

void QKulandayMainWindow::onLastAudioImportPathUpdated(QString last_audio_import_path)
{
    this->last_audio_import_path = last_audio_import_path;
}

void QKulandayMainWindow::synchronizeDecksOverviews()
{
    for (int i = 0; i < this->tab_widget->count(); ++i)
    {
        QWidget *widget = this->tab_widget->widget(i);
        if (QString(widget->metaObject()->className()) == "QDecksOverviewWidget")
        {
            QDecksOverviewWidget *decks = (QDecksOverviewWidget*)widget;
            decks->refreshTable();
        }
    }
}
