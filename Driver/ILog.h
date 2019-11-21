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
#ifndef ILOG_H
#define ILOG_H
#include <QString>
#include <QFile>

class ILog
{
	private:
	QFile *file;
	QString strOldTime;
	public:
	static ILog& instance() {
		static ILog iLog;
		return iLog;
	}
	void init();
	void append(QString strTxt);
	void append(char *u8aStr, unsigned long ulSz);
	void appendErr(QString strTxt);
	void appendWarn(QString strTxt);
	void close(void);
	private:
	ILog();
	void open(void);
};
extern ILog iLog;
#endif // ILOG_H
