#include "IProcessTable.h"
#include <QDebug>
#include <fstream>
#include "IScanLogDatabase.h"
#include <QString>


IProcessTable iProcessTable;
//---------------------------------------------------------------------------
IProcessTable::IProcessTable()
    :structTable( "TableTable" , 5 )
{
    structTable.setField(EN_TIME    , "Time    " , "VARCHAR(32)" );
    structTable.setField(EN_TIME_MS , "Time_ms " , "VARCHAR(32)" );
    structTable.setField(EN_DIR     , "Dir     " , "VARCHAR(32)" );
    structTable.setField(EN_FIELD   , "Field   " , "VARCHAR(32)" );
    structTable.setField(EN_VALUE   , "Value   " , "VARCHAR(32)" );
}
//----------------------------------------------------------------------------
void IProcessTable::initTable(void)
{

    iScanLogDatabase.createTable( structTable );
}
//----------------------------------------------------------------------------
void IProcessTable::process(std::string strPayload)
{
    std::string delimiter(";");
    std::string strDirection = substrCut(&strPayload,delimiter);
    std::string strTime = substrCut(&strPayload,delimiter);
    removeChars(&strDirection," ");
    structTable.setValue(EN_DIR     , strDirection);//@todo dir setted here and afterwards??
    removeChars(&strTime," ");
    unsigned long ulTime = timeStr2ms( strTime );
    structTable.setValue(EN_TIME    , strTime);
    structTable.setValue(EN_TIME_MS , std::to_string(ulTime) );
    IProcessBase::removeCharsUntil(&strPayload, " (");
    IProcessBase::removeCharsUntil(&strPayload, "<Id = ");
    std::string::size_type sz;
    uint32_t iMsgId = std::stoul(strPayload, &sz, 16);

    if ( sz )
    {
        switch ( iMsgId )
        {
        case 0x120:
            process0x120( strPayload );
            break;
        case 0x125:
            process0x125( strPayload );
            break;
        case 0x403:
            process0x403( strPayload );
            break;
        default:
            break;
        }
    }
}
//----------------------------------------------------------------------------
void IProcessTable::process0x120(std::string strPayload)
{
    bool bSuccesfull=false;
    std::vector<std::string> vec_Query;
    //ex.  CMD_TABLE_STOREVAR (01) 00 00 00 00 FF 5E 02 >
    removeCharsUntil(&strPayload,"CMD_TABLE_STOREVAR ");
    std::string strField;
    uint32_t laData[8]={0,};
    sscanf( strPayload.data() ,
            "(%x) %x %x %x %x %x %x %x",
            &laData[0] ,
            &laData[1] ,
            &laData[2] ,
            &laData[3] ,
            &laData[4] ,
            &laData[5] ,
            &laData[6] ,
            &laData[7]
            );// extract numbers
    long lData=0;
    long lSwitch=0;
    lSwitch= static_cast<long>((laData[0]<<16)+(laData[1]<<8)+(laData[2]));
    switch (lSwitch) {
    case 0x010001:
        strField.assign("INCID_DEGx10");
        lData = static_cast<short>((laData[4]<<8)+laData[3]);
        bSuccesfull=true;
        break;
    case 0x010000:
        strField.assign("TILT_DEGx10");
        lData = static_cast<short>((laData[4]<<8)+laData[3]);
        bSuccesfull=true;
        break;
    default:
        bSuccesfull=false;
        break;
    }
    if (bSuccesfull){
        std::string strValue = std::to_string(lData);
        //structTable.setValue(EN_DIR     , "TABLEtoDLL");
        structTable.setValue(EN_FIELD   , strField );
        structTable.setValue(EN_VALUE   , strValue );
        //        std::string strQuery2 = addRecord(&structTable);
        //        iScanLogDatabase.write(strQuery2);
        vec_Query = addRecord2(&structTable);
        iScanLogDatabase.vecStrAppend(vec_Query);
        vec_Query.clear();
    }
}
//----------------------------------------------------------------------------
void IProcessTable::process0x403(std::string strPayload)
{
    std::vector<std::string> vec_Query;
    //ex.  CAN_ID_COLLIMATOR_STATE (0403) Len = 08 Data = 00 40 00 73 00 FA 01 2C >
    removeCharsUntil(&strPayload,"Data = ");
    std::string strField ;
    long laData[8]={0,};
    sscanf( strPayload.data() ,
            "%x %x %x %x %x %x %x %x",
            &laData[0] ,
            &laData[1] ,
            &laData[2] ,
            &laData[3] ,
            &laData[4] ,
            &laData[5] ,
            &laData[6] ,
            &laData[7]
            );
    long lData;

    strField.assign("FOCAL_CM");
    lData = laData[3];
    std::string strValue = std::to_string(lData);
    //structTable.setValue(EN_DIR     , "TABLEtoDLL");
    structTable.setValue(EN_FIELD   , strField );
    structTable.setValue(EN_VALUE   , strValue );
    //        std::string strQuery2 = addRecord(&structTable);
    //        iScanLogDatabase.write(strQuery2);
    vec_Query = addRecord2(&structTable);
    iScanLogDatabase.vecStrAppend(vec_Query);
    vec_Query.clear();


    strField.assign("COLLIM_X");
    lData = static_cast<short>((laData[4]<<8)+laData[5]);
    strValue.assign(std::to_string(lData));
    //structTable.setValue(EN_DIR     , "TABLEtoDLL");
    structTable.setValue(EN_FIELD   , strField );
    structTable.setValue(EN_VALUE   , strValue );
    //        strQuery2.clear();
    //        strQuery2 = addRecord(&structTable);
    //        iScanLogDatabase.write(strQuery2);
    vec_Query = addRecord2(&structTable);
    iScanLogDatabase.vecStrAppend(vec_Query);
    vec_Query.clear();


    strField.assign("COLLIM_Y");
    lData = static_cast<short>((laData[6]<<8)+laData[7]);
    strValue.assign(std::to_string(lData));
    //structTable.setValue(EN_DIR     , "TABLEtoDLL");
    structTable.setValue(EN_FIELD   , strField );
    structTable.setValue(EN_VALUE   , strValue );
    vec_Query = addRecord2(&structTable);
    iScanLogDatabase.vecStrAppend(vec_Query);
    vec_Query.clear();


}
//----------------------------------------------------------------------------
void IProcessTable::appendRecordField(std::string str_nameField, unsigned long ulIndex)
{
    structTable.setValue( EN_FIELD, str_nameField);
    structTable.setValue( EN_VALUE, std::to_string(ulIndex));
    std::vector<std::string> vecQuery = addRecord2(&structTable);
    iScanLogDatabase.vecStrAppend(vecQuery);
}
//----------------------------------------------------------------------------
void IProcessTable::process0x125(std::string strPayload)
{
    //    TABLE to DLL  ; 07:55:24:443 ; <Id = CAN_ID_CLISIS_DBG_125 (0125) Len = 08 Data = 88 00 00 00 00 00 00 80 >

    //    *  BYTE 0 = low byte di Conf_Rx_bits
    //    *  bit Preparation_bit     = Conf_Rx_bits^0    // stato del segnale del tasto  preparazione
    //    *  bit Fluoro_bit          = Conf_Rx_bits^1    // stato del segnale del tasto  fluoro
    //    *  bit Exposure_bit        = Conf_Rx_bits^2    // stato del segnale del tasto  esposizione
    //    *  bit Debug_toggle_bit    = Conf_Rx_bits^3    // toggling bit per indicare tx  attiva
    //    *  bit FLUORO_bit          = Conf_Rx_bits^4    // fluoroscopia (fluoro HW)
    //    *  bit PREPARATION_bit     = Conf_Rx_bits^5    // preparazione (preparazione HW)
    //    *  bit EXPOSURE_bit        = Conf_Rx_bits^6    // esposizione rx (rx HW)
    //    *  bit SEC_STEP_bit        = Conf_Rx_bits^7    // grafia (enable HW)
    //    *
    //    *  BYTE 1 = high byte di Conf_Rx_bits
    //    *
    //    *  bit Gen_Req_bit         = Conf_Rx_bits^8    // stato del segnale in ingresso  del generator request
    //    *  bit Gen_Xray_on_bit     = Conf_Rx_bits^9    // stato del segnale in ingresso  di xray on da generatore
    //    *  bit Preparation_bit_ext = Conf_Rx_bits^10   // stato del segnale di  preparazione ext
    //    *  bit Fluoro_bit_ext      = Conf_Rx_bits^11   // stato del segnale di fluoro  ext
    //    *  bit Exposure_bit_ext    = Conf_Rx_bits^12   // stato del segnale di  esposizione ext
    //    *  bit Debug_error_bit     = Conf_Rx_bits^13   // errori di debug presenti
    //    *  bit Status_CPU_bit      = Conf_Rx_bits^14   // stato attuale del programma
    //    *  bit Dbg_bit_ext         = Conf_Rx_bits^15   // free
    std::string strOut;
    //    TABLE to DLL  ; 08:22:58:921 ; <Id = CAN_ID_CLISIS_DBG_125 (0125) Len = 08 Data = ED 21 00 00 9A 14 14 00 >
    unsigned long laData[8]={0,};
    unsigned long lBitMask = 0;
    removeCharsUntil(&strPayload,"Data = ");
    removeChars(&strPayload,"0X");// replace 0X with blank spaces
    sscanf( strPayload.data() , "%x %x %x %x %x %x %x %x",
            &laData[0] ,
            &laData[1] ,
            &laData[2] ,
            &laData[3] ,
            &laData[4] ,
            &laData[5] ,
            &laData[6] ,
            &laData[7] );// extract numbers

    lBitMask  = laData[0];
    lBitMask += (laData[1]<<8);
    //@todo fix problem with bitmask, names repeat themselves
    appendRecordField("PreparationBit"     , lBitMask&(1<<0 ));
    appendRecordField("FluoroBit"          , lBitMask&(1<<1 ));
    appendRecordField("ExposureBit"        , lBitMask&(1<<2 ));
    appendRecordField("Debug_toggleBit"    , lBitMask&(1<<3 ));
    appendRecordField("FLUOROBit"          , lBitMask&(1<<4 ));
    appendRecordField("PREPARATIONBit"     , lBitMask&(1<<5 ));
    appendRecordField("EXPOSUREBit"        , lBitMask&(1<<6 ));
    appendRecordField("SEC_STEPBit"        , lBitMask&(1<<7 ));

    appendRecordField("Gen_ReqBit"         , lBitMask&(1<<8 ));
    appendRecordField("Gen_Xray_onBit"     , lBitMask&(1<<9 ));
    appendRecordField("PreparationExtBit"  , lBitMask&(1<<10));
    appendRecordField("FluoroExtBit"       , lBitMask&(1<<11));
    appendRecordField("ExposurExtBit"      , lBitMask&(1<<12));
    appendRecordField("Debug_errorBit"     , lBitMask&(1<<13));
    appendRecordField("Status_CPUBit"      , lBitMask&(1<<14));
    appendRecordField("DbgBit"             , lBitMask&(1<<15));

}
//--------------------------------------------------------
void IProcessTable::unpackBit8(std::string * pstrOut, uint8_t u8Val)
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
void IProcessTable::unpackBit32(std::string * pstrOut, uint32_t uiVal)
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
 * @brief IProcessTable::finalize String
 * @param pStrOut
 * @param ulTime
 * @param lBitMask
 * @param lMcStatus
 * @param u8TableBit
 * @param u8GenStat
 * @param iDllStat
 */
void IProcessTable::finalizeString( std::string *pStrOut,
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



