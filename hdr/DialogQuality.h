#ifndef DIALOGQUALITY_H
#define DIALOGQUALITY_H

#include <QDialog>

namespace Ui {
class DialogQuality;
}

class DialogQuality : public QDialog
{
    Q_OBJECT

public:
    explicit DialogQuality(QWidget *parent = 0);
    ~DialogQuality();
    int getQuality();

private slots:
    void on_qualityHorizontalSlider_valueChanged(int value);
    void on_qualitySpinBox_valueChanged(int arg1);

private:
    Ui::DialogQuality *ui;
};

#endif // DIALOGQUALITY_H
