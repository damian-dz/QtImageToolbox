#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>

#include "hdr/FormHistogram.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void actionShowContextMenu(const QPoint &pos);
    void actionShowPixelInfo();
    void closeEvent(QCloseEvent *event);
    void on_actionAbout_Qt_triggered();
    void on_actionAbout_triggered();
    void on_actionAdd_Noise_triggered();
    void on_actionAdjust_Colors_triggered();
    void on_actionBinarize_triggered();
    void on_actionBlend_triggered();
    void on_actionConvolve_triggered();
    void on_actionCopy_triggered();
    void on_actionExit_triggered();
    void on_actionFit_to_Window_triggered();
    void on_actionGrayscale_triggered();
    void on_actionHistogram_triggered();
    void on_actionFlip_Horizontally_triggered();
    void on_actionNegative_triggered();
    void on_actionNormalize_triggered();
    void on_actionOpen_triggered();
    void on_actionOriginal_Size_triggered();
    void on_actionPaste_triggered();
    void on_actionPixel_Count_triggered();
    void on_actionPixelate_triggered();
    void on_actionRandom_Pixel_Image_triggered();
    void on_actionRandomize_Colors_triggered();
    void on_actionResize_triggered();
    void on_actionRotate_triggered();
    void on_actionSave_As_triggered();
    void on_actionSepia_triggered();
    void on_actionSingle_Color_Image_triggered();
    void on_actionUnique_Colors_triggered();
    void on_actionFlip_Vertically_triggered();
    void on_actionZoom_In_triggered();
    void on_actionZoom_Out_triggered(); 

private:
    Ui::MainWindow *ui;
    char mOriginal;
    double mScaleFactor;
    FormHistogram *mHistogramForm;
    QImage mModifiedImg;
    QImage mOriginalImg;
    QLabel *mImageLabel;
    QLabel *mStatusLabel;
    QScrollArea *mScrollArea;
    QPoint mPosition;
    QColor mColor;
    void adjustScrollBar(QScrollBar *scrollBar, double factor);
    void imageLabelandScrollArea();
    void loadImage(const QString &filename);
    void scaleImage(double factor);
    void setAndAdjustPixmap(const QImage &img, bool first = false);
    void updateActions(QImage::Format format);

};

#endif // MAINWINDOW_H
