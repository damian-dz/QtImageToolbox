#ifndef STATS_H
#define STATS_H

#include <QImage>
#include <QHash>

class Stats
{
public:
    static int countColors(const QImage &img);
    static int pixelCount(const QImage &img);
    static QList<QHash<uchar, int> > prepareHistogramData(const QImage &img);
};

#endif // STATS_H
