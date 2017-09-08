#include "hdr/Tools.h"
#include <QDebug>
#include <QTime>
#include <QtMath>
#include <ctime>
#include <limits>
#include <random>

using namespace std;

int generateRandomInt(int min, int max)
{
    return min + (qrand() % static_cast<int>(max - min + 1));
}

uchar truncateFloat(float val)
{
    if (val > 255)
        return 255;
    else if (val < 0)
        return 0;
    return val;
}

uchar truncateInt(int val)
{
    if (val > 255)
        return 255;
    else if (val < 0)
        return 0;
    return val;
}

QImage Tools::addNoise(const QImage &img)
{
    QImage noisy(img.width(), img.height(), img.format());
    uchar *noisyData = noisy.bits();
    const uchar *imgData = img.constBits();
    int numBytes = img.byteCount();
    independent_bits_engine<mt19937, CHAR_BIT, uint_fast32_t> ibe;
    ibe.seed(time(0));
    QVector<uchar> rndData(numBytes);
    generate(rndData.begin(), rndData.end(), ref(ibe));
    for (int i = 0; i < numBytes; ++i)
        noisyData[i] = (imgData[i] + rndData[i]) / 2.0;
    return noisy;
}

QImage Tools::adjustColors(const QImage &img, int brigh, int contr, float gamma, QVector<int> channels)
{
    uchar values[256];
    float factor = 259.0f * (contr + 255.0f) / 255.0f / (259.0f - contr);
    for (int i = 0; i < 256; ++i) {
        values[i] = truncateInt(i + brigh);
        values[i] = truncateFloat(factor * (values[i] - 128.0f) + 128.0f);
        values[i] = truncateFloat(255.0f * powf((values[i] / 256.0f), 1 / gamma));
    }
    int height = img.height();
    int width = img.width();
    QImage res(img);
    const uchar *imgData = img.constBits();
    uchar *resData = res.bits();
    int channelCount = channels.count();
    int *c = channels.data();
    if (img.format() == QImage::Format_RGB32 || img.format() == QImage::Format_ARGB32) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x, resData += 4, imgData += 4) {
                for (int i = 0; i < channelCount; ++i)
                    resData[c[i]] = values[imgData[c[i]]];
            }
        }
    } else if (img.format() == QImage::Format_RGB888) {
        int padding = img.bytesPerLine() - 3 * width;
        for (int y = 0; y < height; ++y, resData += padding, imgData += padding) {
            for (int x = 0; x < width; ++x, resData += 3, imgData += 3)
                for (int i = 0; i < channelCount; ++i)
                    resData[c[i]] = values[imgData[c[i]]];
        }
    } else {
        int padding = img.bytesPerLine() - width;
        for (int y = 0; y < height; ++y, resData += padding, imgData += padding) {
            for (int x = 0; x < width; ++x, resData++, imgData++)
                resData[0] = values[imgData[0]];
        }
    }
    return res;
}

QImage Tools::blendImages(const QList<QImage> &imgList)
{
    int imgCount = imgList.count();
    QList<const uchar *> allPixels;
    for (int i = 0; i < imgCount; ++i)
        allPixels.append(imgList[i].constBits());
    int numBytes = imgList[0].byteCount();
    QImage blend(imgList[0].size(), imgList[0].format());
    uchar *blendData = blend.bits();
    for (int j = 0; j < numBytes; ++j) {
        int sum = 0;
        for (int i = 0; i < imgCount; ++i) {
            sum += allPixels[i][0];
            allPixels[i]++;
        }
        blendData[j] = sum / float(imgCount);
    }
    return blend;
}

void normalizeRawData(float *data, uchar *bytes, int width, int height, int stride, float max)
{
    float scaleFactor = 255 / max;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++)
            bytes[y * stride + x] = qAbs(scaleFactor * data[y * stride + x]);
    }
}

QImage Tools::convolve(const QImage &img, const QVector<QVector<float>> &kernel)
{
    int width = img.width();
    int height = img.height();
    const uchar *imgData = img.constBits();
    int kHlfSize = kernel.size() / 2;
    int stride = img.bytesPerLine();
    float *data = new float[stride * height];
    float max = std::numeric_limits<float>::min();
    float **k = new float *[kernel.size()];
    for (int y = 0; y < kernel.size(); ++y)
         k[y] = new float[kernel[0].size()];
    for (int y = 0; y < kernel.size(); ++y)
        for (int x = 0; x < kernel[0].size(); ++x)
            k[y][x] = kernel[y][x];
    for (int y = kHlfSize; y < height - kHlfSize; ++y) {
        for (int x = kHlfSize; x < width - kHlfSize; ++x) {
            int sum = 0;
            for (int ky = -kHlfSize; ky <= kHlfSize; ++ky) {
                for (int kx = -kHlfSize; kx <= kHlfSize; ++kx)
                    sum += imgData[(y + ky) * stride + x + kx] * k[ky + kHlfSize][kx + kHlfSize];
            }
            data[y * stride + x] = sum;
            if (sum > max)
                max = sum;
        }
    }
    uchar *bytes = new uchar[stride * height];
    normalizeRawData(data, bytes, width, height, stride, max);
    delete[] data;
    for (int y = 0; y < kernel.size(); ++y)
         delete[] k[y];
    delete[] k;
    QImage conv(bytes, width, height, stride, img.format());
    return conv;
}

QImage Tools::grayscale(const QImage &img, double r, double g, double b)
{
    int width = img.width();
    int height = img.height();
    QImage gray(width, height, QImage::Format_Grayscale8);
    uchar *grayData = gray.bits();
    const uchar *imgData = img.constBits();
    int padding = gray.bytesPerLine() - width;
    for (int y = 0; y < height; ++y, grayData += padding) {
        for (int x = 0; x < width; ++x, grayData++, imgData += 4)
            grayData[0] = imgData[2] * r + imgData[1] * g + imgData[0] * b;
    }
    return gray;
}

QImage Tools::Grayscale8ToMono(const QImage &img, int threshold)
{
    QImage mono(img.width(), img.height(), img.format());
    uchar *monoData = mono.bits();
    const uchar *imgData = img.constBits();
    int numBytes = img.byteCount();
    for (int i = 0; i < numBytes; ++i)
        monoData[i] = imgData[i] >= threshold ? 255 : 0;
    return mono.convertToFormat(QImage::Format_Mono);
}

QImage Tools::negative(const QImage &img)
{
    QImage neg(img.width(), img.height(), img.format());
    uchar *negData = neg.bits();
    const uchar *imgData = img.constBits();
    int numBytes = img.byteCount();
    for (int i = 0; i < numBytes; ++i)
        negData[i] = 255 - imgData[i];
    return neg;
}

QImage Tools::normalize(const QImage &img, int yS, int yE, int xS, int xE)
{
    uchar minI = 255;
    uchar maxI = 0;
    const uchar *imgData = img.constBits();
    int width = img.width();
    int height = img.height();
    if (img.format() == QImage::Format_Indexed8 || img.format() == QImage::Format_Grayscale8) {
        int padding = img.bytesPerLine() - width;
        for (int y = yS; y < yE; ++y, imgData += padding) {
            for (int x = xS; x < xE; ++x, imgData++) {
                if (imgData[0] < minI)
                    minI = imgData[0];
                if (imgData[0] > maxI)
                    maxI = imgData[0];
            }
        }
    } else {
        for (int y = yS; y < yE; ++y) {
            for (int x = xS; x < xE; ++x, imgData += 4) {
                for (int i = 0; i < 3; ++i) {
                    if (imgData[i] < minI)
                        minI = imgData[i];
                    if (imgData[i] > maxI)
                        maxI = imgData[i];
                }
            }
        }
    }
    double range = maxI - minI;
    QImage norm(width, height, img.format());
    uchar *normData = norm.bits();
    int numBytes = img.byteCount();
    imgData -= numBytes;
    uchar values[256];
    double normFactor = range / 255;
    for (int i = 0; i < 256; ++i)
        values[i] = (i - minI) / normFactor;
    for (int i = 0; i < numBytes; ++i)
        normData[i] = values[imgData[i]];
    return norm;
}

QImage Tools::pasteImage(const QImage &pastedImg, const QImage &backgroundImg)
{
    QImage mergedImg;
    if (pastedImg.width() >= backgroundImg.width()
            && pastedImg.height() >= backgroundImg.height())
        mergedImg = pastedImg;
    return mergedImg;
}

QImage Tools::randomImage(int width, int height, QImage::Format format)
{
    int numBytes = 0;
    if (format == QImage::Format_ARGB32)
        numBytes = 4 * width * height;
    else if (format == QImage::Format_RGB888) {
        int stride = (3 * width + 3) & ~3;
        numBytes = stride * height;
    }
    else if (format == QImage::Format_Grayscale8) {
        int stride = (width + 3) & ~3;
        numBytes = stride * height;
    }
    uchar *rndData = new uchar[numBytes];
    independent_bits_engine<mt19937, CHAR_BIT, uint_fast32_t> ibe;
    ibe.seed(time(0));
    generate(&rndData[0], &rndData[numBytes], ref(ibe));
    QImage rnd(rndData, width, height, format);
    return rnd;
}

QImage Tools::randomizeColors(const QImage &img)
{
    int height = img.height();
    int width = img.width();
    QImage res(width, height, img.format());
    uchar *resData = res.bits();
    const uchar *imgData = img.constBits();
    uchar values[256];
    for (int i = 0; i < 256; ++i)
        values[i] = generateRandomInt(i, 256);
    if (img.format() == QImage::Format_Grayscale8 || img.format() == QImage::Format_Indexed8) {
        int padding = img.bytesPerLine() - width;
        for (int y = 0; y < height; ++y, resData += padding, imgData += padding) {
            for (int x = 0; x < width; ++x, resData++, imgData++)
                resData[0] = values[imgData[0]];
        }
    } else if (img.format() == QImage::Format_RGB32) {
        uchar values1[3][256];
        for (int i = 0; i < 256; ++i) {
            for (int j = 0; j < 3; ++j)
                values1[j][i] = generateRandomInt(i, 256);
        }
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x, resData += 4, imgData += 4) {
                for (int i = 0; i < 3; ++i)
                    resData[i] = values1[i][imgData[i]];
            }
        }
    }
    return res;
}

QImage Tools::sepia(const QImage &img)
{
    int width = img.width();
    int height = img.height();
    const uchar *imgData = img.constBits();
    if (img.format() == QImage::Format_RGB32 || img.format() == QImage::Format_ARGB32) {
        QImage sep(width, height, img.format());
        uchar *sepData = sep.bits();
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x, sepData += 4, imgData += 4) {
                float r = imgData[2] * .393f + imgData[1] * .769f + imgData[0] * .189f;
                float g = imgData[2] * .349f + imgData[1] * .686f + imgData[0] * .168;
                float b = imgData[2] * .272f + imgData[1] * .534f + imgData[0] * .131;
                sepData[2] = r > 255.0f ? 255 : r;
                sepData[1] = g > 255.0f ? 255 : g;
                sepData[0] = b > 255.0f ? 255 : b;
            }
        }
        return sep;
    } else if (img.format() == QImage::Format_Grayscale8 || img.format() == QImage::Format_Indexed8) {
        QImage sep(width, height, QImage::Format_RGB888);
        uchar *sepData = sep.bits();
        int grayPadding = img.bytesPerLine() - width;
        int rgbPadding = sep.bytesPerLine() - 3 * width;
        for (int y = 0; y < height; ++y, sepData += rgbPadding, imgData += grayPadding) {
            for (int x = 0; x < width; ++x, sepData += 3, imgData++) {
                float r = imgData[0] * .393f + imgData[0] * .769f + imgData[0] * .189f;
                float g = imgData[0] * .349f + imgData[0] * .686f + imgData[0] * .168f;
                float b = imgData[0] * .272f + imgData[0] * .534f + imgData[0] * .131f;
                sepData[0] = r > 255.0f ? 255 : r;
                sepData[1] = g > 255.0f ? 255 : g;
                sepData[2] = b > 255.0f ? 255 : b;
            }        
        }
        return sep;
    }
}

QImage Tools::singleColorImage(int width, int height, QRgb color, QImage::Format format)
{
    int numBytes = 0;
    QImage img(width, height, format);
    uchar *imgData = img.bits();
    if (format == QImage::Format_ARGB32) {
        int pixelCount = width * height;
        QRgb *argb = (QRgb *)img.bits();
        for (int i = 0; i < pixelCount; i++)
            argb[i] = color;
    } else if (format == QImage::Format_RGB888) {
        int stride = (3 * width + 3) & ~3;
        numBytes = stride * height;
        int padding = img.bytesPerLine() - 3 * width;
        uchar *argb = (uchar *)&color;
        for (int y = 0; y < height; ++y, imgData += padding) {
            for (int x = 0; x < width; ++x, imgData += 3) {
                imgData[0] = argb[2];
                imgData[1] = argb[1];
                imgData[2] = argb[0];
            }
        }
    } else if (format == QImage::Format_Grayscale8) {
        int stride = (width + 3) & ~3;
        numBytes = stride * height;
        uchar *value = (uchar *)&color;
        memset(imgData, value[3], numBytes);
    }
    return img;
}
