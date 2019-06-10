#include "text_edit_markers.h"

TextEditMarkers::TextEditMarkers(QWidget *parent)
    : QWidget(parent)
    , layout (new QHBoxLayout)
    , markers (new MarkersTextEdit)
    , payload (new PayloadTextEdit)
{
    layout->addWidget(markers);
    layout->addWidget(payload);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
    
    markers->setMaximumWidth(200);
    
}


MarkersTextEdit::MarkersTextEdit(QWidget *parent) : QTextEdit(parent)
{
    
}


PayloadTextEdit::PayloadTextEdit(QWidget *parent) : QTextEdit(parent)
{
    
}
