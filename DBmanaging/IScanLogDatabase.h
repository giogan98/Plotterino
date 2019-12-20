#ifndef SCANLOGDATABASE_H
#define SCANLOGDATABASE_H
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QString>
#include <QVariant>
#include "Processing/IProcessBase.h"

class IScanLogRecord
{
public:
    QString strDate; /// text with date 2004-05-23
    QString strTime; /// text with Time 14:25:10.234
    QString strCommVersus; /// communication versus - MC->Dll
    QString strTag; /// Type to be displayed
    QString strPayload; /// text with useful content
    IScanLogRecord(){clear();}
    void clear(void){
        strDate		 .clear();
        strTime		 .clear();
        strCommVersus.clear();
        strTag		 .clear();
        strPayload	 .clear();
    }
};
class IScanLogDatabase
{

    // . . . . . . . . . . . . . . . . . . . . . . . . . . . .
private:
    QSqlDatabase db;
    QString strDatabasePath;
    QVector <QString> vec_totalQuery;

    // . . . . . . . . . . . . . . . . . . . . . . . . . . . .
public:
    IScanLogDatabase();
    bool createOpenDatabase(std::string strPath ); // crea database
    bool createTable(ITableData structTable ); //create tabletest in the database
    bool addRecord(IScanLogRecord *penStr); //add a single row
    bool write(std::string strQuery);
    bool vecStrAppend(std::vector<std::string> vecQuery);

    //*******************************************************//
    void executeLoad();
    QVector<QVariant> getFiltered (std::string strColumnName,
                                   std::string strTable,
                                   std::string strDirection,
                                   std::string strField);

    //*******************************************************//

private:
    bool openDb(void);
    void closeDb(void);
    // . . . . . . . . . . . . . . . . . . . . . . . . . . . .
};
extern IScanLogDatabase iScanLogDatabase;
#endif // SCANLOGDATABASE_H
