#ifndef DIALOGGRAYSCALE_H
#define DIALOGGRAYSCALE_H

#include <QDialog>

namespace Ui {
class DialogGrayscale;
}

class DialogGrayscale : public QDialog
{
    Q_OBJECT

public:
    explicit DialogGrayscale(QWidget *parent = 0);
    std::tuple<double, double, double> getParameters();
    bool averageChecked();
    ~DialogGrayscale();

private slots:
    void on_averageCheckBox_clicked(bool checked);

private:
    Ui::DialogGrayscale *ui;
};

#endif // DIALOGGRAYSCALE_H
