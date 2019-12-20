/******************************************************************************
 * @file 		Automatic LOG for QT projects
 * @brief		This class is used to configure and use TXT Log class
 * @author		IVAN Perletti - General Medical Merate.spa - Seriate - ITALY
 * @version		1.0
 * @date		May 12th, 2019
 * @pre			Initialize and Enable the Serial class for the communication
 * @post 		Nope
 * @bug			Not all memory is freed when deleting an object of this class.
 * @warning		Improper use can crash your application
 * @copyright 	GMM.spa - All Rights Reserved
 *
 ******************************************************************************/
#include "ILog.h"

#include <QTextStream>
#include <QIODevice>
#include <QTime>
#include <string.h>

ILog iLog = ILog::instance();

ILog::ILog()
{
    file = new QFile;
}

void ILog::init(void)
{
    QString str2("log/TsLog_");
    str2.append(QDate::currentDate().toString("yyyy_MM_dd"));
    str2.append(".log");
    file->setFileName(str2);
    append("Session start! ");

}
//------------------------------------------------------------------------------
/**
 * @brief ILog::append Error to Log
 * @param strTxt stign to be appended
 */
void ILog::appendErr(QString strTxt)
{
    append("[!]:"+strTxt);
}
//------------------------------------------------------------------------------
/**
 * @brief ILog::append Warning to Log
 * @param strTxt stign to be appended
 */
void ILog::appendWarn(QString strTxt)
{
    append("[W]:"+strTxt);
}
//------------------------------------------------------------------------------
/**
 * @brief ILog::append Text to Log
 * @param strTxt stign to be appended
 */
void ILog::append(QString strTxt)
{
    QString strNewTime = (QTime::currentTime().toString("hh:mm:ss.zzz"));
    //	strOldTime.clear();
    //	strOldTime.append(QTime::currentTime().toString("hhmmss"));
    //	bool bFlag0, bFlag1;
    //	long lNumNew = strNewTime.toLong(&bFlag0,10);
    //	long lNumOld = strOldTime.toLong(&bFlag1,10);
    //	if(lNumOld > lNumNew){
    //		close();
    //		init();
    //	}
    strNewTime.append("; ");
    strNewTime.append(strTxt);
    strNewTime.append("\r\n");
    if (!file->isOpen())
    {
        open();
    }
    if (file->isOpen())
    {
        file->write(strNewTime.toUtf8());
        close();
    }
}

void ILog::append(char * u8aStr, unsigned long ulSz)
{
    char u8aMsg256[256] = {0,};
    if (ulSz >= sizeof(u8aMsg256))
    {
        ulSz = 0;
    }

    memcpy(u8aMsg256, u8aStr, ulSz);
    QString str2(u8aStr);
    append(str2);
}


void ILog::open(void)
{
    file->open(QIODevice::Append);
}
void ILog::close(void)
{
    file->close();
}


