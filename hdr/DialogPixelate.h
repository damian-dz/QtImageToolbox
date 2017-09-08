#ifndef DIALOGPIXELATE_H
#define DIALOGPIXELATE_H

#include <QDialog>

namespace Ui {
class DialogPixelate;
}

class DialogPixelate : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPixelate(int maxSize, QWidget *parent = 0);
    ~DialogPixelate();
    int getPixelSize();

private slots:
    void on_pixelSizeSpinBox_valueChanged(int arg1);

    void on_pixelSizeHSlider_valueChanged(int value);

private:
    Ui::DialogPixelate *ui;
};

#endif // DIALOGPIXELATE_H
