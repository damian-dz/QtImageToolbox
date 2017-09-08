#include "hdr/MainWindow.h"
#include "ui_MainWindow.h"

#include "hdr/DialogAdjust.h"
#include "hdr/DialogBinarize.h"
#include "hdr/DialogBlend.h"
#include "hdr/DialogConvolve.h"
#include "hdr/DialogGrayscale.h"
#include "hdr/DialogQuality.h"
#include "hdr/DialogPixelate.h"
#include "hdr/DialogRandom.h"
#include "hdr/DialogResize.h"
#include "hdr/DialogRotate.h"
#include "hdr/DialogSingleColor.h"
#include "hdr/Stats.h"
#include "hdr/Tools.h"

#include <QClipboard>
#include <QCloseEvent>
#include <QFileDialog>
#include <QHash>
#include <QMessageBox>
#include <QScreen>
#include <QScrollBar>
#include <QTime>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      mOriginal(-1)
{
    ui->setupUi(this);
    imageLabelandScrollArea();
    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 4);
    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
    qsrand(QTime::currentTime().msec());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::actionShowContextMenu(const QPoint &pos)
{
    QPoint globalPos = mImageLabel->mapToGlobal(pos);
    QMenu contextMenu(this);
    contextMenu.addAction("Copy", this, SLOT(on_actionCopy_triggered()));
    contextMenu.addAction("Cut");
    contextMenu.addAction("Paste", this, SLOT(on_actionPaste_triggered()));
    contextMenu.addSeparator();
    int x = qRound(pos.x() / mScaleFactor);
    int y = qRound(pos.y() / mScaleFactor);
    mPosition = QPoint(x, y);
    mColor = !mOriginal ? mModifiedImg.pixelColor(x, y) : mOriginalImg.pixelColor(x, y);
    contextMenu.addAction("Pixel Info", this, SLOT(actionShowPixelInfo()));
    contextMenu.exec(globalPos);
}

void MainWindow::actionShowPixelInfo()
{
    QString coors = QString("x: %1, y: %2")
            .arg(mPosition.x()).arg(mPosition.y());
    QString rgb = QString("A: %1, R: %2, G: %3, B: %4")
            .arg(mColor.alpha()).arg(mColor.red()).arg(mColor.green()).arg(mColor.blue());
    QMessageBox::information(this, "Pixel Info", coors + "\n" + rgb);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this, "About Qt");
}

void MainWindow::on_actionAbout_triggered()
{

}

void MainWindow::on_actionAdd_Noise_triggered()
{
    mModifiedImg = Tools::addNoise(mOriginalImg);
    setAndAdjustPixmap(mModifiedImg);
}

void MainWindow::on_actionAdjust_Colors_triggered()
{
    DialogAdjust adjustDialog(mOriginalImg.format());
    adjustDialog.setModal(true);
    if (adjustDialog.exec()) {
        int brightness = adjustDialog.getBrightness();
        int contrast = adjustDialog.getContrast();
        float gamma = adjustDialog.getGamma();
        QVector<int> channels = adjustDialog.getChannelIndices();
        mModifiedImg = Tools::adjustColors(mOriginalImg, brightness, contrast, gamma, channels);
        setAndAdjustPixmap(mModifiedImg);
    }
}

void MainWindow::on_actionBinarize_triggered()
{
    DialogBinarize binarizeDialog;
    binarizeDialog.setModal(true);
    binarizeDialog.setFixedSize(400, 128);
    if (binarizeDialog.exec()) {
        int thresh = binarizeDialog.getThreshold();
        mModifiedImg = Tools::Grayscale8ToMono(mOriginalImg.convertToFormat(QImage::Format_Grayscale8), thresh);
        setAndAdjustPixmap(mModifiedImg);
    }
}

void MainWindow::on_actionBlend_triggered()
{
    DialogBlend blendDialog;
    blendDialog.setModal(true);
    if (blendDialog.exec()) {
        QList<QImage> imgList = blendDialog.getImageList();
        mOriginalImg = Tools::blendImages(imgList);
        mScaleFactor = 1;
        setAndAdjustPixmap(mOriginalImg);
    }
}

void MainWindow::on_actionConvolve_triggered()
{
    DialogConvolve convolveDialog;
    convolveDialog.setModal(true);
    convolveDialog.setFixedSize(410, 270);
    if (convolveDialog.exec()) {
        auto kernel = convolveDialog.getMultipliedFlippedKernel();
        mModifiedImg = Tools::convolve(mOriginalImg, kernel);
        setAndAdjustPixmap(mModifiedImg);
    }
}

void MainWindow::on_actionCopy_triggered()
{
    QGuiApplication::clipboard()->setImage(mImageLabel->pixmap()->toImage());
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionFit_to_Window_triggered()
{
    bool fitToWindow = ui->actionFit_to_Window->isChecked();
    mScrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow)
        on_actionOriginal_Size_triggered();
}

void MainWindow::on_actionFlip_Horizontally_triggered()
{
    mModifiedImg = mOriginalImg.mirrored(true, false);
    mImageLabel->setPixmap(QPixmap::fromImage(mModifiedImg));
}

void MainWindow::on_actionFlip_Vertically_triggered()
{
    mModifiedImg = mOriginalImg.mirrored(false, true);
    mImageLabel->setPixmap(QPixmap::fromImage(mModifiedImg));
}

void MainWindow::on_actionGrayscale_triggered()
{
    DialogGrayscale grayscaleDialog(this);
    grayscaleDialog.setModal(true);
    grayscaleDialog.setFixedSize(320, 90);
    if (grayscaleDialog.exec()) {
        auto params = grayscaleDialog.getParameters();
        if (!grayscaleDialog.averageChecked()) {
            double r = std::get<0>(params);
            double g = std::get<1>(params);
            double b = std::get<2>(params);
            mModifiedImg = Tools::grayscale(mOriginalImg, r, g, b);
        } else {
            double a = 1 / 3.0;
            mModifiedImg = Tools::grayscale(mOriginalImg, a, a, a);
        }
        setAndAdjustPixmap(mModifiedImg);
        mOriginal = 0;
    }
}

void MainWindow::on_actionHistogram_triggered()
{
    QList<QHash<uchar, int>> histData = Stats::prepareHistogramData(mOriginalImg);
    mHistogramForm = new FormHistogram(histData);
    mHistogramForm->setAttribute(Qt::WA_DeleteOnClose);
    mHistogramForm->show();
}

void MainWindow::on_actionNegative_triggered()
{
    mModifiedImg = QImage(mOriginalImg);
    mModifiedImg.bits();
    mModifiedImg.invertPixels();
    setAndAdjustPixmap(mModifiedImg);
}

void MainWindow::on_actionNormalize_triggered()
{
    mModifiedImg = Tools::normalize(mOriginalImg, 0, mOriginalImg.height(), 0, mOriginalImg.width());
    setAndAdjustPixmap(mModifiedImg);
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Load Image"),
                "/",
                "Image Files (*.png *.jpg *.bmp *.tif);;All Files (*.*)");
    loadImage(filename);
}

void MainWindow::on_actionOriginal_Size_triggered()
{
    mImageLabel->adjustSize();
    mScaleFactor = 1;
}

void MainWindow::on_actionPaste_triggered()
{
    mOriginalImg = QGuiApplication::clipboard()->image();
    setAndAdjustPixmap(mOriginalImg, true);
}

void MainWindow::on_actionPixel_Count_triggered()
{
    int count = Stats::pixelCount(mOriginalImg);
    QString message = "The image consists of " + QString::number(count) + " pixels.";
    QMessageBox::information(this, "Pixel Count", message);
}

void MainWindow::on_actionPixelate_triggered()
{
    int width = mOriginalImg.width();
    int height = mOriginalImg.height();
    DialogPixelate pixelateDialog(qMin(width, height) / 2);
    pixelateDialog.setFixedSize(400, 120);
    pixelateDialog.setModal(true);
    if (pixelateDialog.exec()) {
        int pixelSize =  pixelateDialog.getPixelSize();
        mModifiedImg = mOriginalImg.scaled(width / pixelSize, height / pixelSize).scaled(width, height);
        setAndAdjustPixmap(mModifiedImg);
    }
}

void MainWindow::on_actionRandom_Pixel_Image_triggered()
{
    DialogRandom randomDialog;
    randomDialog.setModal(true);
    randomDialog.setFixedSize(256, 110);
    if (randomDialog.exec()) {
        auto params = randomDialog.getParameters();
        int width = params.first.width();
        int height = params.first.height();
        QImage::Format format = params.second;
        mOriginalImg = Tools::randomImage(width, height, format);
        mScaleFactor = 1;
        setAndAdjustPixmap(mOriginalImg);
    }
}

void MainWindow::on_actionRandomize_Colors_triggered()
{
    mModifiedImg = Tools::randomizeColors(mOriginalImg);
    setAndAdjustPixmap(mModifiedImg);
}

void MainWindow::on_actionResize_triggered()
{
    QSize originalSize(mOriginalImg.width(), mOriginalImg.height());
    DialogResize resizeDialog(originalSize);
    resizeDialog.setModal(true);
    if (resizeDialog.exec()) {
        auto params = resizeDialog.getParameters();
        QSize modifiedSize = params.first;
        Qt::TransformationMode mode = params.second;
        mModifiedImg = mOriginalImg.scaled(modifiedSize, Qt::IgnoreAspectRatio, mode);
        setAndAdjustPixmap(mModifiedImg);
    }
}

void MainWindow::on_actionRotate_triggered()
{
    DialogRotate rotateDialog;
    rotateDialog.setModal(true);
    if (rotateDialog.exec()) {
        double angle = rotateDialog.getAngle();
        QTransform matrix;
        matrix.rotate(angle);
        mModifiedImg = mOriginalImg.transformed(matrix);
        setAndAdjustPixmap(mModifiedImg);
    }
}

void MainWindow::on_actionSave_As_triggered()
{
    QString selectedFilter;
    QString filename = QFileDialog::getSaveFileName(
                this,
                tr("Save Image"),
                "/",
                "BMP Uncompressed Image (*.bmp);;PNG Image (*.png);;JPG Image (*.jpg);;All Files (*.*)",
                &selectedFilter);
    if (filename.isNull() || filename.isEmpty())
        return;
    QString selectedFormat = selectedFilter.mid(0, 3);
    int quality = -1;
    if (selectedFormat != "BMP") {
        DialogQuality qualityDialog;
        qualityDialog.setModal(true);
        if (qualityDialog.exec()) {
            quality = qualityDialog.getQuality();
            if (!mModifiedImg.isNull())
                mModifiedImg.save(filename, selectedFormat.toLatin1().data(), quality);
            else if (!mOriginalImg.isNull())
                mOriginalImg.save(filename, selectedFormat.toLatin1().data(), quality);
        }
    } else {
        if (!mModifiedImg.isNull())
            mModifiedImg.save(filename, selectedFormat.toLatin1().data(), quality);
        else if (!mOriginalImg.isNull())
            mOriginalImg.save(filename, selectedFormat.toLatin1().data(), quality);
    }
}

void MainWindow::on_actionSepia_triggered()
{
    mModifiedImg = Tools::sepia(mOriginalImg);
    setAndAdjustPixmap(mModifiedImg);
}

void MainWindow::on_actionSingle_Color_Image_triggered()
{
    DialogSingleColor singleColorDialog(this);
    singleColorDialog.setModal(true);
    singleColorDialog.setFixedSize(255, 150);
    if (singleColorDialog.exec()) {
        QRgb color = singleColorDialog.getColor();
        auto params = singleColorDialog.getParameters();
        int width = params.first.width();
        int height = params.first.height();
        QImage::Format format = params.second;
        mOriginalImg = Tools::singleColorImage(width, height, color, format);
        setAndAdjustPixmap(mOriginalImg, true);
        singleColorDialog.deleteLater();
    }
}

void MainWindow::on_actionUnique_Colors_triggered()
{
    int count = Stats::countColors(mOriginalImg);
    QString isAre = count < 2 ? "There is " : "There are ";
    QString colorS = count < 2 ? " unique color " : " unique colors ";
    QString message = isAre + QString("%L1").arg(count) + colorS + "in the image.";
    QMessageBox::information(this, "Unique Colors", message);
}

void MainWindow::on_actionZoom_In_triggered()
{
    scaleImage(1.25);
}

void MainWindow::on_actionZoom_Out_triggered()
{
    scaleImage(0.8);
}

void MainWindow::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value() + ((factor - 1) * scrollBar->pageStep() / 2)));
}

void MainWindow::imageLabelandScrollArea()
{
    mImageLabel = new QLabel(this);
    mImageLabel->setBackgroundRole(QPalette::Dark);
    mImageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    mImageLabel->setScaledContents(true);
    mImageLabel->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mImageLabel, SIGNAL(customContextMenuRequested(const QPoint &)),
        this, SLOT(actionShowContextMenu(const QPoint &)));
    mScrollArea = new QScrollArea(this);
    mScrollArea->setBackgroundRole(QPalette::Dark);
    mScrollArea->setWidget(mImageLabel);
    setCentralWidget(mScrollArea);
    mStatusLabel = new QLabel(this);
    ui->statusBar->addWidget(mStatusLabel);
}

void MainWindow::loadImage(const QString &filename)
{
    if (filename.isNull() || filename.isEmpty())
        return;
    QImage newImage(filename);
    mOriginalImg = newImage;
    mModifiedImg = QImage();
    mScaleFactor = 1;
    mOriginal = 1;
    setAndAdjustPixmap(mOriginalImg);
    updateActions(mOriginalImg.format());
    QWidget::setWindowTitle(filename + " — Qt Image Toolbox");
}

void MainWindow::scaleImage(double factor)
{
    mScaleFactor *= factor;
    mImageLabel->resize(mScaleFactor * mImageLabel->pixmap()->size());
    adjustScrollBar(mScrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(mScrollArea->verticalScrollBar(), factor);
    ui->actionZoom_In->setEnabled(mScaleFactor < 3.0);
    ui->actionZoom_Out->setEnabled(mScaleFactor > 0.115);
}

void MainWindow::setAndAdjustPixmap(const QImage &img, bool first)
{
    mImageLabel->setPixmap(QPixmap::fromImage(img));
    if (first)
        mScaleFactor = 1;
    mScaleFactor != 1 ? mImageLabel->resize(mScaleFactor * mImageLabel->pixmap()->size())
                     : mImageLabel->adjustSize();
    QString format;
    switch (img.format()) {
    case QImage::Format_Mono:
        format = "Mono (1 bpp)";
        break;
    case QImage::Format_Grayscale8:
        format = "Grayscale (8 bpp)";
        break;
    case QImage::Format_Indexed8:
        format = "Indexed (8 bpp)";
        break;
    case QImage::Format_RGB888:
        format = "RGB (24 bpp)";
        break;
    case QImage::Format_RGB32:
        format = "RGB (32 bpp)";
        break;
    case QImage::Format_ARGB32:
        format = "ARGB (32 bpp)";
        break;
    default:
        format = "Unknown (" + QString::number(img.depth()) + " bpp)";
        break;
    }
    const QString message = QString("Width: %1 Height: %2 Pixel Format: %3")
            .arg(img.width()).arg(img.height()).arg(format);
    mStatusLabel->setText(message);
}

void MainWindow::updateActions(QImage::Format format)
{
    bool isGrayOrIndx8 = format == QImage::Format_Grayscale8 || format == QImage::Format_Indexed8;
    ui->actionBinarize->setEnabled(true);
    ui->actionAdjust_Colors->setEnabled(true);
    ui->actionConvolve->setEnabled(true);
    ui->actionFlip_Horizontally->setEnabled(true);
    ui->actionFlip_Vertically->setEnabled(true);
    ui->actionAdd_Noise->setEnabled(true);
    ui->actionRandomize_Colors->setEnabled(true);
    ui->actionGrayscale->setEnabled(!isGrayOrIndx8);
    ui->actionHistogram->setEnabled(true);
    ui->actionNegative->setEnabled(true);
    ui->actionNormalize->setEnabled(true);
    ui->actionPixel_Count->setEnabled(true);
    ui->actionPixelate->setEnabled(true);
    ui->actionResize->setEnabled(true);
    ui->actionSepia->setEnabled(true);
    ui->actionRotate->setEnabled(true);
    ui->actionUnique_Colors->setEnabled(true);
    ui->actionZoom_In->setEnabled(true);
    ui->actionZoom_Out->setEnabled(true);
    ui->actionSave_As->setEnabled(true);
}
