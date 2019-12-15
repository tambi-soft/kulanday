#include "dirtydozen_write_widget.h"

DirtydozenWriteWidget::DirtydozenWriteWidget(QDir *deckpath, QString deckname, Config *config, QWidget *parent) : QWidget(parent)
{
    this->deckpath = deckpath;
    this->deckname = deckname;
    this->config = config;
}
