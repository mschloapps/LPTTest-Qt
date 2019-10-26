
#include "charttimer.h"
#include <QtWidgets>
#include <QtCharts>
#include <QtSql>
#include <QtDebug>

// Function to get MySQL Data
QMap<qint32, QMap<QString, QList<qreal>>> getData() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("lpttest");
    db.setUserName("user");
    db.setPassword("password");
    bool ok = db.open();

    // Query database
    QSqlQuery query;
    query.exec("SELECT trace_id, HEX(trace_data) as `data`, trace_time FROM test");

    QMap<qint32, QMap<QString, QList<qreal>>> data;

    // Loop through query result, decode and convert data, and store in a QMap
    while (query.next()) {
        QMap<QString, QList<qreal>> blob;
        QString bytes_str = query.value(1).toString();
        QList<double> blobdata;
        for (qint32 i=0; i<bytes_str.length(); i+=8) {
            QString sbyte = bytes_str.mid(i, 8);
            bool status = false;
            qint32 byte = sbyte.toUInt(&status, 16);
            qreal datapoint = (qreal)byte/1000;
            blobdata.append(datapoint);
        }
        blob.insert(query.value(2).toString(), blobdata);
        data.insert(query.value(0).toInt(), blob);
    }
    return (data);
}

// Create instance of ChartTimer Class
ChartTimer::ChartTimer(QWidget *parent) : QGraphicsView(new QGraphicsScene, parent) {

    num_traces = 50;
    trace_id = 1;

    // Get Plot Data
    plot_data = getData();

    // Initialize QChart, Axis, and Timestamp
    m_chart = new QChart;
    m_chart->setMinimumSize(800, 600);
    m_chart->legend()->hide();

    axisY = new QValueAxis;
    axisY->setRange(-130.0, -30.0);
    axisY->setTickCount(10);
    axisY->setTickType(QValueAxis::TicksFixed);
    axisY->setTickInterval(10.0);
    axisY->setTitleText("dBm");
    m_chart->addAxis(axisY, Qt::AlignLeft);
    scene()->addItem(m_chart);

    timestamp = new QGraphicsSimpleTextItem(m_chart);
    timestamp->setPos(m_chart->size().width()/2-50, m_chart->size().height()-150);

    // Start 1 second update timer
    timerId = startTimer(1000);

}

// Function to handle timer update interval
void ChartTimer::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timerId) {

        // Clear chart
        m_chart->removeAllSeries();

        // Get next blob, store in chart series, and add to chart
        QLineSeries *series = new QLineSeries();
        QMap<QString, QList<qreal>> blob = plot_data[trace_id];
        QString key = blob.firstKey();
        QList<qreal> datapoints = blob[key];
        for (qint32 i=0; i<datapoints.size(); i++) {
            series->append(i, datapoints[i]);
        }
        m_chart->addSeries(series);

        // Re-attach axis that was removed with series before
        series->attachAxis(axisY);

        // Set timestamp to correct format, and set the text
        QDateTime tm = QDateTime::fromString(key, "yyyy-MM-ddTHH:mm:ss.zzz");
        QString tm_string = tm.toLocalTime().toString("MM/dd/yyyy HH:mm:ss AP");
        timestamp->setText(tm_string);

        // Update the chart
        scene()->update();


        // Increment trace_id and reset if necessary for continuous loop
        trace_id++;
        if (trace_id > num_traces) {
            trace_id = 1;
        }
    }
}
