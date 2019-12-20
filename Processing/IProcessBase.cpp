#include "Processing/IProcessBase.h"
#include <algorithm>
#include <QDebug>
using namespace std;
//-----------------------------------------------------------------------------
ITableData::ITableData(std::string sTableName,uint8_t u8FieldNum)
{
    strTableName.assign(sTableName);
    u8FieldNumber = u8FieldNum;
    pFldData = new (nothrow) ITableFieldData[u8FieldNumber];
    clear();
}
//-----------------------------------------------------------------------------
void ITableData::clear(void)
{
    for (int ii = 0 ; ii < u8FieldNumber; ii++)
    {
        pFldData[ii].strFieldName.clear();
        pFldData[ii].strType.clear();
    }
}
//-----------------------------------------------------------------------------
string ITableData::getTableName(void)
{
    return (strTableName);
}

//-----------------------------------------------------------------------------
void ITableData::setField(uint8_t u8FieldNum ,
                          std::string sFieldName,
                          std::string sFieldType)
{
    if ( u8FieldNum < u8FieldNumber)
    {
        pFldData[u8FieldNum].strFieldName.clear();
        pFldData[u8FieldNum].strFieldName.assign(sFieldName);
        pFldData[u8FieldNum].strType.clear();
        pFldData[u8FieldNum].strType.assign(sFieldType);
    }
}
//-----------------------------------------------------------------------------
void ITableData::setValue(uint8_t u8FieldNum ,
                          std::string sValue)
{
    if ( u8FieldNum < u8FieldNumber)
    {
        pFldData[u8FieldNum].strValue.clear();
        pFldData[u8FieldNum].strValue.assign(sValue);
    }
}
//-----------------------------------------------------------------------------
std::string ITableData::getField( uint8_t u8FieldNum )
{
    std::string strOut = "";
    if ( u8FieldNum < u8FieldNumber)
    {
        strOut.append(pFldData[u8FieldNum].strFieldName);
    }
    return (strOut);
}
//-----------------------------------------------------------------------------
std::string ITableData::getType( uint8_t u8FieldNum )
{
    std::string strOut = "";
    if ( u8FieldNum < u8FieldNumber)
    {
        strOut.append(pFldData[u8FieldNum].strType);
    }
    return (strOut);
}
//-----------------------------------------------------------------------------
std::string ITableData::getValue( uint8_t u8FieldNum )
{
    std::string strOut = "";
    if ( u8FieldNum < u8FieldNumber)
    {
        strOut.append(pFldData[u8FieldNum].strValue);
    }
    return (strOut);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
IProcessBase::IProcessBase()
{

}
//-----------------------------------------------------------------------------
/**
 * @brief ProcessLog::unpack Time String
 * @param strTime    time fomratted string HH:MM:SS.dms
 * @return
 */
unsigned long IProcessBase::timeStr2ms(std::string strTime)
{
    unsigned long  ulTime = 0;
    unsigned int l_hour=0, l_min=0, l_sec=0, l_ms=0;
    removeChars(&strTime," ");
    sscanf( strTime.data() ,
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
//--------------------------------------------------------
void IProcessBase::removeCharsUntil(std::string * strProcessed, const std::string strMatchToFind)
{
    size_t szPos;

    szPos = strProcessed->find(strMatchToFind);
    if (szPos < strProcessed->size() ){
        strProcessed->erase (0, szPos+strMatchToFind.size());
    }
}
//--------------------------------------------------------
void IProcessBase::removeChars(std::string * strProcessed, std::string strMatchToFind)
{
    size_t szPos;

    szPos = strProcessed->find(strMatchToFind);

    while (szPos < strProcessed->size() )
    {
        strProcessed->erase (szPos,strMatchToFind.size());
        szPos = strProcessed->find(strMatchToFind);
    }
}
//--------------------------------------------------------
void IProcessBase::removeCharOccurrence(std::string * str, const char u8Val)
{
    str->erase(std::remove(str->begin(), str->end(), u8Val), str->end());
}
//--------------------------------------------------------
/**
 * @brief IProcessBase::substring Find
 * @param str  text to be analyzed
 * @param delimiter   ending characters to be found
 * @return rsutlign string
 */
std::string IProcessBase::substrFind(std::string * str, std::string delimiter)
{
    size_t pos = 0;
    std::string token = "";

    pos = str->find(delimiter);
    if( pos != std::string::npos)
    {
        token = str->substr(0, pos);
        //@$ << str->data();
    }
    return (token);
}
//--------------------------------------------------------
/**
 * @brief IProcessBase::substring Cut
 * @param str  text to be analyzed
 * @param delimiter   ending characters to be found
 * @return rsutlign string
 */
std::string IProcessBase::substrCut(std::string * str, std::string delimiter)
{
    size_t pos = 0;
    std::string token = "";

    pos = str->find(delimiter);
    if( pos != std::string::npos)
    {
        token = str->substr(0, pos);
        str->erase(0, pos + delimiter.length());
        //@$ << str->data();
    }
    return (token);
}


//----------------------------------------------------------------------------
std::string IProcessBase::addRecord(ITableData * pstrTable)
{
    std::string strQuery;

    std::string strFields;
    std::string strValues;
    strFields.clear();
    strValues.clear();
    for (uint8_t ii=0; ii<pstrTable->getNumEl(); ii++)
    {
        strFields.append(pstrTable->getField(ii));

        strValues.append("'");
        strValues.append(pstrTable->getValue(ii));
        strValues.append("'");
        if( ii < pstrTable->getNumEl()-1 )
        {
            strFields.append(",");
            strValues.append(",");
        }
    }

    strQuery.assign("INSERT INTO ");
    strQuery.append(pstrTable->getTableName());
    strQuery.append("( ");
    strQuery.append(strFields);
    strQuery.append(")VALUES (");//'%1','%2','%3','%4','%5');"
    strQuery.append(strValues);
    strQuery.append(")");
    //@$<<strQuery.data();
    return (strQuery);

}
//----------------------------------------------------------------------------

std::vector<string> IProcessBase::addRecord2(ITableData * pstrTable)
{
    std::string strQuery;

    vector<string> str_vecValues; //devo returnare questo

    std::string strFields;
    std::string strValues;
    std::string strPointFields;
    strFields.clear();
    //	strValues.clear();
    strPointFields.clear();
    for (uint8_t ii=0; ii<pstrTable->getNumEl(); ii++)

    {
        strValues.clear();
        strPointFields.append(" :");
        strFields.append(pstrTable->getField(ii));
        strPointFields.append(pstrTable->getField(ii));

        //		strValues.append("'");
        strValues.append(pstrTable->getValue(ii));
        str_vecValues.push_back(strValues);
        //		strValues.append("'");
        if( ii < pstrTable->getNumEl()-1 )
        {
            strFields.append(",");
            strPointFields.append(",");
            //			strValues.append(",");

        }
    }

    //                    strQuery.assign("INSERT INTO ");
    //                    strQuery.append(pstrTable->getTableName());
    //                    strQuery.append("( ");
    //                    strQuery.append(strFields);
    //                    strQuery.append(")VALUES (");//'%1','%2','%3','%4','%5');"
    //                //	strQuery.append(strValues);
    //                    strQuery.append(strPointFields);


    strQuery.append(")");
    //@$<<strQuery.data();
    return (str_vecValues);


}
