#include "hdr/DialogBinarize.h"
#include "ui_DialogBinarize.h"

DialogBinarize::DialogBinarize(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogBinarize)
{
    ui->setupUi(this);
    ui->thresholdHorizontalSlider->setValue(127);
    ui->thresholdHorizontalSlider->setFocus();
    ui->thresholdLineEdit->setValidator(new QIntValidator(0, 255, this));
}

void DialogBinarize::on_thresholdHorizontalSlider_valueChanged(int value)
{
    ui->thresholdLineEdit->setText(QString::number(value));
}

void DialogBinarize::on_thresholdLineEdit_textChanged(const QString &arg1)
{
    ui->thresholdHorizontalSlider->setValue(arg1.toInt());
}

int DialogBinarize::getThreshold()
{
    return ui->thresholdLineEdit->text().toInt();
}

DialogBinarize::~DialogBinarize()
{
    delete ui;
}
