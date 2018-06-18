#include "resizing_text_edit.h"

ResizingTextEdit::ResizingTextEdit(QWidget *parent) : QTextEdit(parent)
{
    QSizePolicy size_policy = this->sizePolicy();
    size_policy.setVerticalPolicy(QSizePolicy::Fixed);
    this->setSizePolicy(size_policy);
    
    connect(this, &ResizingTextEdit::textChanged, this, &ResizingTextEdit::updateGeometry);
}

void ResizingTextEdit::updateGeometry()
{
    QMargins widget_margins = this->contentsMargins();
    qreal document_margin = this->document()->documentMargin();
    QFontMetrics font_metrics = QFontMetrics(this->document()->defaultFont());
    
    int line_count = this->document()->lineCount();
    if (line_count > 5)
    {
        line_count = 5;
    }
    else
    {
        int str_width = font_metrics.width(this->toPlainText(), -1);
        if (this->width() < str_width + 2 * document_margin + widget_margins.left() + widget_margins.right())
        {
            line_count = 5;
        }
        else
        {
            line_count = 1;
        }
    }
    
    int height = widget_margins.top() +
            document_margin +
            line_count * font_metrics.height() +
            this->document()->documentMargin() +
            widget_margins.bottom();
    
    setMinimumHeight(height);
    setMaximumHeight(height);
    
    QTextEdit::updateGeometry();
}

void ResizingTextEdit::paintEvent(QPaintEvent *e)
{
    QTextEdit::paintEvent(e);
}
