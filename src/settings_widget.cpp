#include "settings_widget.h"

SettingsWidget::SettingsWidget(Config *config, QWidget *parent)
    : QWidget(parent)
    , layout (new QVBoxLayout)
{
    this->config = config;
    
    setLayout(this->layout);
    
    addDeckPathSettingsArea();
    addAudioEditorSettingsArea();
    addDirtyDozenSettingsArea();
    
    this->layout->addStretch();
}

void SettingsWidget::addDirtyDozenSettingsArea()
{
    QGroupBox *spin_boxes_group = new QGroupBox("Dirty Dozen Matrix Size");
    
    // matrix size
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
    
    // item size
    QSpinBox *dd_item_size = new QSpinBox;
    dd_item_size->setRange(100, 800);
    int item_size = this->config->getDirtyDozenFieldSize();
    dd_item_size->setValue(item_size);
    QLabel *label_item_size = new QLabel(tr("Image Height"));
    
    dd_spinbox_layout->addWidget(label_item_size);
    dd_spinbox_layout->addWidget(dd_item_size);
    
    // item aspect ratio
    QDoubleSpinBox *dd_aspect = new QDoubleSpinBox;
    dd_aspect->setRange(0, 2);
    double aspect = this->config->getDirtyDozenAspectRatio();
    dd_aspect->setValue(aspect);
    QLabel *label_item_aspect = new QLabel(tr("Image Aspect Ratio (default: 1.60)"));
    
    dd_spinbox_layout->addWidget(label_item_aspect);
    dd_spinbox_layout->addWidget(dd_aspect);
    
    // put the group together
    spin_boxes_group->setLayout(dd_spinbox_layout);
    
    this->layout->addWidget(spin_boxes_group);
    
    connect(dd_width_spin, qOverload<int>(&QSpinBox::valueChanged), this, &SettingsWidget::ddWidthChanged);
    connect(dd_height_spin, qOverload<int>(&QSpinBox::valueChanged), this, &SettingsWidget::ddHeightChanged);
    
    connect(dd_item_size, qOverload<int>(&QSpinBox::valueChanged), this, &SettingsWidget::ddItemSizeChanged);
    connect(dd_aspect, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &SettingsWidget::ddItemAspectChanged);
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

void SettingsWidget::ddItemSizeChanged(int size)
{
    this->config->setDirtyDozenFieldSize(size);
}

void SettingsWidget::ddItemAspectChanged(double aspect)
{
    this->config->setDirtyDozenAspectRatio(aspect);
}

void SettingsWidget::addDeckPathSettingsArea()
{
    QGroupBox *group = new QGroupBox("Path for opening/saving Decks");
    QVBoxLayout *layout_outer = new QVBoxLayout;
    QHBoxLayout *layout = new QHBoxLayout();
    
    this->edit_path = new QLineEdit;
    //this->edit_path->setReadOnly(true);
    layout->addWidget(edit_path);
    
    edit_path->setText(this->config->getDeckpathString());
    
    QPushButton *button = new QPushButton("select Folder");
    layout->addWidget(button);
    connect(button, &QPushButton::clicked, this, &SettingsWidget::showFolderSelectDialog);
    connect(edit_path, &QLineEdit::textChanged, this, &SettingsWidget::saveFolderPath);
    
    QWidget *edit_and_button_widget = new QWidget;
    layout->setMargin(0);
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
        saveFolderPath();
    }
}

void SettingsWidget::saveFolderPath()
{
    QString dir = this->edit_path->text();
    this->config->setDeckpath(dir);
}

void SettingsWidget::addAudioEditorSettingsArea()
{
    QGroupBox *group = new QGroupBox("Path for the external Audio Editor (e.g. Audacity)");
    QHBoxLayout *layout = new QHBoxLayout;
    group->setLayout(layout);
    this->layout->addWidget(group);
    
    this->edit_audio_editor = new QLineEdit;
    layout->addWidget(this->edit_audio_editor);
    
    edit_audio_editor->setText(this->config->getAudioEditorPath());
    
    QPushButton *button = new QPushButton("select Audio Editor");
    
    connect(button, &QPushButton::clicked, this, &SettingsWidget::showAudioEditorSelectDialog);
    connect(edit_audio_editor, &QLineEdit::textChanged, this, &SettingsWidget::saveAudioEditorPath);
    
    layout->addWidget(button);
}

void SettingsWidget::showAudioEditorSelectDialog()
{
    QString path_old = this->edit_audio_editor->text();
    QString path = QFileDialog::getExistingDirectory(this, tr("Select Audio Editor"), path_old);
    
    if (!path.isEmpty() || !path.isNull())
    {
        this->edit_audio_editor->setText(path);
        saveAudioEditorPath();
    }
}

void SettingsWidget::saveAudioEditorPath()
{
    QString path = this->edit_audio_editor->text();
    this->config->setAudioEditorPath(path);
}
