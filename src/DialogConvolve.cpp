#include "hdr/DialogConvolve.h"
#include "ui_DialogConvolve.h"

#include <QLineEdit>
#include <QDebug>
#include <QRegExp>

DialogConvolve::DialogConvolve(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::DialogConvolve)
{
    ui->setupUi(this);
    loadKernel("Average");
    QRegExp rgx("(-|)[0-9]{1,3}|(-|)[0-9]/[1-9][0-9]{1,2}");
    ui->multiplierLineEdit->setValidator(new QRegExpValidator(rgx));
}

DialogConvolve::~DialogConvolve()
{
    delete ui;
}

void DialogConvolve::fillKernel(int *coeffs, int numRows, int numCols)
{
    ui->kernel3x3RadioButton->blockSignals(true);
    ui->kernel3x3RadioButton->setChecked(numRows == 3);
    ui->kernel5x5RadioButton->setChecked(numRows == 5);
    renderKernelTableWidget(numRows, numCols);
    ui->kernel3x3RadioButton->blockSignals(false);
    for (int y = 0; y < numRows; y++) {
        for (int x = 0; x < numCols; x++) {
            QLineEdit *lineEdit = dynamic_cast<QLineEdit*>(ui->kernelTableWidget->cellWidget(y, x));
            lineEdit->setText(QString::number(coeffs[y * numCols + x]));
        }
    }
}

void DialogConvolve::loadKernel(const QString &kernelName)
{
    if (kernelName == "Average") {
        const int xy = 3;
        int coeffs[xy][xy] = { { 1, 1, 1 },
                               { 1, 1, 1 },
                               { 1, 1, 1 } };
        ui->multiplierLineEdit->setText("");
        fillKernel((int *)coeffs, xy, xy);
    } else if (kernelName == "Gaussian Blur 3×3") {
        const int xy = 3;
        int coeffs[xy][xy] = { { 1, 2, 1 },
                               { 2, 4, 2 },
                               { 1, 2, 1 } };
        ui->multiplierLineEdit->setText("1/16");
        fillKernel((int *)coeffs, xy, xy);
    } else if (kernelName == "Gaussian Blur 5×5") {
        const int xy = 5;
        int coeffs[xy][xy] = { { 1,  4,  6,  4, 1 },
                               { 4, 16, 24, 16, 4 },
                               { 6, 24, 36, 24, 6 },
                               { 4, 16, 24, 16, 4 },
                               { 1,  4,  6,  4, 1 } };
        ui->multiplierLineEdit->setText("1/256");
        fillKernel((int *)coeffs, xy, xy);
    } else if (kernelName == "Emboss") {
        const int xy = 3;
        int coeffs[xy][xy] = { { -2, -1, 0 },
                               { -1,  1, 1 },
                               {  0,  1, 2 } };
        ui->multiplierLineEdit->setText("");
        fillKernel((int *)coeffs, xy, xy);
    } else if (kernelName == "Laplacian") {
        const int xy = 3;
        int coeffs[xy][xy] = { { 0,  1, 0 },
                               { 1, -4, 1 },
                               { 0,  1, 0 } };
        ui->multiplierLineEdit->setText("");
        fillKernel((int *)coeffs, xy, xy);
    } else if (kernelName == "PrewittX") {
        const int xy = 3;
        int coeffs[xy][xy] = { { -1, 0, 1 },
                               { -1, 0, 1 },
                               { -1, 0, 1 } };
        ui->multiplierLineEdit->setText("");
        fillKernel((int *)coeffs, xy, xy);
    } else if (kernelName == "PrewittY") {
        const int xy = 3;
        int coeffs[xy][xy] = { { -1, -1, -1 },
                               {  0,  0,  0 },
                               {  1,  1,  1 } };
        ui->multiplierLineEdit->setText("");
        fillKernel((int *)coeffs, xy, xy);
    } else if (kernelName == "SobelX") {
        const int xy = 3;
        int coeffs[xy][xy] = { { -1, 0, 1 },
                               { -2, 0, 2 },
                               { -1, 0, 1 } };
        ui->multiplierLineEdit->setText("");
        fillKernel((int *)coeffs, xy, xy);
    } else if (kernelName == "SobelY") {
        const int xy = 3;
        int coeffs[xy][xy] = { { -1, -2, -1 },
                               {  0,  0,  0 },
                               {  1,  2,  1 } };
        ui->multiplierLineEdit->setText("");
        fillKernel((int *)coeffs, xy, xy);
    } else if (kernelName == "Sharpen") {
        const int xy = 3;
        int coeffs[xy][xy] = { {  0, -1,  0 },
                               { -1,  5, -1 },
                               {  0, -1,  0 } };
        ui->multiplierLineEdit->setText("");
        fillKernel((int *)coeffs, xy, xy);
    } else if (kernelName == "Unsharp Masking 5×5") {
        const int xy = 5;
        int coeffs[xy][xy] = { { 1,  4,    6,  4, 1 },
                               { 4, 16,   24, 16, 4 },
                               { 6, 24, -476, 24, 6 },
                               { 4, 16,   24, 16, 4 },
                               { 1,  4,    6,  4, 1 } };
        ui->multiplierLineEdit->setText("-1/256");
        fillKernel((int *)coeffs, xy, xy);
    }
}

void DialogConvolve::renderKernelTableWidget(int colCount, int rowCount)
{
    ui->kernelTableWidget->setColumnCount(colCount);
    ui->kernelTableWidget->setRowCount(rowCount);
    int xySize = 38;
    for (int i = 0; i < colCount; i++)
        ui->kernelTableWidget->setColumnWidth(i, xySize);
    for (int i = 0; i < rowCount; i++)
        ui->kernelTableWidget->setRowHeight(i, xySize);
    QStringList labels;
    for (int i = 17; i < 17 + colCount; i++)
        labels << QString('0' + i);
    ui->kernelTableWidget->setHorizontalHeaderLabels(labels);
    ui->kernelTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->kernelTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    int x = ui->kernelTableWidget->geometry().x();
    int y = ui->kernelTableWidget->geometry().y();
    int tableWidth = colCount * xySize + 17;
    int tableHeight = rowCount * xySize + 25;
    ui->kernelTableWidget->setGeometry(x, y, tableWidth, tableHeight);
    QIntValidator *intValidator = new QIntValidator(-1000, 1000, this);
    for (int y = 0; y < rowCount; y++) {
        for (int x = 0; x < colCount; x++) {
            if (ui->kernelTableWidget->cellWidget(y, x))
                continue;
            QLineEdit *lineEdit = new QLineEdit;
            lineEdit->setAlignment(Qt::AlignCenter);
            lineEdit->setValidator(intValidator);
            lineEdit->setText("1");
            ui->kernelTableWidget->setCellWidget(y, x, lineEdit);
        }
    }
}

void DialogConvolve::on_comboBox_currentIndexChanged(const QString &arg1)
{
    loadKernel(arg1);
}

void DialogConvolve::on_kernel3x3RadioButton_toggled(bool checked)
{
    checked ? renderKernelTableWidget(3, 3) : renderKernelTableWidget(5, 5);
}

float parseMutiplier(QString number)
{
    if (number.contains("/")) {
        QStringList numDen = number.split("/");
        return numDen[0].toFloat() / numDen[1].toFloat();
    } else
        return number.toFloat();
}

QVector<QVector<float>> DialogConvolve::getMultipliedFlippedKernel()
{
    float multiplier = ui->multiplierLineEdit->text().isEmpty() ?
                1 : parseMutiplier(ui->multiplierLineEdit->text());
    int numRows = ui->kernelTableWidget->rowCount();
    int numCols = ui->kernelTableWidget->columnCount();
    QVector<QVector<float>> multipliedFlippedKernel(numRows);
    for (int y = 0; y < numRows; y++) {
        for (int x = 0; x < numCols; x++) {
            QLineEdit *lineEdit = dynamic_cast<QLineEdit *>(
                        ui->kernelTableWidget->cellWidget(numRows - 1 - y, numCols - 1 - x));
            float cellValue = lineEdit->text().toFloat();
            multipliedFlippedKernel[y] << multiplier * cellValue;
        }
    }
    return multipliedFlippedKernel;
}
