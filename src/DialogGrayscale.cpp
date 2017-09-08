#include "hdr/DialogGrayscale.h"
#include "ui_DialogGrayscale.h"

DialogGrayscale::DialogGrayscale(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::DialogGrayscale)
{
    ui->setupUi(this);
    ui->rDoubleSpinBox->setFocus();
}

void DialogGrayscale::on_averageCheckBox_clicked(bool checked)
{
     ui->rDoubleSpinBox->setDisabled(checked);
     ui->gDoubleSpinBox->setDisabled(checked);
     ui->bDoubleSpinBox->setDisabled(checked);
     if (!checked) {
         ui->rDoubleSpinBox->setFocus();
         ui->rDoubleSpinBox->selectAll();
     }
}

std::tuple<double, double, double> DialogGrayscale::getParameters()
{
    double r = ui->rDoubleSpinBox->value();
    double g = ui->gDoubleSpinBox->value();
    double b = ui->bDoubleSpinBox->value();
    return std::make_tuple(r, g, b);
}

bool DialogGrayscale::averageChecked()
{
    return ui->averageCheckBox->isChecked();
}

DialogGrayscale::~DialogGrayscale()
{
    delete ui;
}
