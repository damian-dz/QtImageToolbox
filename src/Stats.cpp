#include "hdr/Stats.h"

#include <QDebug>
#include <QTime>

int Stats::countColors(const QImage &img)
{
    QSet<int> colorSet;
    int numBytes = img.byteCount();
    const int *imgPixels = (const int *)img.bits();
    int pixelCount = numBytes / 4;
    for (int i = 0; i < pixelCount; ++i)
        colorSet.insert(imgPixels[i]);
    return colorSet.size();
}

int Stats::pixelCount(const QImage &img)
{
    return img.width() * img.height();
}

QList<QHash<uchar, int>> Stats::prepareHistogramData(const QImage &img)
{
    int width = img.width();
    int height = img.height();
    const uchar *imgPixels = img.bits();
    QList<QHash<uchar, int>> channels;
    if (img.format() == QImage::Format_Indexed8 || img.format() == QImage::Format_Grayscale8 ) {
        QHash<uchar, int> values;
        int padding = img.bytesPerLine() - width;
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (values.contains(imgPixels[0]))
                    values[imgPixels[0]]++;
                else
                    values.insert(imgPixels[0], 1);
                imgPixels++;
            }
            imgPixels += padding;
        }
        channels.append(values);
    } else {
        QHash<uchar, int> valuesR;
        QHash<uchar, int> valuesG;
        QHash<uchar, int> valuesB;
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (valuesR.contains(imgPixels[2]))
                    valuesR[imgPixels[2]]++;
                else
                    valuesR.insert(imgPixels[2], 1);
                if (valuesG.contains(imgPixels[1]))
                    valuesG[imgPixels[1]]++;
                else
                    valuesG.insert(imgPixels[1], 1);
                if (valuesB.contains(imgPixels[0]))
                    valuesB[imgPixels[0]]++;
                else
                    valuesB.insert(imgPixels[0], 1);
                imgPixels += 4;
            }
        }
        channels.append(valuesR);
        channels.append(valuesG);
        channels.append(valuesB);
    }
    return channels;
}
