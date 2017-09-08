#include "hdr/DialogRandom.h"
#include "ui_DialogRandom.h"

DialogRandom::DialogRandom(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::DialogRandom)
{
    ui->setupUi(this);
    QIntValidator *intValidator = new QIntValidator(0, 10000, this);
    ui->widthLineEdit->setValidator(intValidator);
    ui->heightLineEdit->setValidator(intValidator);
    ui->widthLineEdit->setFocus();
}

DialogRandom::~DialogRandom()
{
    delete ui;
}

QPair<QSize, QImage::Format> DialogRandom::getParameters()
{
    int width = ui->widthLineEdit->text().toInt();
    int height = ui->heightLineEdit->text().toInt();
    QImage::Format format = QImage::Format_Invalid;
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
