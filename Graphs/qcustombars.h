#ifndef QCUSTOMBARS_H
#define QCUSTOMBARS_H
#include "Graphs/qcustomplot.h"
#include <QWidget>


class QCustomBars
{
public:
    typedef enum{
        BARS_XTIME   = 0,//on x axis time is displayed
        BARS_XOTHERS = 1,//on x axis other double types are displayed
        BARS_PLUSTOT = 2,//time on x axis, bars are stacked on each others
    }enumChoiceBar;

    typedef enum{
        EN_LSNONE =0,
        EN_LSLINE =1,
    }enLineStyle;

private:
    QVector<double> ranges;
    QVector<double> columns;
    double max, min, range = 0, dBin;
    QList<QCPBars*> listBars;
    QCustomPlot *plot;

public:
    void setStyle(int iStyleNumber);
    void giveMeUi (QCustomPlot *cplot);
    void barsXTime  (QVector<QTime> vec_time);
    void barsXTimeV2(QVector<double> vec_timeMs);
    void barsXAxisIntervals(QVector<double> vec_elements, int iNumBin);
    void yDispersion(QCustomPlot *cplot, QVector<QVector<double>> vec_vecDoub);
    void barsXTimeV3(enumChoiceBar choice, int iNumBin, QVector<double> vec_doubles);
    void barsXTimeV4(int iNumBin, QVector<double> vec_douberrors, QVector<double> vec_doubtots);
    void graphCustom00 (QCustomPlot *cplot, QVector<double> vec_timeMs, QVector<double> vec_doubVals);
    void graphLines (QCustomPlot *cplot, QPen pencolor, QVector<double> vec_xAxis, QVector<double> vec_yAxis);
    void barsGraph(enumChoiceBar choice,int iNumBin, QVector<double> vec_doubs, QVector<double> vec_douberrors={});
    void scatterGraph (QVector<double> vec_timeMs, QVector<double> vec_doubVals, QPen penColor,
                       enLineStyle lineStyle = EN_LSNONE);

private:
    QCustomBars();
    ~QCustomBars();
    void styleAxes ();
    void setBackgroundGradient();
    void inizializeColumns(int iNumBin);
    QString changeToHour(QString str_toChange);
    double findMax (QVector<double> vectorOfDouble);
    double findMin (QVector<double> vectorOfDouble);
    void makeConfrontRanges(enumChoiceBar choice, int iNumBin);
    void addBarsElements(QVector<double> vec_doub, int iNumBin);
    QVector<QString> getLabels(enumChoiceBar choice, int iNumBin);

    //@todo application styling with css file
    //@todo fix mouse on over display of coordinates, hour shown is behind one hour
    //@todo fix bug clear button not clearing axes
    //@todo fix barsgraph invert the to vectors ( errors below)
    //@todo fix bug where on the same widgt you make first a scatterplot, than a barscharts
    //@todo add legenda and set graph names accordingly, to do so: plot->graph->setname
    //@todo make color change for multiple calls of the same function in the same wdg (hint:lighter)
    //@todo fix bug legend max height
    //@todo make combobox choice better:
    //m_ui->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
};

#endif // QCUSTOMBARS_H
