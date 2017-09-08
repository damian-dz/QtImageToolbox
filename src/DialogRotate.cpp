#include "hdr/DialogRotate.h"
#include "ui_DialogRotate.h"

DialogRotate::DialogRotate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRotate)
{
    ui->setupUi(this);
    ui->angleDoubleSpinBox->setFocus();
}

DialogRotate::~DialogRotate()
{
    delete ui;
}

double DialogRotate::getAngle()
{
    return ui->angleDoubleSpinBox->value();
}

bool DialogRotate::originalSizeChecked()
{
    return ui->originalSizeCheckBox->isChecked();
}

int DialogRotate::selectedInterpolationMode()
{
    return ui->interpolationComboBox->currentIndex();
}
