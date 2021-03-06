#ifndef DIALOGROTATE_H
#define DIALOGROTATE_H

#include <QDialog>

namespace Ui {
class DialogRotate;
}

class DialogRotate : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRotate(QWidget *parent = 0);
    ~DialogRotate();
    double getAngle();
    bool originalSizeChecked();
    int selectedInterpolationMode();

private:
    Ui::DialogRotate *ui;
};

#endif // DIALOGROTATE_H
