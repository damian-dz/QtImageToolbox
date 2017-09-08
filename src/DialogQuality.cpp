#include "hdr/DialogQuality.h"
#include "ui_DialogQuality.h"

DialogQuality::DialogQuality(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::DialogQuality)
{
    ui->setupUi(this);
    ui->qualitySpinBox->setValue(80);
    ui->qualitySpinBox->setFocus();
}

DialogQuality::~DialogQuality()
{
    delete ui;
}

int DialogQuality::getQuality()
{
    return ui->qualitySpinBox->value();
}

void DialogQuality::on_qualityHorizontalSlider_valueChanged(int value)
{
    ui->qualitySpinBox->setValue(value);
}

void DialogQuality::on_qualitySpinBox_valueChanged(int arg1)
{
    ui->qualityHorizontalSlider->setValue(arg1);
}
