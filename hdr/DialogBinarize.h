#ifndef DIALOGBINARIZE_H
#define DIALOGBINARIZE_H

#include <QDialog>

namespace Ui {
class DialogBinarize;
}

class DialogBinarize : public QDialog
{
    Q_OBJECT

public:
    explicit DialogBinarize(QWidget *parent = 0);
    int getThreshold();
    ~DialogBinarize();

private slots:
    void on_thresholdHorizontalSlider_valueChanged(int value);
    void on_thresholdLineEdit_textChanged(const QString &arg1);

private:
    Ui::DialogBinarize *ui;
};

#endif // DIALOGBINARIZE_H
