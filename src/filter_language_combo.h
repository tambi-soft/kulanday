#ifndef FILTER_LANGUAGE_COMBO_H
#define FILTER_LANGUAGE_COMBO_H

#include <QObject>
#include <QWidget>
#include <QComboBox>
#include <QDir>

#include "config.h"

/* A QComboBox populated with the deck-prefixes to select an according filter */
class FilterLanguageCombo : public QComboBox
{
    Q_OBJECT
public:
    explicit FilterLanguageCombo(QDir* decks_path, Config *config, QWidget *parent = nullptr);
    
    void populateComboNameFilterBox();
    
private:
    QDir *decks_path;
    Config *config;
    
signals:
    
public slots:
};

#endif // FILTER_LANGUAGE_COMBO_H
