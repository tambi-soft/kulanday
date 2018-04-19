#include "q_dirtydozen_widget.h"

QDirtyDozenWidget::QDirtyDozenWidget(QString deck_name, QWidget *parent)
    : QWidget(parent)
    , grid (new QGridLayout)
    , audioPlayer (new QMediaPlayer)
{
    DISPLAY_COMBO_ITEMS << "image" << "name" << "word" << "translation";
    
    setLayout(grid);
    
    initialize(deck_name);
}

void QDirtyDozenWidget::initialize(QString deck_name)
{
    DbAdapter *db_adapter = new DbAdapter(deck_name);
    QList<QMap<QString,QVariant>> data = db_adapter->selectDeckDirtyDozenItems();
    
    QLabel *select_display_combo_label = new QLabel("select display:");
    this->grid->addWidget(select_display_combo_label, 0, 0);
    
    QComboBox *select_display_combo = new QComboBox();
    select_display_combo->addItems(DISPLAY_COMBO_ITEMS);
    this->grid->addWidget(select_display_combo, 0, 1);
    
    QPushButton *replay_audio_button = new QPushButton("replay audio");
    this->grid->addWidget(replay_audio_button, 0, 3);
    
    for (int i; i < data.length(); ++i)
    {
        
    }
}

void QDirtyDozenWidget::update()
{
    
}

void QDirtyDozenWidget::clear()
{
    
}

void QDirtyDozenWidget::showAllButtonClicked()
{
    
}

void QDirtyDozenWidget::labelClicked()
{
    
}

void QDirtyDozenWidget::playNextAudio()
{
    
}

void QDirtyDozenWidget::playRandomAudio()
{
    
}

void QDirtyDozenWidget::playAudio()
{
    
}

void QDirtyDozenWidget::replayAudioClicked()
{
    
}

void QDirtyDozenWidget::selectDisplayCurrentIndexChanged()
{
    
}

