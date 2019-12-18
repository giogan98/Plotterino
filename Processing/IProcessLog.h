#ifndef PROCESSLOG_H
#define PROCESSLOG_H
#include <string>
#include "Processing/IProcessBase.h"
class IProcessLog: private IProcessBase
{
public:
    IProcessLog();
    void initTable(void) override;
    void processFile (const char * ucaNameFileIn,
                      const char * ucaNameFileOut);
    void processLogLine(std::string strLine);
    unsigned long timeStr2ms(const char * u8aData);

private:
    void processPayloadTable(std::string strPayload);
    void processPayloadGen(std::string strPayload);
    void processPayloadMc(std::string strPayload);
};

extern IProcessLog iProcessLog;
#endif // PROCESSLOG_H
