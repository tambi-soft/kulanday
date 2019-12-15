#include "stats_widget.h"

StatsWidget::StatsWidget(QDir *deckpath, QWidget *parent) : QWidget(parent)
{
    this->deckpath = deckpath;
    
    this->scroll_area = new QScrollArea;
    connect(this->scroll_area->verticalScrollBar(), &QScrollBar::rangeChanged, this, &StatsWidget::scrollBarRangeChanged);
    
    QVBoxLayout *scroll_layout = new QVBoxLayout;
    
    setLayout(scroll_layout);
    scroll_layout->addWidget(this->scroll_area);
    
    this->scroll_area->setWidgetResizable(true);
    
    scroll_layout->setMargin(0);
}

void StatsWidget::showData()
{
    this->layout = new QVBoxLayout;
    this->scroll_widget = new QWidget;
    this->scroll_widget->setLayout(this->layout);
    this->scroll_area->setWidget(scroll_widget);
    
    
    DbAdapterMeta *db_meta = new DbAdapterMeta(this->deckpath);
    QStringList stati_list = db_meta->selectStatiGrouped();
    
    QStringList decks_names = this->deckpath->entryList(QDir::NoDotAndDotDot | QDir::Dirs, QDir::Name);
    
    // prefix_ -> QMap<QString,QVariant>
    QMap<QString,QMap<QString,QVariant>> data;
    for (int i=0; i < decks_names.length(); i++)
    {
        QString deckname = decks_names.at(i);
        QString prefix = deckname.split("_")[0];
        
        if (! data.contains(prefix) && deckname.contains("_"))
        {
            QMap<QString,QVariant> item;
            item["prefix"] = prefix;
            item["decks_count"] = 1;
            item["words_count"] = 0;
            for (int i=0; i < stati_list.length(); i++)
            {
                item[stati_list.at(i)] = 0;
            }
            
            data[prefix] = item;
        }
        else
        {
            int count = data[prefix]["decks_count"].toInt();
            data[prefix]["decks_count"] = ++count;
        }
        
        DbAdapter *db = new DbAdapter(this->deckpath, deckname);
        int word_count = db->selectDeckEntriesCount();
        
        int word_count_old = data[prefix]["words_count"].toInt();
        data[prefix]["words_count"] = word_count_old + word_count;
        
        QString status = db_meta->selectStatusForDeck(deckname);
        int status_current = data[prefix][status].toInt();
        data[prefix][status] = status_current + word_count;
    }
    
    
    for (QString item : data.keys())
    {
        addPrefixStats(data[item], stati_list);
    }
}

void StatsWidget::addPrefixStats(QMap<QString,QVariant> item, QStringList stati_list)
{
    QGroupBox *group = new QGroupBox(item["prefix"].toString());
    this->layout->addWidget(group);
    
    QGridLayout *grid = new QGridLayout;
    group->setLayout(grid);
    
    grid->addWidget(new QLabel("number of decks: "+item["decks_count"].toString()), 0, 0);
    grid->addWidget(new QLabel("number of words: "+item["words_count"].toString()), 1, 0);
    
    for (int i=0; i < stati_list.length(); i++)
    {
        QString status = stati_list.at(i);
        grid->addWidget(new QLabel(status + ":\t\t" + item[status].toString() + "\t words"), i, 1);
    }
}

void StatsWidget::showEvent(QShowEvent */* event */)
{
    // store the scrollbar position first to be restored in scrollBarRangeChanged afterwards
    this->scrollbar_pos = this->scroll_area->verticalScrollBar()->sliderPosition();
    
    if (this->scroll_widget != nullptr)
    {
        this->scroll_widget->deleteLater();
    }
    
    showData();
}

void StatsWidget::scrollBarRangeChanged(int /*min*/, int max)
{
    if (this->scrollbar_pos <= max)
    {
        this->scroll_area->verticalScrollBar()->setSliderPosition(this->scrollbar_pos);
    }
}
