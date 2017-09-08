#include "hdr/DialogPixelate.h"
#include "ui_DialogPixelate.h"

DialogPixelate::DialogPixelate(int maxSize, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPixelate)
{
    ui->setupUi(this);
    ui->pixelSizeSpinBox->setMaximum(maxSize);
    ui->pixelSizeHSlider->setMaximum(maxSize);
}

DialogPixelate::~DialogPixelate()
{
    delete ui;
}

int DialogPixelate::getPixelSize()
{
    return ui->pixelSizeSpinBox->value();
}

void DialogPixelate::on_pixelSizeSpinBox_valueChanged(int arg1)
{
    ui->pixelSizeHSlider->blockSignals(true);
    ui->pixelSizeHSlider->setValue(arg1);
    ui->pixelSizeHSlider->blockSignals(false);
}

void DialogPixelate::on_pixelSizeHSlider_valueChanged(int value)
{
    ui->pixelSizeSpinBox->blockSignals(true);
    ui->pixelSizeSpinBox->setValue(value);
    ui->pixelSizeSpinBox->blockSignals(false);
}
