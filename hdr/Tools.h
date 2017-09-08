#ifndef TOOLS_H
#define TOOLS_H

#include <QImage>

class Tools
{
public:
    static QImage addNoise(const QImage &img);
    static QImage adjustColors(const QImage &img, int brigh, int contr, float gamma, QVector<int> channels);
    static QImage blendImages(const QList<QImage> &imgList);
    static QImage convolve(const QImage &img, const QVector<QVector<float>> &kernel);
    static QImage grayscale(const QImage &img, double r = .299, double g = .587, double b = .114);
    static QImage Grayscale8ToMono(const QImage &img, int threshold = 127);
    static QImage negative(const QImage &img);
    static QImage normalize(const QImage &img, int yS, int yE, int xS, int xE);
    static QImage pasteImage(const QImage &pastedImg, const QImage &backgroundImg);
    static QImage randomImage(int width, int height, QImage::Format format = QImage::Format_ARGB32);
    static QImage randomizeColors(const QImage &img);
    static QImage sepia(const QImage &img);
    static QImage singleColorImage(int width, int height, QRgb color, QImage::Format format = QImage::Format_ARGB32);
};

#endif // TOOLS_H
