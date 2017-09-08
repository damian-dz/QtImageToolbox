#include "hdr/DialogBlend.h"
#include "ui_DialogBlend.h"

#include <QFileSystemModel>
#include <QStandardPaths>
#include <QDebug>
#include <QMessageBox>

DialogBlend::DialogBlend(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::DialogBlend)
{
    ui->setupUi(this);
    ui->horizontalLayout->setStretchFactor(ui->folderTreeView, 70);
    ui->horizontalLayout->setStretchFactor(ui->selectedFilesListWidget, 30);
    dirModel = new QFileSystemModel(this);
    dirModel->setRootPath(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    QStringList filters;
    filters << "*.bmp" << "*.jpg" << "*.png";
    dirModel->setNameFilters(filters);
    dirModel->setNameFilterDisables(false);
    ui->folderTreeView->setModel(dirModel);
    ui->folderTreeView->setColumnWidth(0, 220);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);
}

void DialogBlend::on_addButton_clicked()
{
    QString path = dirModel->fileInfo(ui->folderTreeView->currentIndex()).absoluteFilePath();
    QImage img(path);
    if (absPaths.count() == 0) {
        firstWidth = img.width();
        firstHeight = img.height();
        firstFormat = img.format();
    } else {
        if (firstWidth != img.width() || firstHeight != img.height()) {
            QMessageBox::critical(this, "Error", "All of the images must have the same dimensions.");
            return;
        }
        if (firstFormat != img.format()) {
            QMessageBox::critical(this, "Error", "All of the images must have the same pixel format.");
            return;
        }
    }
    imgList.append(img);
    if (!absPaths.contains(path)) {
        absPaths.append(path);
        QString fileName = dirModel->fileInfo(ui->folderTreeView->currentIndex()).fileName();
        ui->selectedFilesListWidget->addItem(fileName);
    } else
        QMessageBox::critical(this, "Error", "The selected file is already on the list.");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(absPaths.count() - 1);
    ui->folderTreeView->setFocus();
}

void DialogBlend::on_removeButton_clicked()
{
    absPaths.removeAt(ui->selectedFilesListWidget->currentRow());
    imgList.removeAt(ui->selectedFilesListWidget->currentRow());
    delete ui->selectedFilesListWidget->currentItem();
    if (absPaths.count() < 2)
    ui->buttonBox->button(QDialogButtonBox::Ok)->setDisabled(true);
}

QList<QImage> DialogBlend::getImageList()
{
    return imgList;
}

DialogBlend::~DialogBlend()
{
    delete ui;
}
