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
    QFontMetrics font_metrics = QFontMetrics(this->currentFont());
    
    int line_count = this->document()->lineCount();
    int str_width = font_metrics.width(this->toPlainText(), -1);
    int str_width_with_margins = str_width + 2 * document_margin + widget_margins.left() + widget_margins.right();
    int line_count_new = line_count;
    if (this->width() < str_width_with_margins)
    {
        //line_count = 5;
        
        if (str_width_with_margins <= this->width() * 2)
        {
            line_count_new = 2;
        }
        else if (str_width_with_margins <= this->width() * 3)
        {
            line_count_new = 3;
        }
        else if (str_width_with_margins <= this->width() * 4)
        {
            line_count_new = 4;
        }
        else if (str_width_with_margins <= this->width() * 5)
        {
            line_count_new = 5;
        }
    }
    if (line_count > 1 && line_count <= 5)
    {
        // just keep line_count as it is
    }
    else if (line_count > 5)
    {
        line_count = 5;
    }
    else
    {
        line_count = 1;
    }
    
    if (line_count < line_count_new)
    {
        line_count = line_count_new;
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
