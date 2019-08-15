#include "filter_language_combo.h"

FilterLanguageCombo::FilterLanguageCombo(QDir *decks_path, Config *config, QWidget *parent) : QComboBox(parent)
{
    this->decks_path = decks_path;
    this->config = config;
    
    populateComboNameFilterBox();
}

void FilterLanguageCombo::populateComboNameFilterBox()
{
    QStringList decks_names = decks_path->entryList(QDir::NoDotAndDotDot | QDir::Dirs, QDir::Name);
    
    QStringList items;
    items << "[all]";
    foreach (QString decks_name, decks_names) {
        QString item = decks_name.split("_")[0];
        if (! items.contains(item + "_") && decks_name.contains("_"))
        {
            items.append(item + "_");
        }
    }
    blockSignals(true);
    addItems(items);
    blockSignals(false);
    
    QString current_text = this->config->getLastLanguageFilter();
    setCurrentText(current_text);
}
