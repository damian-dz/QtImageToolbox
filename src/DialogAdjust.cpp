#include "hdr/DialogAdjust.h"
#include "ui_DialogAdjust.h"

DialogAdjust::DialogAdjust(QImage::Format format, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAdjust),
    m_Reversed(false)
{
    ui->setupUi(this);
    ui->brightnessVLayout->setAlignment(ui->brightnessSpinBox, Qt::AlignHCenter);
    ui->gammaVLayout->setAlignment(ui->gammaDoubleSpinBox, Qt::AlignHCenter);
    ui->contrastVLayout->setAlignment(ui->contrastSpinBox, Qt::AlignHCenter);
    if (format == QImage::Format_Indexed8 || format == QImage::Format_Grayscale8)
        ui->channeslGroupBox->setDisabled(true);
    else if (format == QImage::Format_RGB32)
        ui->alphaCheckBox->setDisabled(true);
    else if (format == QImage::Format_RGB888) {
        ui->alphaCheckBox->setDisabled(true);
        m_Reversed = true;
    }
}

DialogAdjust::~DialogAdjust()
{
    delete ui;
}

int DialogAdjust::getBrightness()
{
    return ui->brightnessSpinBox->value();
}

int DialogAdjust::getContrast()
{
    return ui->contrastSpinBox->value();
}

float DialogAdjust::getGamma()
{
    return ui->gammaDoubleSpinBox->value();
}

QVector<int> DialogAdjust::getChannelIndices()
{
    QVector<int> channels;
    if (ui->alphaCheckBox->isChecked())
        channels << 3;
    if (!m_Reversed) {
        if (ui->redCheckBox->isChecked())
            channels << 2;
        if (ui->greenCheckBox->isChecked())
            channels << 1;
        if (ui->blueCheckBox->isChecked())
            channels << 0;
    } else {
        if (ui->redCheckBox->isChecked())
            channels << 0;
        if (ui->greenCheckBox->isChecked())
            channels << 1;
        if (ui->blueCheckBox->isChecked())
            channels << 2;
    }
    return channels;
}

void DialogAdjust::on_brightnessHSlider_valueChanged(int value)
{
    ui->brightnessSpinBox->blockSignals(true);
    ui->brightnessSpinBox->setValue(value);
    ui->brightnessSpinBox->blockSignals(false);
}

void DialogAdjust::on_brightnessSpinBox_valueChanged(int arg1)
{
    ui->brightnessHSlider->blockSignals(true);
    ui->brightnessHSlider->setValue(arg1);
    ui->brightnessHSlider->blockSignals(false);
}

void DialogAdjust::on_gammaHSlider_valueChanged(int value)
{
    ui->gammaDoubleSpinBox->blockSignals(true);
    ui->gammaDoubleSpinBox->setValue(value / 100.0);
    ui->gammaDoubleSpinBox->blockSignals(false);
}

void DialogAdjust::on_gammaDoubleSpinBox_valueChanged(double arg1)
{
    ui->gammaHSlider->blockSignals(true);
    ui->gammaHSlider->setValue(arg1 * 100);
    ui->gammaHSlider->blockSignals(false);
}

void DialogAdjust::on_contrastSpinBox_valueChanged(int arg1)
{
    ui->contrastHSlider->blockSignals(true);
    ui->contrastHSlider->setValue(arg1);
    ui->contrastHSlider->blockSignals(false);
}

void DialogAdjust::on_contrastHSlider_valueChanged(int value)
{
    ui->contrastSpinBox->blockSignals(true);
    ui->contrastSpinBox->setValue(value);
    ui->contrastSpinBox->blockSignals(false);
}
