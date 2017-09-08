#ifndef DIALOGCONVOLVE_H
#define DIALOGCONVOLVE_H

#include <QDialog>

namespace Ui {
class DialogConvolve;
}

class DialogConvolve : public QDialog
{
    Q_OBJECT

public:
    explicit DialogConvolve(QWidget *parent = 0);
    ~DialogConvolve();
    QVector<QVector<float>> getMultipliedFlippedKernel();

private slots:
    void on_comboBox_currentIndexChanged(const QString &arg1);
    void on_kernel3x3RadioButton_toggled(bool checked);

private:
    Ui::DialogConvolve *ui;
    void fillKernel(int *coeffs, int numRows, int numCols);
    void loadKernel(const QString &kernelName);
    void renderKernelTableWidget(int colCount, int rowCount);
};

#endif // DIALOGCONVOLVE_H
