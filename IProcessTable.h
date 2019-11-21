#ifndef IPROCESSTABLE_H
#define IPROCESSTABLE_H
#include <string>
#include "IProcessBase.h"

class ITableIoRecord
{
	public:
	std::string strTime; /// text with Time [ms] from midnight
	std::string strTag; /// Type to be displayed
	std::string strPayload; /// text with useful content
	ITableIoRecord(){clear();}
	void clear(void){
		strTime		 .clear();
		strTag		 .clear();
		strPayload	 .clear();
	}
};

class ITableCommRecord
{
	public:

	//	std::string strCmd; /// communication command - Request, Answ, Error
	std::string strId; /// Id of command
	std::string strField; /// string name of Id
	std::string strHex; /// Hex value - paload
	int32_t s32Value; /// converted decimal value
	ITableCommRecord(){clear();}
	void clear(void){
		//		strCmd		 .clear();
		strId		 .clear();
		strField	 .clear();
		strHex		 .clear();
		s32Value = 0;
	}
};

class IProcessTable: private IProcessBase
{
	public:
	enum{
		EN_TIME    = 0,
		EN_TIME_MS ,
		EN_DIR     ,
		EN_FIELD   , // qVetoro ..qvGiorgio = ivan.dammi("AEC_ERR")
		EN_VALUE   ,
	};

	private:
	ITableData structTable;
	public:
	IProcessTable();
	void initTable(void) override;
	void process(std::string strPayload);
	void process0x120(std::string strPayload);
    void process0x125(std::string strPayload);
	void process0x403(std::string strPayload);
	private:
	void unpackBit8(std::string * pstrOut, uint8_t u8Val);
	void unpackBit32(std::string * pstrOut, uint32_t uiVal);
	void finalizeString(std:: string *pStrOut,
						uint32_t lBitMask,
						uint32_t lMcStatus);

    void appendRecordField(std::string str_nameField, unsigned long ulIndex);
};
extern IProcessTable iProcessTable;

#endif // IPROCESSTABLE_H
