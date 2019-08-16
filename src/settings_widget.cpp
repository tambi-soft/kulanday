#include "settings_widget.h"

SettingsWidget::SettingsWidget(Config *config, QWidget *parent)
    : QWidget(parent)
    , layout (new QVBoxLayout)
{
    this->config = config;
    
    setLayout(this->layout);
    
    addDirtyDozenSettingsArea();
    addDeckPathSettingsArea();
    
    this->layout->addStretch();
}

void SettingsWidget::addDirtyDozenSettingsArea()
{
    QGroupBox *spin_boxes_group = new QGroupBox("Dirty Dozen Matrix Size");
    QSpinBox *dd_width_spin = new QSpinBox;
    QSpinBox *dd_height_spin = new QSpinBox;
    
    dd_width_spin->setRange(1, 10);
    dd_height_spin->setRange(1, 10);
    
    dd_width_spin->setSingleStep(1);
    dd_height_spin->setSingleStep(1);
    
    this->dd_size = this->config->getDirtyDozenSize();
    dd_width_spin->setValue(dd_size.width());
    dd_height_spin->setValue(dd_size.height());
    
    QLabel *label_width = new QLabel(tr("Matrix Width:"));
    QLabel *label_height = new QLabel(tr("Matrix Height:"));
    
    QVBoxLayout *dd_spinbox_layout = new QVBoxLayout;
    dd_spinbox_layout->addWidget(label_width);
    dd_spinbox_layout->addWidget(dd_width_spin);
    dd_spinbox_layout->addWidget(label_height);
    dd_spinbox_layout->addWidget(dd_height_spin);
    
    spin_boxes_group->setLayout(dd_spinbox_layout);
    
    this->layout->addWidget(spin_boxes_group);
    
    connect(dd_width_spin, qOverload<int>(&QSpinBox::valueChanged), this, &SettingsWidget::ddWidthChanged);
    connect(dd_height_spin, qOverload<int>(&QSpinBox::valueChanged), this, &SettingsWidget::ddHeightChanged);
}

void SettingsWidget::ddWidthChanged(int width)
{
    this->dd_size.setWidth(width);
    this->config->setDirtyDozenSize(this->dd_size);
}

void SettingsWidget::ddHeightChanged(int height)
{
    this->dd_size.setHeight(height);
    this->config->setDirtyDozenSize(this->dd_size);
}

void SettingsWidget::addDeckPathSettingsArea()
{
    QGroupBox *group = new QGroupBox("Path for opening/saving Decks");
    QVBoxLayout *layout_outer = new QVBoxLayout;
    QHBoxLayout *layout = new QHBoxLayout();
    
    this->edit_path = new QLineEdit;
    this->edit_path->setReadOnly(true);
    layout->addWidget(edit_path);
    
    edit_path->setText(this->config->getDeckpathString());
    
    QPushButton *button = new QPushButton("select Folder");
    layout->addWidget(button);
    connect(button, &QPushButton::clicked, this, &SettingsWidget::showFolderSelectDialog);
    
    QWidget *edit_and_button_widget = new QWidget;
    layout->setMargin(0);
    layout->setSpacing(0);
    edit_and_button_widget->setLayout(layout);
    
    QLabel *hint = new QLabel("<b>Hint:</b> You have to restart <i>kulanday</i> for the changed path to take effect!");
    layout_outer->addWidget(edit_and_button_widget);
    layout_outer->addWidget(hint);
    
    group->setLayout(layout_outer);
    this->layout->addWidget(group);
}

void SettingsWidget::showFolderSelectDialog()
{
    QString dir_old = this->edit_path->text();
    QString dir_new = QFileDialog::getExistingDirectory(this, tr("Select Directory"),
        dir_old,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    
    qDebug() << "DIR:" << dir_new;
    // if the user hit "cancel", we have an empty string here
    if (!dir_new.isEmpty() || !dir_new.isNull())
    {
        this->edit_path->setText(dir_new);
        this->config->setDeckpath(dir_new);
    }
}
