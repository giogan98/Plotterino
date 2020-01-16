#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Completion/getcomplsuggestions.h"
#include "Graphs/qcustomplot.h"
#include "Graphs/qcustombars.h"
#include "DBmanaging/IScanLogDatabase.h"
#include "Processing/IProcessGen.h"
#include "Processing/IProcessMC.h"
#include "Processing/IProcessLog.h"

//---------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setMinimumSize(800,600);
    textItem = new QCPItemText(ui->wdg_graph1);
    connect(ui->wdg_graph1, &QCustomPlot::mouseMove, this, &MainWindow::onMouseMove);
    QString str_stylesheet,str_stylefile;
    str_stylefile="C:/Users/giorg/Documents/Qt_documenti/DBGraphsV1/Stylesheet/Aqua.qss";
    QFile file(str_stylefile);

    if (!file.open(QFile::ReadOnly | QFile::Text)){
        qDebug()<<"QSS file is not opened";
        return;
    }
    QTextStream in(&file);
    str_stylesheet=in.readAll();
    setStyleSheet(str_stylesheet);
}
//---------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
}
//---------------------------------------------------------------------------------
void MainWindow::onMouseMove(QMouseEvent *event)
{
    QCustomPlot* customPlot = qobject_cast<QCustomPlot*>(sender());
    //    double x = customPlot->xAxis->pixelToCoord(event->pos().x());
    double y = customPlot->yAxis->pixelToCoord(event->pos().y());
    //    QString strx= QString("%l").arg(x);
    //    strx = custombars.changeToHour(strx);
    //    textItem->setText(strx);
    //    textItem->position->setCoords(QPointF(x, y));
    //    textItem->setFont(QFont(font().family(), 8));
    //    customPlot->replot();

    const double dTimeS = customPlot->xAxis->pixelToCoord(event->pos().x());
    int ss = static_cast<int>(dTimeS);
    int mm = ( ss / 60    ) % 60;
    int hh = ( ss / 3660  ) % 24;
    int ms = static_cast<int>((dTimeS *1000.0)) - ss*1000;
    ss = ss % 60;
    //customPlot->setToolTip(QString::asprintf("%02d:%02d:%02d.%03d",hh,mm,ss,ms));

    textItem->setText(QString::asprintf("%02d:%02d:%02d.%03d",hh,mm,ss,ms));
    textItem->position->setCoords(QPointF(dTimeS, y));
    textItem->setFont(QFont(font().family(), 8));
    customPlot->replot();
}
//---------------------------------------------------------------------------------
void MainWindow::on_pbn_database_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
//---------------------------------------------------------------------------------
void MainWindow::on_pbn_graphs_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    QStringList strl_suggestions;
    QVector<QVariant> qv_varSugg;
    qv_varSugg = iScanLogDatabase.getFiltered("DISTINCT Field","TableTest","%","%");
    for ( int ii = 0; ii < qv_varSugg.length(); ii++){
        strl_suggestions.append(qv_varSugg[ii].toString());
    }
    getSuggestions.getCompleted(ui->lined_field, strl_suggestions);
}
//---------------------------------------------------------------------------------
void MainWindow::on_pbn_plot_clicked()
{
    QCustomPlot *cplot;
    QString str_widget = ui->cbox_selectWidget->currentText();
    QString str_graphType = ui->cb_graphType->currentText();
    QString str_field = ui->lined_field->text();
    cplot = ui->wdg_graph1;
    if (str_widget == "Widget 2"){
        cplot = ui->wdg_graph2;
    }

    if (str_graphType == "Scatter"){
        std::string str_field = ui->lined_field->text().toStdString();
        QVector<QVariant> vec_tempMs = iScanLogDatabase.getFiltered("Time_ms","TableTest","%",str_field);
        QVector<QVariant> vec_tempValue = iScanLogDatabase.getFiltered("Value","TableTest","%",str_field);

        QVector<double> vec_doubMs,vec_doubVals;
        QVector<double> vec_doubValsOk, vec_doubMsOk,vec_doubValsEr, vec_doubMsEr;
        for (int ii = 0; ii < vec_tempMs.length(); ii++){
            vec_doubMs.append(vec_tempMs[ii].toDouble());
            vec_doubVals.append(vec_tempValue[ii].toDouble());
        }
        for (int ii = 0; ii < vec_doubMs.length(); ii++){
            if (static_cast<int>(vec_doubVals[ii])%100==0){
                vec_doubMsEr.append(vec_doubMs[ii]);
                vec_doubValsEr.append(vec_doubVals[ii]);
            }else{
                vec_doubMsOk.append(vec_doubMs[ii]);
                vec_doubValsOk.append(vec_doubVals[ii]);
            }
        }
        cbars.giveMeUi(cplot);
        cbars.scatterGraph(vec_doubMsEr,vec_doubValsEr,QPen(Qt::red), cbars.EN_LSNONE);
        cbars.scatterGraph(vec_doubMsOk,vec_doubValsOk,QPen(Qt::blue), cbars.EN_LSNONE);
    }
    else if (str_graphType == "Bars_Stacked"){
        std::string str_field = ui->lined_field->text().toStdString();
        QVector<QVariant> vec_tempMs = iScanLogDatabase.getFiltered("Time_ms","TableTest","%",str_field);
        QVector<QVariant> vec_tempValue = iScanLogDatabase.getFiltered("Value","TableTest","%",str_field);

        QVector<double> vec_doubMs,vec_doubVals;
        QVector<double> vec_doubMsEr, vec_doubMsOk;
        for (int ii = 0; ii < vec_tempMs.length(); ii++){
            vec_doubMs.append(vec_tempMs[ii].toDouble());
            vec_doubVals.append(vec_tempValue[ii].toDouble());
        }
        for (int ii = 0; ii < vec_doubMs.length(); ii++){
            if (static_cast<int>(vec_doubVals[ii])%100==0){
                vec_doubMsEr.append(vec_doubMs[ii]);
                //vec_doubValsEr.append(vec_doubVals[ii]);
            }else{
                vec_doubMsOk.append(vec_doubMs[ii]);
            }
        }
        cbars.giveMeUi(cplot);
        cbars.barsGraph(QCustomBars::BARS_PLUSTOT,24,vec_doubMsOk,vec_doubMsEr);
    }
    else if ( str_graphType == "Lines"){
        std::string str_field = ui->lined_field->text().toStdString();
        QVector<QVariant> vec_tempMs = iScanLogDatabase.getFiltered("Time_ms","TableTest","%",str_field);
        QVector<QVariant> vec_tempValue = iScanLogDatabase.getFiltered("Value","TableTest","%",str_field);

        QVector<double> vec_doubMs,vec_doubVals;
        for (int ii = 0; ii < vec_tempMs.length(); ii++){
            vec_doubMs.append(vec_tempMs[ii].toDouble());
            vec_doubVals.append(vec_tempValue[ii].toDouble());
        }
        cbars.giveMeUi(cplot);
        cbars.scatterGraph(vec_doubMs,vec_doubVals,QPen(Qt::green), cbars.EN_LSLINE);
    }
    else if (str_graphType == "Other_bars"){
        //@todo
    }
}
//---------------------------------------------------------------------------------
void MainWindow::on_pbn_createDb_clicked()
{
    QFile filedb("./Alfa.db");
    filedb.remove();
    iScanLogDatabase.createOpenDatabase("./Alfa.db");
    QString str_fileName = "C:/Users/giorg/Documents/Doc_tirocinio/TableLog_2019_10_11.txt";
    //    QString str_fileName = QFileDialog::getOpenFileName(this, "Open log file");
    QFile file(str_fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)){
        qDebug()<<"Log file is not opened";
    }
    else{
        QElapsedTimer t;
        qint64 lProcessed=0;
        qint64 lFileSize = file.size();
        t.start();
        ui->pbn_graphs->setEnabled(false);
        QTextStream inStream(&file);
        while( ! inStream.atEnd() )
        {
            std::string strLineLog = inStream.readLine().toUtf8().constData();
            iProcessLog.processLogLine(strLineLog);
            lProcessed += strLineLog.length();
            if ( lProcessed*100>(ui->progressBar->value()+9)*lFileSize){
                ui->progressBar->setValue(static_cast<int>(lProcessed*100)/lFileSize);
            }
        }

        iScanLogDatabase.executeLoad();
        ui->progressBar->setValue(100);
        ui->pbn_graphs->setEnabled(true);
        qDebug() << "The slow operation took" << t.elapsed() << "milliseconds";
    }
}
//---------------------------------------------------------------------------------
void MainWindow::on_pbn_clear_clicked()
{
    ui->lined_field->clear();

    ui->wdg_graph1->clearPlottables();
    ui->wdg_graph2->clearPlottables();

    ui->wdg_graph1->clearGraphs();
    ui->wdg_graph2->clearGraphs();

    ui->wdg_graph1->legend->setVisible(false);
    ui->wdg_graph2->legend->setVisible(false);

    ui->wdg_graph1->replot();
    ui->wdg_graph2->replot();
}
//---------------------------------------------------------------------------------
