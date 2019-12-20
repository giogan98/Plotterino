#include "Processing/IProcessLog.h"
#include <QDebug>
#include <fstream>
#include "DBmanaging/IScanLogDatabase.h"
#include "Processing/IProcessMC.h"
#include "Processing/IProcessGen.h"
#include "Processing/IProcessTable.h"
#include <QString>


IProcessLog iProcessLog;
//------------------------------------------------------------------------------
IProcessLog::IProcessLog()
{

}
//-----------------------------------------------------------------------------
void IProcessLog::initTable(void)
{

}
//-----------------------------------------------------------------------------
/**
 * @brief ProcessLog::unpack Time String
 * @param u8aData
 * @return
 */
unsigned long IProcessLog::timeStr2ms(const char * u8aData)
{
    unsigned long  ulTime;
    unsigned int l_hour=0, l_min=0, l_sec=0, l_ms=0;
    sscanf( u8aData ,
            "%02d:%02d:%02d:%03d",
            &l_hour	,
            &l_min	,
            &l_sec	,
            &l_ms	);
    ulTime = l_ms + (l_sec + l_min*60 + l_hour * 3600)*1000; // max Num: 90060999ms
    if (ulTime >= 47018074)
        ulTime++;
    return(ulTime);
}
//------------------------------------------------------------------------
void IProcessLog::processPayloadTable(std::string strPayload)
{
    iProcessTable.process(strPayload);
}
//------------------------------------------------------------------------
void IProcessLog::processPayloadGen(std::string strPayload)
{
    iProcessGen.process(strPayload);
}
//------------------------------------------------------------------------
void IProcessLog::processPayloadMc(std::string strPayload)
{
    iProcessMC.process(strPayload);
}
//------------------------------------------------------------------------
/**
 * @brief ProcessLog process Log Line
 * @param strLine   text lines to be processed
 */
void IProcessLog::processLogLine(std::string strLine)
{
    std::string strDelimit = "; ";
    std::string strCommDir =
            substrFind(&strLine, strDelimit); // communication direction
    removeCharOccurrence(&strCommDir,' ');
    switch( strdjb(strCommDir.data()) )
    {
    //@attention, missing case 1 and 3?
    case strdjb("M.C.toDLL"):
    case strdjb("DLLtoM.C."):
        processPayloadMc(strLine);
        break;
    case strdjb("IQtoDLL"):
    case strdjb("DLLtoIQ"):
        processPayloadGen(strLine);
        break;
    case strdjb("TABLEtoDLL"):
    case strdjb("DLLtoTABLE"):
        processPayloadTable(strLine);
        break;

    default:
        break;
    }
}
//------------------------------------------------------------------------
void IProcessLog::processFile (const char * ucaNameFileIn,
                               const char * ucaNameFileOut)
{
    std::string strLineLog, strOut;
    std::string previousLine = "";
    std::ifstream infile;
    std::ofstream outFile;
    int iRowCounter = 0;

    if (ucaNameFileIn == nullptr || ucaNameFileOut == nullptr ){
        return;
    }

    infile.open (ucaNameFileIn);
    outFile.open (ucaNameFileOut, std::ofstream::out | std::ofstream::trunc);

    //@$<<"processing files";
    while(iRowCounter<1000000) // To get you all the lines.
    {
        getline(infile,strLineLog); // Saves the line in STRING.
    }
}
//------------------------------------------------------------------------------
