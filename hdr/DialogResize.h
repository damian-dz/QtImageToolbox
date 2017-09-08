#ifndef DIALOGRESIZE_H
#define DIALOGRESIZE_H

#include <QDialog>

namespace Ui {
class DialogResize;
}

class DialogResize : public QDialog
{
    Q_OBJECT

public:
    explicit DialogResize(QSize dims, QWidget *parent = 0);
    ~DialogResize();
    QPair<QSize, Qt::TransformationMode> getParameters();

private slots:
    void on_heightLineEdit_textChanged(const QString &arg1);
    void on_heightLineEdit_textEdited(const QString &arg1);
    void on_percentageHeightLineEdit_textChanged(const QString &arg1);
    void on_percentageHeightLineEdit_textEdited(const QString &arg1);
    void on_percentageWidthLineEdit_textEdited(const QString &arg1);
    void on_perecentageWidthLineEdit_textChanged(const QString &arg1);
    void on_widthLineEdit_textChanged(const QString &arg1);
    void on_widthLineEdit_textEdited(const QString &arg1);

private:
    Ui::DialogResize *ui;
    QSize originalSize;
};

#endif // DIALOGRESIZE_H
