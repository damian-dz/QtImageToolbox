#include "hdr/DialogResize.h"
#include "ui_DialogResize.h"


DialogResize::DialogResize(QSize dims, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::DialogResize)
{
    ui->setupUi(this);
    ui->widthLineEdit->setText(QString::number(dims.width()));
    ui->heightLineEdit->setText(QString::number(dims.height()));
    originalSize = dims;
    QIntValidator *intValidator = new QIntValidator(0, 10000, this);
    ui->widthLineEdit->setValidator(intValidator);
    ui->heightLineEdit->setValidator(intValidator);
    intValidator = new QIntValidator(0, 1000, this);
    ui->percentageWidthLineEdit->setValidator(intValidator);
    ui->percentageHeightLineEdit->setValidator(intValidator);
    ui->widthLineEdit->setFocus();
}

DialogResize::~DialogResize()
{
    delete ui;
}

void DialogResize::on_widthLineEdit_textChanged(const QString &arg1)
{

}

void DialogResize::on_heightLineEdit_textChanged(const QString &arg1)
{

}

void DialogResize::on_perecentageWidthLineEdit_textChanged(const QString &arg1)
{

}

void DialogResize::on_percentageHeightLineEdit_textChanged(const QString &arg1)
{

}

QPair<QSize, Qt::TransformationMode> DialogResize::getParameters()
{
    int width = ui->widthLineEdit->text().toInt();
    int height = ui->heightLineEdit->text().toInt();
    return qMakePair(QSize(width, height), (Qt::TransformationMode)ui->interpolationComboBox->currentIndex());
}

void DialogResize::on_widthLineEdit_textEdited(const QString &arg1)
{
    int widthNew = arg1.toInt();
    double factor = widthNew / (double)originalSize.width();
    if (ui->aspectRatioCheckBox->isChecked()) {
        int heightNew = qRound(originalSize.height() * factor);
        ui->heightLineEdit->setText(QString::number(heightNew));
    }
    ui->percentageWidthLineEdit->blockSignals(true);
    int widthNewPrcnt = qRound(factor * 100);
    ui->percentageWidthLineEdit->setText(QString::number(widthNewPrcnt));
    ui->percentageWidthLineEdit->blockSignals(false);
}

void DialogResize::on_heightLineEdit_textEdited(const QString &arg1)
{
    int heightNew = arg1.toInt();
    double factor = heightNew / (double)originalSize.height();
    if (ui->aspectRatioCheckBox->isChecked()) {
        int widthNew = qRound(originalSize.width() * factor);
        ui->widthLineEdit->setText(QString::number(widthNew));
    }
    ui->percentageHeightLineEdit->blockSignals(true);
    int heightNewPrcnt = qRound(factor * 100);
    ui->percentageHeightLineEdit->setText(QString::number(heightNewPrcnt));
    ui->percentageHeightLineEdit->blockSignals(false);
}

void DialogResize::on_percentageWidthLineEdit_textEdited(const QString &arg1)
{

}


void DialogResize::on_percentageHeightLineEdit_textEdited(const QString &arg1)
{

}
