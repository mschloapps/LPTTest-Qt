#ifndef CHARTTIMER_H
#define CHARTTIMER_H
#include <QtWidgets>
#include <QtCharts>
#include <QtCore>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

// Create inherited class based on QGraphicsView
class ChartTimer : public QGraphicsView {
    //Q_OBJECT

public:
    ChartTimer(QWidget *parent = nullptr);

protected:
    void timerEvent(QTimerEvent *) override;

private:
    int timerId;
    int trace_id;
    int num_traces;
    QChart *m_chart;
    QValueAxis *axisY;
    QGraphicsSimpleTextItem *timestamp;
    QMap<qint32, QMap<QString, QList<qreal>>> plot_data;
};


#endif // CHARTTIMER_H
