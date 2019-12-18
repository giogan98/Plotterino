#include "Processing/IProcessMC.h"
#include <QDebug>
#include <fstream>
#include "DBmanaging/IScanLogDatabase.h"
#include <QString>


IProcessMC iProcessMC;
//---------------------------------------------------------------------------
IProcessMC::IProcessMC()
    :structTable( "TableTest" , 5 )
{
    structTable.setField(EN_TIME    , "Time    " , "VARCHAR(32)" );
    structTable.setField(EN_TIME_MS , "Time_ms " , "VARCHAR(32)" );
    structTable.setField(EN_DIR     , "Dir     " , "VARCHAR(32)" );
    structTable.setField(EN_FIELD   , "Field   " , "VARCHAR(32)" );
    structTable.setField(EN_VALUE   , "Value   " , "VARCHAR(32)" );
}
//----------------------------------------------------------------------------
void IProcessMC::initTable(void)
{

    iScanLogDatabase.createTable( structTable );
}
//----------------------------------------------------------------------------
void IProcessMC::process(std::string strPayload)
{
    std::string delimiter(";");
    std::string strDirection = substrCut(&strPayload,delimiter);
    std::string strTime = substrCut(&strPayload,delimiter);
    removeChars(&strDirection," ");
    structTable.setValue(EN_DIR     , strDirection);
    removeChars(&strTime," ");
    unsigned long ulTime = timeStr2ms( strTime );
    structTable.setValue(EN_TIME    , strTime);
    structTable.setValue(EN_TIME_MS , std::to_string(ulTime) );
    // 14:54:00:829 ; <Id = 0650 Len = 06 Data = W enDLL_FluorokV(28) 00 00 00 35 >";
    const std::string str0 = "Id = ";
    IProcessBase::removeCharsUntil(&strPayload, str0);
    std::string::size_type sz;
    uint32_t iMsgId = std::stoul(strPayload, &sz, 16);
    //	DLL to M.C.   ; 13:00:02:030 ; <Id = 0650 Le
    //	M.C. to DLL   ; 13:00:02:052 ; <Id = 0651 Le

    if ( sz )
    {
        switch ( iMsgId )
        {
        case 0x650: //ok
            process0x650( strPayload );
            break;
        case 0x651:
            process0x651( strPayload );
            break;
        case 0x652:
            process0x652( strPayload );
            break;
        }
    }
}
//----------------------------------------------------------------------------
void IProcessMC::process0x650(std::string strPayload)
{
    //@trovami
    //ex.  <Id = 0651 Len = 06 Data = W enMC_FluoroTime(2F) 00 00 00 01 >
//    removeCharsUntil(&strPayload,"enMC_");
    std::size_t found = strPayload.find("enMC_");
    if (found!=std::string::npos){
        removeCharsUntil(&strPayload,"enMC_");
    }else{
        removeCharsUntil(&strPayload,"enDLL_");
    }
    std::string delimiter("(");
    std::string strField = substrCut(&strPayload,delimiter);
    delimiter.clear();
    delimiter.append(") ");
    std::string szStrId = substrCut(&strPayload,delimiter);
    uint32_t laData[8]={0,};
    sscanf( strPayload.data() ,
            "%x %x %x %x",
            &laData[0] ,
            &laData[1] ,
            &laData[2] ,
            &laData[3] );// extract numbers
    uint32_t u32BitMask  = laData[3];
    u32BitMask += (laData[2]<<8);
    u32BitMask += (laData[1]<<16);
    u32BitMask += (laData[0]<<24);
    std::string strValue = std::to_string(u32BitMask);
    delimiter.clear();
    delimiter.append(">");
    removeChars(&strPayload," ");
    std::string strHex = substrCut(&strPayload,delimiter);


    structTable.setValue(EN_FIELD   , strField );
    structTable.setValue(EN_VALUE   , strValue );
    //	std::string strQuery2 = addRecord(&structTable);
    //	iScanLogDatabase.write(strQuery2);
    std::vector<std::string> vecQuery2 = addRecord2(&structTable);
    iScanLogDatabase.vecStrAppend(vecQuery2);
}
//----------------------------------------------------------------------------
void IProcessMC::process0x651(std::string strPayload)
{
    //ex.  <Id = 0651 Len = 06 Data = W enMC_FluoroTime(2F) 00 00 00 01 >
//    removeCharsUntil(&strPayload,"enMC_");
    std::size_t found = strPayload.find("enMC_");
    if (found!=std::string::npos){
        removeCharsUntil(&strPayload,"enMC_");
    }else{
        removeCharsUntil(&strPayload,"enDLL_");
    }
    std::string delimiter("(");
    std::string strField = substrCut(&strPayload,delimiter);
    delimiter.clear();
    delimiter.append(") ");
    std::string szStrId = substrCut(&strPayload,delimiter);
    uint32_t laData[8]={0,};
    sscanf( strPayload.data() ,
            "%x %x %x %x",
            &laData[0] ,
            &laData[1] ,
            &laData[2] ,
            &laData[3] );// extract numbers
    uint32_t u32BitMask  = laData[3];
    u32BitMask += (laData[2]<<8);
    u32BitMask += (laData[1]<<16);
    u32BitMask += (laData[0]<<24);
    std::string strValue = std::to_string(u32BitMask);
    delimiter.clear();
    delimiter.append(">");
    removeChars(&strPayload," ");
    std::string strHex = substrCut(&strPayload,delimiter);

    structTable.setValue(EN_DIR     , "MC2Dll");
    structTable.setValue(EN_FIELD   , strField );
    structTable.setValue(EN_VALUE   , strValue );
    //	std::string strQuery2 = addRecord(&structTable);
    //	iScanLogDatabase.write(strQuery2);
    std::vector<std::string> vecQuery2 = addRecord2(&structTable);
    iScanLogDatabase.vecStrAppend(vecQuery2);

}
//----------------------------------------------------------------------------
void IProcessMC::process0x652(std::string strPayload)
{
    std::string strOut;
    unsigned long laData[8]={0,};
    unsigned long lBitMask = 0;
    unsigned long lMcStatus = 0;
    removeCharsUntil(&strPayload,"DEBUG data = ");
    removeChars(&strPayload,"0X");// replace 0X with blank spaces
    sscanf( strPayload.data() , "%x %x %x %x %x %x",
            &laData[0] ,
            &laData[1] ,
            &laData[2] ,
            &laData[3] ,
            &laData[4] ,
            &laData[5] );// extract numbers

    switch(laData[0])
    {
    case 6:
        lBitMask  = laData[1];
        lBitMask += (laData[2]<<8);
        lBitMask += (laData[3]<<16);
        lBitMask += (laData[4]<<24);
        lMcStatus = laData[5] ;
        structTable.setValue( EN_FIELD, "DbgIo");
        structTable.setValue( EN_VALUE, std::to_string(lBitMask));

        //		std::string strQuery = addRecord(&structTable);
        //		iScanLogDatabase.write(strQuery);
        std::vector<std::string> vecQuery = addRecord2(&structTable);
        iScanLogDatabase.vecStrAppend(vecQuery);
        structTable.setValue( EN_FIELD, "DbgMcStatus");
        structTable.setValue( EN_VALUE, std::to_string(lMcStatus));
        //		std::string strQuery2 = addRecord(&structTable);
        //		iScanLogDatabase.write(strQuery2);
        std::vector<std::string> vecQuery2 = addRecord2(&structTable);
        iScanLogDatabase.vecStrAppend(vecQuery2);
        //		finalizeString(&strOut, static_cast <uint32_t> (lBitMask), static_cast <uint32_t> (lMcStatus));
        //		//@$ <<  strOut.c_str() ; // Prints out STRING
        break;
    }
}
//--------------------------------------------------------
void IProcessMC::unpackBit8(std::string * pstrOut, uint8_t u8Val)
{
    int ii, iBit;

    for (ii=0; ii<8; ii++)
    {
        char u8aNum[4];
        iBit = ( u8Val & (1<<ii) ) !=0;
        itoa(iBit,u8aNum,10);
        pstrOut->append(u8aNum);
        pstrOut->append(" ");
    }
}
//--------------------------------------------------------
void IProcessMC::unpackBit32(std::string * pstrOut, uint32_t uiVal)
{
    int ii, iBit;

    for (ii=0; ii<19; ii++)
    {
        char u8aNum[4];
        iBit = ( uiVal & (1<<ii) ) !=0;
        itoa(iBit,u8aNum,10);
        pstrOut->append(u8aNum);
        pstrOut->append(" ");
    }
}
//----------------------------------------------------------------------------
/**
 * @brief IProcessMC::finalize String
 * @param pStrOut
 * @param ulTime
 * @param lBitMask
 * @param lMcStatus
 * @param u8TableBit
 * @param u8GenStat
 * @param iDllStat
 */
void IProcessMC::finalizeString( std::string *pStrOut,
                                 uint32_t lBitMask,
                                 uint32_t lMcStatus)
{
    char s8aDummy[16]={0,}; // more than max Int number: 9 digits + sign

    pStrOut->clear();
    //	itoa(ulTime, s8aDummy, 10);
    //	pStrOut->append(s8aDummy);
    //	pStrOut->append(" " );
    unpackBit32(pStrOut, lBitMask);
    itoa( lMcStatus,s8aDummy, 10);
    pStrOut->append(s8aDummy);
    pStrOut->append(" " );
    pStrOut->append("\n");

}
