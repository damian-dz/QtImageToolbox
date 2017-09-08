#ifndef DIALOGRANDOM_H
#define DIALOGRANDOM_H

#include <QDialog>

namespace Ui {
class DialogRandom;
}

class DialogRandom : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRandom(QWidget *parent = 0);
    ~DialogRandom();
    QPair<QSize, QImage::Format> getParameters();  

private:
    Ui::DialogRandom *ui;
};

#endif // DIALOGRANDOM_H
