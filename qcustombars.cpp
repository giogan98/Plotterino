#include "qcustombars.h"
#include "mainwindow.h"


QCustomBars::QCustomBars()
{

}

//---------------------------------------------------------------------------------
/**
 * @brief QCustomBars::QCustomBars
 * @brief DEPRECATED FUNCTION, use barsXTimeV3 instead
 * @param vec_elements vector containg the elements to display
 * @param iNumBin number of columns you want to have
 */
void QCustomBars::barsXAxisIntervals(QVector<double> vec_elements, int iNumBin)
{

    inizializeColumns(iNumBin);

    //Find min and max values
    max = findMax(vec_elements);
    min = findMin(vec_elements);

    range = max - min;
    dBin = range / iNumBin;

    ranges.clear();
    //Calculate the various intervals of values + save them in the ranges vector
    for ( int ii = 0; ii<iNumBin; ii++ )
    {
        double element = min + ii * dBin;
        ranges.append(element);
    }
    //Find for each number the range in wich it belongs
    for ( int ii=0; ii<vec_elements.length(); ii++)
    {
        int index = 0;
        for (int kk = 0; kk < ranges.length(); kk++) {
            if (vec_elements[ii]>=ranges[kk] && vec_elements[ii]<=max){
                index = kk;
            }
            else
            {
                break;
            }
        }
        double appo = columns[index];
        appo += 1;
        columns[index] = appo;
    }

    // prepare x axis with labels
    QVector<double> ticks;
    QVector<QString> labels;
    QVector<QString> labProvvisory;
    for (int ii=0; ii<ranges.length(); ii++)
    {
        QString str_converter = QString::number(ranges[ii]);
        labProvvisory.append(str_converter);
    }

    for (int ii = 0; ii<iNumBin; ii++){
        ticks.append(ii+1);
        QString lab_provv;
        if ( ii != iNumBin-1){
            lab_provv = labProvvisory[ii] + "\n-\n" + labProvvisory[ii+1] ;
        }
        else{
            lab_provv = labProvvisory[ii] + "\n-\n" + QString::number(max);
        }
        labels.append(lab_provv);
    }

    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    plot->xAxis->setTicker(textTicker);
    plot->yAxis->setLabel("N° of occurrences");


    listBars.clear();
    for ( int ii=0; ii<iNumBin; ii++){
        QCPBars* bar = new QCPBars(plot->xAxis, plot->yAxis);
        listBars.append(bar);
        //        listBars[ii] ->setPen(QPen(QColor(0, 168, 140).lighter(130)));
        //        listBars[ii] ->setBrush(QColor(0, 168, 140));
    }

    // Add data:

    for ( int ii = 0; ii<iNumBin; ii++){
        listBars[ii] ->setData(ticks, columns);
    }

    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    plot->show();
    plot->replot();

}
//---------------------------------------------------------------------------------
/**
 * @brief QCustomBars::barsXAxisTime
 * @warning DEPRECATED FUNCTION, use barsXTimeV3 instead
 * @param vec_time
 */
void QCustomBars::barsXTime(QVector<QTime> vec_time)
{
    int iNumBin = 24;
    inizializeColumns(iNumBin);

    if (vec_time.length()<=0){
        qDebug() << "[!] ERROR: length of vec_time is not valid";
        return;
    }

    for (int ii = 0; ii<vec_time.length(); ii++)
    {
        QString t = vec_time[ii].toString("hh");
        int nt = t.toInt();
        double appo =columns[nt];
        appo++;
        columns[nt]=appo;
    }

    // create empty bar chart objects + set names and colors:
    listBars.clear(); //in case you want to use this function multiple times in the same window
    for ( int ii=0; ii<iNumBin; ii++){
        QCPBars* bar = new QCPBars(plot->xAxis, plot->yAxis);
        listBars.append(bar);
        //        listBars[ii] ->setPen(QPen(QColor(0, 168, 140).lighter(130)));
        //        listBars[ii] ->setBrush(QColor(0, 168, 140));
    }

    // prepare x axis with labels
    QVector<double> ticks;
    QVector<QString> labels;
    QVector<QString> labProvvisory;
    for (int ii=0; ii<iNumBin; ii++)
    {
        QString str_converter = QString::number(ii);
        labProvvisory.append(str_converter);
    }

    for (int ii = 0; ii<iNumBin; ii++){
        ticks.append(ii+1);
        QString lab_provv;
        if ( ii != iNumBin-1){
            lab_provv = labProvvisory[ii] + "\n-\n" + labProvvisory[ii+1] ;
        }
        else{
            lab_provv = labProvvisory[ii] + "\n-\n" + QString::number(24);
        }
        labels.append(lab_provv);
    }

    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    plot->xAxis->setTicker(textTicker);
    plot->yAxis->setLabel("N° of occurrences");



    // Add data:

    for ( int ii = 0; ii<iNumBin; ii++){
        listBars[ii] ->setData(ticks, columns);
    }
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom );
    plot->rescaleAxes(true);
    plot->show();
    plot->replot();

}

//---------------------------------------------------------------------------------
/**
 * @brief QCustomBars::barsXTimeV2
 * @warning DEPRECATED FUNCTION, use barsXTimeV3 instead
 * @param vec_timeMs
 */
void QCustomBars::barsXTimeV2(QVector<double> vec_timeMs)
{
    int iNumBin = 24;
    int t = 0;
    double t_hour = 0;

    inizializeColumns(iNumBin);

    if (vec_timeMs.length()<=0){
        qDebug() << "[!] ERROR: length of vec_timeMs is not valid";
        return;
    }

    for (int ii = 0; ii<vec_timeMs.length(); ii++)
    {
        t_hour = vec_timeMs[ii]/(1000*3600);
        t =int (t_hour);
        double appo =columns[t];
        appo++;
        columns[t]=appo;
    }

    // create empty bar chart objects + set names and colors:
    listBars.clear(); //in case you want to use this function multiple times in the same window

    for ( int ii=0; ii<iNumBin; ii++){
        QCPBars* bar = new QCPBars(plot->xAxis, plot->yAxis);
        listBars.append(bar);
    }

    // prepare x axis with labels
    QVector<double> ticks;
    QVector<QString> labels;
    QVector<QString> vec_hoursProvvisory;

    for (int ii=0; ii<iNumBin; ii++)
    {
        QString str_converter = QString::number(ii);
        vec_hoursProvvisory.append(str_converter);
    }

    for (int ii = 0; ii<iNumBin; ii++){
        ticks.append(ii+1);
        QString lab_provv;
        if ( ii != iNumBin-1){
            lab_provv = vec_hoursProvvisory[ii] + "\n-\n" + vec_hoursProvvisory[ii+1] ;
        }
        else{
            lab_provv = vec_hoursProvvisory[ii] + "\n-\n" + QString::number(24);
        }
        labels.append(lab_provv);
    }

    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    plot->xAxis->setTicker(textTicker);
    plot->yAxis->setLabel("N° of occurrences");



    // Add data:

    for ( int ii = 0; ii<iNumBin; ii++){
        listBars[ii] ->setData(ticks, columns);
    }
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom );
    plot->rescaleAxes(true);
    plot->show();
    plot->replot();

}

//---------------------------------------------------------------------------------

/**
 * @brief QCustomBars::barsXTimeV3
 * @brief Plots bars based on the number of occurences
 * @param choice to select xaxis time or other doubles on x axis
 * @param iNumBin number of columns
 * @param vec_doubles vector wich contains the values to display
 */
void QCustomBars::barsXTimeV3(enumChoiceBar choice, int iNumBin, QVector<double> vec_doubles)
{
    if ( iNumBin <=0){
        qDebug() << "[!] ERROR: iNumBin not valid";
        return;
    }
    if (vec_doubles.length()<=0){
        qDebug() << "[!] ERROR: length of vec_doubles is not valid";
        return;
    }

    switch (choice) {
    case BARS_XTIME:
        max =24;
        min = 0;
        break;
    case BARS_XOTHERS:
        max = findMax(vec_doubles);
        min = findMin(vec_doubles);
        break;
    case BARS_PLUSTOT:
        qDebug()<<"[!]Not yet implemented";
        break;
    default:
        qDebug()<<"[!]ERROR: you've added an element to enumChoice and not implemented it correctly ";
        break;
    }

    QCPBars* bar = new QCPBars(plot->xAxis, plot->yAxis);
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    QVector<double> ticks;
    QVector<QString> labels;


    makeConfrontRanges(choice,iNumBin);

    addBarsElements(vec_doubles,iNumBin);

    labels = getLabels(choice,iNumBin);
    for (int ii = 0; ii<iNumBin; ii++){
        ticks.append(ii+1);
    }

    textTicker->addTicks(ticks, labels);
    plot->xAxis->setTicker(textTicker);
    plot->yAxis->setLabel("N° of occurrences");

    bar->setData(ticks,columns);
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom );
    plot->rescaleAxes(true);
    plot->show();
    plot->replot();

}
//---------------------------------------------------------------------------------
/**
 * @brief QCustomBars::barsXTimeV4
 * @brief v3 plus added possibility to display % from totals with stacking of bars
 * @brief to use this function you first have to call the QCustomBars::giveMeUi function
 * @param choice
 * @param iNumBin number of columns you want to have
 * @param vec_doubles vector containing the values
 */
void QCustomBars::barsXTimeV4(int iNumBin, QVector<double> vec_douberrors, QVector<double> vec_doubtots)
{
    QCPBars *bars = new QCPBars(plot->xAxis, plot->yAxis);
    QCPBars *bars_on_top = new QCPBars(plot->xAxis, plot->yAxis);
    bars_on_top->moveAbove(bars);
    QVector<double> ticks, columnsTot;
    QVector<QString> labels, vec_strTemp;

    max =24;
    min = 0;

    if ( iNumBin <=0){
        qDebug() << "[!] ERROR: iNumBin not valid";
        return;
    }
    if (vec_douberrors.length()<=0){
        qDebug() << "[!] ERROR: length of vec_doubles is not valid";
        return;
    }
    if (vec_doubtots.length()<=0){
        qDebug() << "[!] ERROR: length of vec_doubtots is not valid";
        return;
    }

    makeConfrontRanges(BARS_PLUSTOT,iNumBin);
    addBarsElements(vec_douberrors,iNumBin);

    for (int ii=0; ii<=iNumBin; ii++)
    {
        double d_numb = (double)ii*(max)/iNumBin;
        QString str_converter = QString::number(d_numb,10,2);
        vec_strTemp.append(str_converter);
    }

    for (int ii = 0; ii<iNumBin; ii++){
        ticks.append(ii+1);
        QString str_tempName;
        str_tempName = changeToHour(vec_strTemp[ii]) + "\n-\n" + changeToHour(vec_strTemp[ii+1]);
        labels.append(str_tempName);
    }

    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    plot->xAxis->setTicker(textTicker);
    plot->yAxis->setLabel("N° of occurrences");

    for (int ii = 0; ii<iNumBin; ii++){
        columnsTot.append(0);
    }

    for (int ii = 0; ii<vec_doubtots.length(); ii++)
    {
        int index = 0;
        for (int kk = 0; kk<iNumBin-1; kk++){

            if (vec_doubtots[ii]>ranges[kk]){
                index++;
            }
            else{
                break;
            }
        }
        double appo =columnsTot[index];
        appo++;
        columnsTot[index]=appo;
    }

    for ( int ii = 0; ii< columnsTot.length(); ii++){
        columnsTot[ii] = columnsTot[ii] - columns[ii];
        if (columnsTot[ii]<0){
            columnsTot[ii]=0;
        }
    }
    bars->setData(ticks, columns);
    bars_on_top->setData(ticks,columnsTot);

    bars_on_top->setBrush(QColor(0, 0, 255));
    bars->setBrush(QColor(255,0,0));


    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom );
    plot->rescaleAxes(true);
    plot->show();
    plot->replot();

}
//---------------------------------------------------------------------------------
/**
 * @brief QCustomBars::barsGraph
 * @brief function v3 + v4 combined together, give the functions two vectors and it stacks them by time or range values
 * @param choice
 * @param iNumBin
 * @param vec_doubs
 * @param vec_douberrors
 */
void QCustomBars::barsGraph(enumChoiceBar choice, int iNumBin, QVector<double> vec_doubs, QVector<double> vec_douberrors)
{
    QCPBars *bars = new QCPBars(plot->xAxis, plot->yAxis);
    QCPBars *bars_on_top = new QCPBars(plot->xAxis, plot->yAxis);
    bars_on_top->moveAbove(bars); //errors are presented below
    QVector<double> vec_db_ticks, vec_db_columns, vec_db_columns_errors;
    QVector<QString> vec_str_labels, vec_str_temp;

    if ( iNumBin <=0){
        qDebug() << "[!] ERROR: iNumBin not valid";
        return;
    }
    if (vec_douberrors.length()<0){
        qDebug() << "[!] ERROR: length of vec_doubles is not valid";
        return;
    }
    if (vec_doubs.length()<0){
        qDebug() << "[!] ERROR: length of vec_doubtots is not valid";
        return;
    }

    switch (choice) {
    case BARS_XOTHERS:
        max = findMax(vec_doubs);
        min = findMin(vec_doubs);
        break;
    case BARS_XTIME:
    case BARS_PLUSTOT:
        max =24;
        min = 0;
        break;
    default:
        qDebug()<<"[!]ERROR: you've added an element to enumChoice and not implemented it correctly ";
        break;
    }

    makeConfrontRanges(choice,iNumBin);
    //    inizializeColumns(iNumBin);
    //    addBarsElements(vec_doubs,iNumBin);

    for (int ii=0; ii<=iNumBin; ii++)
    {
        double d_numb = (double)ii*(max)/iNumBin;
        QString str_converter = QString::number(d_numb,10,2);
        vec_str_temp.append(str_converter);
    }

    for (int ii = 0; ii<iNumBin; ii++)
    {
        vec_db_ticks.append(ii+1);
        QString str_tempName;
        str_tempName = changeToHour(vec_str_temp[ii]) + "\n-\n" + changeToHour(vec_str_temp[ii+1]);
        vec_str_labels.append(str_tempName);
    }

    if (choice == BARS_XTIME || choice == BARS_PLUSTOT){
        QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
        textTicker->addTicks(vec_db_ticks, vec_str_labels);
        plot->xAxis->setTicker(textTicker);
    }

    plot->yAxis->setLabel("N° of occurrences");

    for (int ii = 0; ii<iNumBin; ii++){
        vec_db_columns.append(0);
        vec_db_columns_errors.append(0);
    }

    for (int ii = 0; ii<vec_doubs.length(); ii++)
    {
        int index = 0;
        for (int kk = 0; kk<iNumBin-1; kk++){

            if (vec_doubs[ii]>ranges[kk]){
                index++;
            }
            else{
                break;
            }
        }
        double appo =vec_db_columns[index];
        appo++;
        vec_db_columns[index]=appo;
    }
    for (int ii = 0; ii<vec_douberrors.length(); ii++)
    {
        int index = 0;
        for (int kk = 0; kk<iNumBin-1; kk++){

            if (vec_douberrors[ii]>ranges[kk]){
                index++;
            }
            else{
                break;
            }
        }
        double appo =vec_db_columns_errors[index];
        appo++;
        vec_db_columns_errors[index]=appo;
    }

    bars->setData(vec_db_ticks, columns);
    bars_on_top->setData(vec_db_ticks,vec_db_columns);

    bars_on_top->setBrush(QColor(0, 0, 255));
    bars->setBrush(QColor(255,0,0));


    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom );
    plot->rescaleAxes(true);
    plot->show();
    plot->replot();
    columns.clear();
}
//---------------------------------------------------------------------------------
/**
 * @brief QCustomBars::graphCustom00
 * @brief plot values on y, time on x, if value % 100 == 0 the point is red
 * @warning DEPRECATED FUNCTION, use scatterGraph instead with multiplecalls on the same qcustomplot widget
 * @param cplot to point to the qcustomplot you want to plot to
 * @param vec_timeMs
 * @param vec_doubVals
 */
void QCustomBars::graphCustom00(QCustomPlot *cplot, QVector<double> vec_timeMs, QVector<double> vec_doubVals)
{
    QVector<double>vec_timeS;
    vec_timeS.clear();
    cplot->addGraph();
    cplot->addGraph();

    if ( vec_timeMs.length() != vec_doubVals.length()){
        qDebug() << " vec_timeMs and vec_doubVals have diferent lengths";
        return;
    }
    for ( int ii=0; ii<vec_timeMs.length(); ii++){
        vec_timeS.append(vec_timeMs[ii]/1000);
    }

    for (int ii=0; ii < vec_timeS.length(); ii++){
        if ((int)vec_doubVals[ii]%100==0){
            cplot->graph(1)->addData(vec_timeS[ii],vec_doubVals[ii]);
        }else{
            cplot->graph(0)->addData(vec_timeS[ii],vec_doubVals[ii]);
        }
    }
    cplot->graph(0)->setPen(QPen(Qt::blue));
    cplot->graph(1)->setPen(QPen(Qt::red));
    cplot->graph(0)->setLineStyle(QCPGraph::lsNone);
    cplot->graph(1)->setLineStyle(QCPGraph::lsNone);


    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeSpec(Qt::UTC);
    dateTicker->setDateTimeFormat("hh:\nmm:ss\nzzz");
    cplot->xAxis->setTicker(dateTicker);
    cplot->xAxis->setSubTicks(false);

    cplot->xAxis->setLabel("x");
    cplot->yAxis->setLabel("y");
    cplot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
    cplot->graph(1)->setScatterStyle(QCPScatterStyle::ssCircle);



    cplot->rescaleAxes(true);
    cplot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    cplot->setInteraction(QCP::iSelectPlottables, true);
    cplot->replot();

}
//---------------------------------------------------------------------------------
/**
 * @brief QCustomBars::scatterGraph
 * @brief plot values on y, time on x, QPen sets the color of the graph
 * @brief the result is a scatter style diagram
 * @param vec_timeMs
 * @param vec_doubVals
 * @param penColor
 */
void QCustomBars::scatterGraph(QVector<double> vec_timeMs, QVector<double> vec_doubVals, QPen penColor, enLineStyle lineStyle )
{
    plot->legend->setVisible(true);
    plot->legend->setFont(QFont("Helvetica", 9));
    plot->legend->setRowSpacing(-3);

    QVector<double>vec_timeS;
    vec_timeS.clear();
    plot->addGraph();

    if ( vec_timeMs.length() != vec_doubVals.length()){
        qDebug() << " vec_timeMs and vec_doubVals have diferent lengths";
        return;
    }
    for ( int ii=0; ii<vec_timeMs.length(); ii++){
        vec_timeS.append(vec_timeMs[ii]/1000);
    }
    plot->graph()->setData(vec_timeS,vec_doubVals); //you have to use time in seconds, not ms
    plot->graph()->setPen(penColor);

    switch (lineStyle){
    case EN_LSNONE:
        plot->graph()->setLineStyle(QCPGraph::lsNone);
        break;
    case EN_LSLINE:
        plot->graph()->setLineStyle(QCPGraph::lsLine);
        break;
    default:
        break;
    }

    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeSpec(Qt::UTC);
    dateTicker->setDateTimeFormat("hh:\nmm:ss\nzzz");
    plot->xAxis->setTicker(dateTicker);
    plot->xAxis->setSubTicks(false);

    plot->xAxis->setLabel("Time");
    plot->yAxis->setLabel("Value");
    QCPScatterStyle style = QCPScatterStyle::ssDisc;
    style.setSize(3);
    plot->graph()->setScatterStyle(style);

    plot->rescaleAxes(true);
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    plot->setInteraction(QCP::iSelectPlottables, true);
    plot->replot();
}
//---------------------------------------------------------------------------------
/**
* @brief QCustomBars::graphLines
* @brief make line graph, multiple calls with the same givemeui make multiple graphs
* @warning deprecated function, use instead scattergraph with argument linestyle = EN_LSLINE
* @param cplot
* @param vec_xAxis
* @param vec_yAxis
*/
void QCustomBars::graphLines(QCustomPlot *cplot, QPen penColor, QVector<double> vec_xAxis, QVector<double> vec_yAxis)
{
    QVector<double>vec_timeS;
    if ( vec_xAxis.length() != vec_yAxis.length()){
        qDebug() << " vec_timeMs and vec_doubVals have diferent lengths";
        return;
    }
    for ( int ii=0; ii<vec_xAxis.length(); ii++){
        vec_timeS.append(vec_xAxis[ii]/1000);
    }
    // create graph and assign data to it:
    cplot->addGraph();

    cplot->graph()->setData(vec_timeS, vec_yAxis);
    plot->graph()->setPen(penColor);
    plot->graph()->setLineStyle(QCPGraph::lsLine);
    //    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    //    dateTicker->setDateTimeSpec(Qt::UTC);
    //    dateTicker->setDateTimeFormat("hh:\nmm:ss\nzzz");
    //    plot->xAxis->setTicker(dateTicker);
    //    plot->xAxis->setSubTicks(false);

    plot->xAxis->setLabel("x");
    plot->yAxis->setLabel("y");
    QCPScatterStyle style = QCPScatterStyle::ssDisc;
    style.setSize(3);
    plot->graph()->setScatterStyle(style);

    plot->rescaleAxes(true);
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    plot->setInteraction(QCP::iSelectPlottables, true);
    plot->replot();
}

//---------------------------------------------------------------------------------
/**
 * @brief QCustomBars::setStyle
 * @brief set the style for the selected graph
 * @warning function not tested
 * @param iStyleNumber
 */
void QCustomBars::setStyle(int iStyleNumber)
{
    switch(iStyleNumber){
    case 1:
        setBackgroundGradient();
        styleAxes();
        break;
    default:
        //do something
        break;
    }
}
//---------------------------------------------------------------------------------
/**
 * @brief QCustomBars::yDispersion
 * @brief create scatter style plot, depending on the length of the qvector you have multiple days/hours/...
 * @warning function not tested
 * @param cplot
 * @param vec_vecDoub
 */
void QCustomBars::yDispersion(QCustomPlot *cplot, QVector<QVector<double> > vec_vecDoub)
{
    cplot->addGraph();
    QVector<double> vec_dbX, vec_dbY;

    for (int ii = 0; ii < vec_vecDoub.length(); ii++){
        vec_dbX.clear();
        vec_dbX.append((double)ii+1);
        for (int kk = 0; kk < vec_vecDoub[ii].length(); kk++){
            vec_dbY.clear();

            vec_dbY.append(vec_vecDoub[ii][kk]);
            cplot->graph()->addData(vec_dbX,vec_dbY);
        }

    }

    cplot->xAxis->setLabel("x: Days");
    cplot->yAxis->setLabel("y: Values");
    cplot->graph()->setLineStyle(QCPGraph::lsNone);
    cplot->graph()->setScatterStyle(QCPScatterStyle::ssCircle);
    cplot->rescaleAxes(true);
    cplot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    cplot->setInteraction(QCP::iSelectPlottables, true);
    cplot->replot();

}

//---------------------------------------------------------------------------------
/**
 * @brief QCustomBars::giveMeUi
 * @brief used to set the QCustomPlot to wich the functions plot to
 * @param cplot
 */
void QCustomBars::giveMeUi(QCustomPlot *cplot)
{
    plot = cplot;
}

//---------------------------------------------------------------------------------
/**
 * @brief QCustomBars::inizializeColumns
 * @brief clears the vector of columns, initialize it with the number given to the function
 * @param numberOfRanges
 */
void QCustomBars::inizializeColumns(int numberOfRanges)
{
    //Initialize number count of each column to 0
    columns.clear();
    for (int ii = 0; ii<numberOfRanges; ii++){
        columns.append(0);
    }
}
//---------------------------------------------------------------------------------
/**
 * @brief QCustomBars::addBarsElements
 * @brief confront each element of vec_doub with the ranges to find the columns of belonging
 * @param vec_doub
 * @param iNumBin
 */
void QCustomBars::addBarsElements(QVector<double> vec_doub, int iNumBin)
{
    inizializeColumns(iNumBin);
    for (int ii = 0; ii<vec_doub.length(); ii++)
    {
        int index = 0;
        for (int kk = 0; kk<iNumBin-1; kk++){

            if (vec_doub[ii]>ranges[kk]){
                index++;
            }
            else{
                break;
            }
        }
        double appo =columns[index];
        appo++;
        columns[index]=appo;
    }
}
//---------------------------------------------------------------------------------
/**
 * @brief QCustomBars::makeConfrontRanges
 * @brief create a vector of doubles with the ranges
 * @warning this is meant to be used only with hours on x axis due to the *3600*1000 multiplication
 * @param iNumBin
 */
void QCustomBars::makeConfrontRanges(enumChoiceBar choice,int iNumBin)
{
    ranges.clear();
    for (int ii = 1; ii<iNumBin; ii++){
        double d_doub = (double)ii*(max)/iNumBin;
        if (choice == 2 || choice == 0){
            d_doub = d_doub * 3600 * 1000; // to convert in ms per hour
        }
        ranges.append(d_doub);
    }
}

//---------------------------------------------------------------------------------
/**
 * @brief QCustomBars::findMax
 * @brief find max of a vector of doubles
 * @param vectorOfDouble
 * @return
 */
double QCustomBars::findMax(QVector<double> vectorOfDouble)
{
    if ( vectorOfDouble.length()== 0 )
    {
        qDebug()<<"[!] ERROR: vector is null";
    }
    double d_max = vectorOfDouble[0];

    for (int ii=0; ii<vectorOfDouble.length(); ii++)
    {
        if ( vectorOfDouble[ii]>d_max )
        {
            d_max = vectorOfDouble[ii];
        }
    }
    return d_max;
}
//---------------------------------------------------------------------------------
/**
 * @brief QCustomBars::findMin
 * @brief find min of a vector of doubles
 * @param vectorOfDouble
 * @return
 */
double QCustomBars::findMin(QVector<double> vectorOfDouble)
{
    if ( vectorOfDouble.length()== 0 )
    {
        qDebug()<<"[!] ERROR: vector is null";
    }
    double d_min = vectorOfDouble[0];

    for (int ii=0; ii<vectorOfDouble.length(); ii++)
    {
        if ( vectorOfDouble[ii]<d_min )
        {
            d_min = vectorOfDouble[ii];
        }
    }
    return d_min;
}
//---------------------------------------------------------------------------------
/**
 * @brief QCustomBars::styleAxes
 * @warning function not tested
 */
void QCustomBars::styleAxes()
{
    //prepare x axis:
    plot->xAxis->setTickLength(0, 4);
    plot->xAxis->setRange(0, 8);
    plot->xAxis->setBasePen(QPen(Qt::white));
    plot->xAxis->setTickPen(QPen(Qt::white));
    plot->xAxis->grid()->setVisible(true);
    plot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    plot->xAxis->setTickLabelColor(Qt::white);
    plot->xAxis->setLabelColor(Qt::white);

    // prepare y axis:
    plot->yAxis->setPadding(5); // a bit more space to the left border
    plot->yAxis->setBasePen(QPen(Qt::white));
    plot->yAxis->setTickPen(QPen(Qt::white));
    plot->yAxis->setSubTickPen(QPen(Qt::white));
    plot->yAxis->grid()->setSubGridVisible(true);
    plot->yAxis->setTickLabelColor(Qt::white);
    plot->yAxis->setLabelColor(Qt::white);
    plot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
    plot->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

}
//---------------------------------------------------------------------------------
/**
 * @brief QCustomBars::setBackgroundGradient
 * @warning function not tested
 */
void QCustomBars::setBackgroundGradient()
{
    // set dark background gradient:
    QLinearGradient gradient(0, 0, 0, 400);
    gradient.setColorAt(0, QColor(90, 90, 90));
    gradient.setColorAt(0.38, QColor(105, 105, 105));
    gradient.setColorAt(1, QColor(70, 70, 70));
    plot->setBackground(QBrush(gradient));
}
//---------------------------------------------------------------------------------
/**
 * @brief QCustomBars::getLabels
 * @brief given the number of columns you want and the type of graphic returns the labels for x axis
 * @param iNumBin
 * @return
 */
QVector<QString> QCustomBars::getLabels(enumChoiceBar choice,int iNumBin)
{
    QVector<QString> vec_strTemp;
    QVector<QString>labels;
    for (int ii=0; ii<=iNumBin; ii++)
    {
        double d_numb = (double)ii*(max)/iNumBin;
        QString str_converter = QString::number(d_numb,10,2);
        vec_strTemp.append(str_converter);
    }

    for (int ii = 0; ii<iNumBin; ii++){
        QString str_tempName;

        if (choice==0){
            str_tempName = changeToHour(vec_strTemp[ii]) + "\n-\n" + changeToHour(vec_strTemp[ii+1]);
        }
        else if (choice == 1){
            str_tempName = vec_strTemp[ii] + "\n-\n" + vec_strTemp[ii+1];
        }

        labels.append(str_tempName);
    }
    return labels;
}
//---------------------------------------------------------------------------------
/**
 * @brief QCustomBars::changeToHour
 * @brief change the text 13.25 to 13:15, 13.50 to 13:30, etc.
 * @param str_toChange
 * @return return the QString changed by the function
 */
QString QCustomBars::changeToHour(QString str_toChange)
{
    QString prefix, suffix, number;
    double d_suff;
    prefix = str_toChange;
    suffix = str_toChange;
    prefix = prefix.mid(0, prefix.indexOf("."));
    suffix = suffix.mid(suffix.indexOf("."),suffix.length());

    d_suff= suffix.toDouble();
    d_suff = d_suff*60;
    suffix = QString::number(d_suff);

    if (suffix.length()<2){
        suffix = "0" + suffix;
    }
    number = prefix + ":" + suffix;
    return number;
}
//---------------------------------------------------------------------------------
