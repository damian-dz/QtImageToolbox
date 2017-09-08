#ifndef FORMHISTOGRAM_H
#define FORMHISTOGRAM_H

#include <QWidget>

namespace Ui {
class FormHistogram;
}

class FormHistogram : public QWidget
{
    Q_OBJECT

public:
    explicit FormHistogram(const QList<QHash<uchar, int>> &channels, QWidget *parent = 0);
    ~FormHistogram();

private:
    Ui::FormHistogram *ui;
    void drawHistogram(QList<QHash<uchar, int>> channels);
};

#endif // FORMHISTOGRAM_H
