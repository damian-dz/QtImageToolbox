#ifndef DIALOGBLEND_H
#define DIALOGBLEND_H

#include <QDialog>
#include <QFileSystemModel>
#include <QImage>

namespace Ui {
class DialogBlend;
}

class DialogBlend : public QDialog
{
    Q_OBJECT

public:
    explicit DialogBlend(QWidget *parent = 0);
    QList<QImage> getImageList();
    ~DialogBlend();

private slots:
    void on_addButton_clicked();
    void on_removeButton_clicked();

private:
    Ui::DialogBlend *ui;
    QFileSystemModel *dirModel;
    QStringList absPaths;
    int firstWidth;
    int firstHeight;
    QImage::Format firstFormat;
    QList<QImage> imgList;
};

#endif // DIALOGBLEND_H
