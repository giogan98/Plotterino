#include "IScanLogDatabase.h"
#include "Processing/IProcessMC.h"
#include "Processing/IProcessGen.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlQuery>
#include "Driver/ILog.h"
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QFileInfo>
IScanLogDatabase iScanLogDatabase;
IScanLogDatabase::IScanLogDatabase()
{

}
//-----------------------------------------------------------------------------
/**
 * @brief ScanLogDatabase::createOpenDatabase
 * @brief create database and open it, return true if opening is successfull
 * @param strPath string to path of DataBase
 * @return TRUE if succesfully operation
 */
bool IScanLogDatabase::createOpenDatabase(std::string strPath )
{
    bool bSuccesfull = false;
    if ( strPath != ""){
        strDatabasePath.clear();
        strDatabasePath.append(QString::fromStdString(strPath));
    }
    //create database
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QString::fromStdString(strPath));
    bSuccesfull = db.open();
    iProcessMC.initTable(); //basta una, tabella è la stessas
    return bSuccesfull;
}

//-----------------------------------------------------------------------------
/**
 * @brief IScanLogDatabase::createTable
 * @brief create table using struct from ITableData
 * @param structTable
 * @return
 */
bool IScanLogDatabase::createTable(ITableData structTable)
{
    db = QSqlDatabase::database();
    bool bSuccesfull = db.open();
    if ( bSuccesfull )
    {
        QString strQuery = "SELECT count(*) FROM sqlite_master "
                           " WHERE type='table' AND name='";
        strQuery.append(structTable.getTableName().data());
        strQuery.append("';");
        QSqlQuery query;
        bSuccesfull = query.exec(strQuery);

        std::string strFields;
        strFields.clear();

        if ( query.size() <= 0 )
        {
            QString strqryTbl = "create table ";
            strqryTbl.append(structTable.getTableName().data());
            strqryTbl.append(" (");
            std::string strFields;
            strFields.clear();
            for( uint8_t ii=0; ii<structTable.getNumEl(); ii++ )
            {
                strFields.append(structTable.getField(ii));
                strFields.append(" ");
                strFields.append(structTable.getType(ii));
                if( ii < structTable.getNumEl()-1 )
                {
                    strFields.append(",");
                }
            }
            strqryTbl.append( strFields.data() );
            strqryTbl.append( ")" );
            bool bSuccesfull2 = query.exec(strqryTbl);
            // db.close();
            if( !bSuccesfull2 ){
                qDebug()<<"IScanLogDatabase::createTable Impossible to set table: already existing?";
                qDebug()<<query.lastError();
            }
        } else {
            qDebug()<<"IScanLogDatabase::createTable Already Existing DB";
        }
    }
    else
    {
        qDebug()<<"IScanLogDatabase::createTable Impossible to open DB";
    }
    return bSuccesfull;
}
//-----------------------------------------------------------------------------
/**
 * @brief IScanLogDatabase::addRecord
 * @brief create a query and execute it to add a single row in the database
 * @param penStr
 * @return
 */
bool IScanLogDatabase::addRecord(IScanLogRecord * penStr)
{
    db = QSqlDatabase::database();

    bool bSuccesfull = false;
    if (penStr != nullptr)
    {
        bSuccesfull = openDb();
        if ( bSuccesfull )
        {
            QString strTxtQuery = QString("INSERT INTO TABLELOG ("
                                          "date, time, tag, payload,comments)"
                                          "VALUES ('%1','%2','%3','%4','%5');")
                    .arg(penStr->strDate		)
                    .arg(penStr->strTime		)
                    .arg(penStr->strTag		)
                    .arg(penStr->strPayload	)
                    .arg(penStr->strCommVersus);
            QSqlQuery query;
            query.clear();
            bSuccesfull = query.exec(strTxtQuery);
            iLog.append(strTxtQuery);
            // db.close();
            if( !bSuccesfull ){
                qDebug()<<"IScanLogDatabase::addRecord Failed Query";
            }

        }
        else
        {
            qDebug()<<"IScanLogDatabase::addRecordImpossible to create open DB";
        }
    }
    else
    {
        qDebug()<<"IScanLogDatabase::addRecord Void ptr to record";
    }
    return bSuccesfull;
}
//-----------------------------------------------------------------------------
/**
 * @brief IScanLogDatabase::write
 * @param strQuery
 * @return
 */
bool IScanLogDatabase::write(std::string strQuery)
{
    bool bSuccesfull = false;
    db = QSqlDatabase::database();

    if (strQuery != "")
    {
        bSuccesfull = openDb();
        if ( bSuccesfull )
        {
            const QString strTxtQuery = QString( strQuery.data() );
            QSqlQuery query;
            bSuccesfull = query.exec(strTxtQuery);
            iLog.append(strTxtQuery);
            if( !bSuccesfull )
            {
                qDebug()<<"IScanLogDatabase::write Failed Query";
            }
        }
        else
        {
            qDebug()<<"IScanLogDatabase::write Impossible to create open DB";
        }
    }
    else
    {
        qDebug()<<"IScanLogDatabase::write Void ptr to record";
    }
    // db.close();
    return bSuccesfull;
}

//-----------------------------------------------------------------------------

bool IScanLogDatabase::vecStrAppend(std::vector<std::string> vecQuery)
{
    bool bSuccesfull = false;
    QString str_vals="";

    if (vecQuery.size()>0){
        bSuccesfull=true;
        for (std::size_t ii = 0; ii<vecQuery.size(); ii++){
            str_vals = QString::fromStdString(vecQuery[ii]);
            vec_totalQuery.append(str_vals);
        }
    }
    return bSuccesfull;
}
//-----------------------------------------------------------------------------
void IScanLogDatabase::executeLoad()
{
    db = QSqlDatabase::database();

    QSqlQuery query;
    bool bSuccesfull;
    QVector<QString> vec_fields;
    vec_fields<<":Time"<<":Time_ms"<<":Dir"<<":Field"<<":Value";

    if (vec_totalQuery.size()!=0)
    {
        bSuccesfull = openDb();
        if ( bSuccesfull )
        {
            QString str_genQuery = "INSERT INTO TableTest ( Time , Time_ms , Dir , Field ,Value )VALUES( :Time, :Time_ms , :Dir , :Field , :Value )";

            query.exec("begin exclusive transaction;");
            query.prepare(str_genQuery);

            for ( int ii = 0; ii <  vec_totalQuery.length()-5; ii+=5){

                query.bindValue(vec_fields[0], vec_totalQuery[ii+0]);
                query.bindValue(vec_fields[1], vec_totalQuery[ii+1].toInt());
                query.bindValue(vec_fields[2], vec_totalQuery[ii+2]);
                query.bindValue(vec_fields[3], vec_totalQuery[ii+3]);
                query.bindValue(vec_fields[4], vec_totalQuery[ii+4].toInt());
                query.exec();
            }

            query.exec("commit;");

        }
        // db.close();
        vec_totalQuery.clear();

    }
}
//-----------------------------------------------------------------------------

QVector<QVariant> IScanLogDatabase::getFiltered(std::string strColumnName, std::string strTable, std::string strDirection, std::string strField)
{
    QVector<QVariant> vec_values;
    bool bSuccesfull = true;
    db = QSqlDatabase::database();
    bSuccesfull = db.open();
    if ( bSuccesfull )
    {
        char u8array[512]={0,};
        snprintf(u8array, sizeof (u8array),
                 "SELECT %s FROM %s WHERE Dir LIKE '%s' AND Field LIKE '%s' ",
                 strColumnName.data(),
                 strTable.data(),
                 strDirection.data(),
                 strField.data());

        std::string str_command;
        str_command.assign(u8array);

        QSqlQuery query(QString::fromStdString(str_command));
        while (query.next()) {
            vec_values.append(query.value(0));
        }
        // db.close();
    }
    else{
        qDebug()<<db.lastError();
    }
    return vec_values; //you can convert qvariant into double, string etc if necessary


}
//-----------------------------------------------------------------------------
/**
 * @brief open Data base
 * @return TRUE if operation succesfull
 */
bool IScanLogDatabase::openDb(void)
{
    return db.open();
}
//-----------------------------------------------------------------------------
/**
 * @brief close Data base
 */
void IScanLogDatabase::closeDb(void)
{
    // db.close();
}


