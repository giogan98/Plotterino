#include "IProcessGen.h"
#include "IScanLogDatabase.h"
#include <QDebug>
#include <cstdlib>
IProcessGen iProcessGen;

IProcessGen::IProcessGen()
    :	structTable( "TableTest" , 5 )
{
	structTable.setField(EN_TIME    , "Time    " , "VARCHAR(32)" );
	structTable.setField(EN_TIME_MS , "Time_ms " , "VARCHAR(32)" );
	structTable.setField(EN_DIR     , "Dir     " , "VARCHAR(32)" );
	structTable.setField(EN_FIELD   , "Field   " , "VARCHAR(32)" );
	structTable.setField(EN_VALUE   , "Value   " , "VARCHAR(32)" );
}
//----------------------------------------------------------------------------
void IProcessGen::initTable(void)
{
	iScanLogDatabase.createTable( structTable );
}
//----------------------------------------------------------------------------
void IProcessGen::process(std::string strPayload)
{
	// IQ to DLL     ; 13:03:34:212 ; <<RX CLN - RESPONSE - SET - LEN=45, MSG.ID=9, NR.ELEM=6, RADTECHNIQUE=1, RADFOCUS=1, RADKV=120, RADMA=200.000, RADMS=10.000, RADMAS=2.000 >";
	std::string delimiter(";");
	std::string strDirection = substrCut(&strPayload,delimiter);
	std::string strTime = substrCut(&strPayload,delimiter);
	removeChars(&strDirection," ");
	structTable.setValue(EN_DIR     , strDirection);
	removeChars(&strTime," ");
	unsigned long ulTime = timeStr2ms( strTime );
	structTable.setValue(EN_TIME    , strTime);
	structTable.setValue(EN_TIME_MS , std::to_string(ulTime) );

	removeCharsUntil(&strPayload, "NR.ELEM");
	int32_t iNumEl;
	int32_t iSuccess = sscanf( strPayload.data() , "=%ul", &iNumEl );// extract numbers
	removeCharsUntil(&strPayload, ", ");
	std::string strField;
	int32_t s32Value;
	if( iSuccess > 0 )
	{
		for ( int32_t ii=0; ii < iNumEl; ii++ )
		{
			strField.clear();
			strField = substrCut(&strPayload, "=");

			long lOk = sscanf( strPayload.data() , "%li", &s32Value );// extract numbers
			if ( lOk > 0 )
			{
				structTable.setValue( EN_FIELD, strField);
				structTable.setValue( EN_VALUE, std::to_string(s32Value));
                std::vector<std::string> vec_Query = addRecord2(&structTable);
                iScanLogDatabase.vecStrAppend(vec_Query);
			}
			removeCharsUntil(&strPayload, ", ");
		}
	}
}


