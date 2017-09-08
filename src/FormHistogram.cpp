#include "hdr/FormHistogram.h"
#include "ui_FormHistogram.h"

#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QValueAxis>

using namespace QtCharts;

#include <QDebug>
#include <QVBoxLayout>

FormHistogram::FormHistogram(const QList<QHash<uchar, int>> &channels, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::FormHistogram)
{
    ui->setupUi(this);
    drawHistogram(channels);
}

void FormHistogram::drawHistogram(QList<QHash<uchar, int>> channels)
{
    int channelCount = channels.count();
    QVBoxLayout *verticalLayout = new QVBoxLayout(this);
    for (int n = 0; n < channelCount; ++n) {
        QChart *chart = new QChart;
        QLineSeries *lowerSeries = 0;
        QLineSeries *upperSeries = new QLineSeries(chart);
        const int numBins = 256;
        for (int i = 0; i < numBins; ++i)
            upperSeries->append(i, channels[n][i]);
        QAreaSeries *area = new QAreaSeries(upperSeries, lowerSeries);
        if (n == 0 && channelCount == 1)
            area->setColor(QColor::fromRgb(127, 127, 127));
        else if (n == 0)
            area->setColor(QColor::fromRgb(255, 0, 0));
        else if (n == 1)
            area->setColor(QColor::fromRgb(0, 255, 0));
        else if (n == 2)
            area->setColor(QColor::fromRgb(0, 0, 255));
        area->setUseOpenGL(true);
        chart->addSeries(area);
        QValueAxis *axisX = new QValueAxis;
        axisX->setTickCount(16);
        axisX->setLabelFormat("%d");
        QValueAxis *axisY = new QValueAxis;
        axisY->setLabelFormat("%d");
        chart->setAxisX(axisX, area);
        chart->setAxisY(axisY, area);
        chart->legend()->hide();
        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        verticalLayout->addWidget(chartView);
    }
}

FormHistogram::~FormHistogram()
{
    delete ui;
}
