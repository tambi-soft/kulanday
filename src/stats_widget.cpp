#include "stats_widget.h"

StatsWidget::StatsWidget(QDir *deckpath, QWidget *parent) : QWidget(parent)
{
    this->deckpath = deckpath;
    
    QScrollArea *scroll_area = new QScrollArea;
    QWidget *scroll_widget = new QWidget;
    QVBoxLayout *scroll_layout = new QVBoxLayout;
    
    setLayout(scroll_layout);
    scroll_layout->addWidget(scroll_area);
    
    scroll_area->setWidgetResizable(true);
    scroll_area->setWidget(scroll_widget);
    scroll_widget->setLayout(this->layout);
    scroll_layout->setMargin(0);
    
    
    DbAdapterMeta *db_meta = new DbAdapterMeta(this->deckpath);
    QStringList stati_list = db_meta->selectStatiGrouped();
    
    QStringList decks_names = this->deckpath->entryList(QDir::NoDotAndDotDot | QDir::Dirs, QDir::Name);
    
    for (int i=0; i < decks_names.length(); i++)
    {
        QString deckname = decks_names.at(i);
        QString prefix = deckname.split("_")[0];
        
        if (! this->data.contains(prefix) && deckname.contains("_"))
        {
            QMap<QString,QVariant> item;
            item["prefix"] = prefix;
            item["decks_count"] = 1;
            item["words_count"] = 0;
            for (int i=0; i < stati_list.length(); i++)
            {
                item[stati_list.at(i)] = 0;
            }
            
            this->data[prefix] = item;
        }
        else
        {
            int count = this->data[prefix]["decks_count"].toInt();
            this->data[prefix]["decks_count"] = ++count;
        }
        
        DbAdapter *db = new DbAdapter(this->deckpath, deckname);
        int word_count = db->selectDeckEntriesCount();
        
        int word_count_old = this->data[prefix]["words_count"].toInt();
        this->data[prefix]["words_count"] = word_count_old + word_count;
        
        QString status = db_meta->selectStatusForDeck(deckname);
        int status_current = this->data[prefix][status].toInt();
        this->data[prefix][status] = status_current + word_count;
    }
    
    
    for (QString item : this->data.keys())
    {
        addPrefixStats(this->data[item], stati_list);
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
