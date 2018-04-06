#include "q_dirtydozen_widget.h"

QDirtyDozenWidget::QDirtyDozenWidget(QWidget *parent)
    : QWidget(parent)
    , grid (new QGridLayout)
    , audioPlayer (new QMediaPlayer)
{
    DISPLAY_COMBO_ITEMS << "image" << "name" << "word" << "translation";
    
    setLayout(grid);
    
    
}

void QDirtyDozenWidget::initialize(QString deckpath)
{
    DbAdapter *db_adapter = new DbAdapter();
    QList<QMap<QString,QVariant>> data = db_adapter->selectDeckDirtyDozenItems();
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

