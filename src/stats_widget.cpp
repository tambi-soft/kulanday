#include "stats_widget.h"

StatsWidget::StatsWidget(QDir *deckpath, QWidget *parent) : QWidget(parent)
{
    this->deckpath = deckpath;
    
    setLayout(this->layout);
    
    DbAdapterMeta *db_meta = new DbAdapterMeta(this->deckpath);
    
    QStringList decks_names = this->deckpath->entryList(QDir::NoDotAndDotDot | QDir::Dirs, QDir::Name);
    
    for (int i=0; i < decks_names.length(); i++)
    {
        QString deckname = decks_names.at(i);
        QString prefix = deckname.split("_")[0];
        
        if (! this->data.contains(prefix) && deckname.contains("_"))
        {
            QMap<QString,QVariant> item;
            item["prefix"] = prefix + "_";
            item["decks_count"] = 1;
            item["words_count"] = 0;
            this->data[prefix] = item;
        }
        else
        {
            int count = this->data[prefix]["decks_count"].toInt();
            this->data[prefix]["decks_count"] = ++count;
        }
        
        DbAdapter *db = new DbAdapter(this->deckpath, deckname);
        QMap<QString,QVariant> word_count = db->selectDeckEntriesCount();
        int word_count_old = this->data[prefix]["words_count"].toInt();
        this->data[prefix]["words_count"] = word_count_old + word_count["count"].toInt();
    }
    
    
    for (QString item : this->data.keys())
    {
        addPrefixStats(this->data[item]);
    }
}

void StatsWidget::addPrefixStats(QMap<QString,QVariant> item)
{
    qDebug() << item;
    QGroupBox *group = new QGroupBox(item["prefix"].toString());
    this->layout->addWidget(group);
    
    QGridLayout *grid = new QGridLayout;
    group->setLayout(grid);
    
    grid->addWidget(new QLabel("number of decks: "+item["decks_count"].toString()), 0, 0);
    grid->addWidget(new QLabel("number of words: "+item["words_count"].toString()), 1, 0);
}
