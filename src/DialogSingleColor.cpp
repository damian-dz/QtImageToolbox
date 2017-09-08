#include "hdr/DialogSingleColor.h"
#include "ui_DialogSingleColor.h"

#include <QDebug>

DialogSingleColor::DialogSingleColor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSingleColor),
    alphaChannel(true)
{
    ui->setupUi(this);
    QIntValidator *intValidator = new QIntValidator(0, 10000, this);
    ui->widthLineEdit->setValidator(intValidator);
    ui->heightLineEdit->setValidator(intValidator);
    ui->widthLineEdit->setFocus();
}

DialogSingleColor::~DialogSingleColor()
{
    delete ui;
}

QRgb DialogSingleColor::getColor()
{
    uchar argb[4];
    argb[3] = ui->aSpinBox->value();
    argb[2] = ui->rSpinBox->value();
    argb[1] = ui->gSpinBox->value();
    argb[0] = ui->bSpinBox->value();
    return *(QRgb *)argb;
}

QPair<QSize, QImage::Format> DialogSingleColor::getParameters()
{
    int width = ui->widthLineEdit->text().toInt();
    int height = ui->heightLineEdit->text().toInt();
    QImage::Format format;
    switch(ui->formatComboBox->currentIndex()) {
    case 0:
        format = QImage::Format_ARGB32;
        break;
    case 1:
        format = QImage::Format_RGB888;
        break;
    case 2:
        format = QImage::Format_Grayscale8;
        break;
    }
    return qMakePair(QSize(width, height), format);
}

void DialogSingleColor::on_colorDialogButton_clicked()
{
    QColor color = alphaChannel ?
                QColorDialog::getColor(Qt::white, this, "Select Color", QColorDialog::ShowAlphaChannel) :
                QColorDialog::getColor(Qt::white, this);
    ui->aSpinBox->setValue(color.alpha());
    ui->rSpinBox->setValue(color.red());
    ui->gSpinBox->setValue(color.green());
    ui->bSpinBox->setValue(color.blue());
}

void DialogSingleColor::on_formatComboBox_currentIndexChanged(int index)
{
    ui->aSpinBox->setEnabled(index == 0);
    alphaChannel = index == 0;
}
