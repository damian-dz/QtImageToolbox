#ifndef DIALOGSINGLECOLOR_H
#define DIALOGSINGLECOLOR_H

#include <QDialog>
#include <QColorDialog>

namespace Ui {
class DialogSingleColor;
}

class DialogSingleColor : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSingleColor(QWidget *parent = 0);
    ~DialogSingleColor();
    QRgb getColor();
    QPair<QSize, QImage::Format> getParameters();

private slots:
    void on_colorDialogButton_clicked();
    void on_formatComboBox_currentIndexChanged(int index);

private:
    Ui::DialogSingleColor *ui;
    bool alphaChannel;
};

#endif // DIALOGSINGLECOLOR_H
