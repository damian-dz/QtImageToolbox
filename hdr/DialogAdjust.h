#ifndef DIALOGADJUST_H
#define DIALOGADJUST_H

#include <QDialog>

namespace Ui {
class DialogAdjust;
}

class DialogAdjust : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAdjust(QImage::Format format, QWidget *parent = 0);
    ~DialogAdjust();
    int getBrightness();
    int getContrast();
    float getGamma();
    QVector<int> getChannelIndices();

private slots:
    void on_brightnessHSlider_valueChanged(int value);
    void on_brightnessSpinBox_valueChanged(int arg1);
    void on_contrastHSlider_valueChanged(int value);
    void on_contrastSpinBox_valueChanged(int arg1);
    void on_gammaDoubleSpinBox_valueChanged(double arg1);
    void on_gammaHSlider_valueChanged(int value);

private:
    Ui::DialogAdjust *ui;
    bool m_Reversed;
};

#endif // DIALOGADJUST_H
