#ifndef IPROCESSMC_H
#define IPROCESSMC_H
#include <string>
#include "Processing/IProcessBase.h"

class IMcIoRecord
{
	public:
	std::string strTime; /// text with Time [ms] from midnight
	std::string strTag; /// Type to be displayed
	std::string strPayload; /// text with useful content
	IMcIoRecord(){clear();}
	void clear(void){
		strTime		 .clear();
		strTag		 .clear();
		strPayload	 .clear();
	}
};

class IMcCommRecord
{
	public:

	//	std::string strCmd; /// communication command - Request, Answ, Error
	std::string strId; /// Id of command
	std::string strField; /// string name of Id
	std::string strHex; /// Hex value - paload
	int32_t s32Value; /// converted decimal value
	IMcCommRecord(){clear();}
	void clear(void){
		//		strCmd		 .clear();
		strId		 .clear();
		strField	 .clear();
		strHex		 .clear();
		s32Value = 0;
	}
};

class IProcessMC: private IProcessBase
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
	IProcessMC();
	void initTable(void) override;
	void process(std::string strPayload);
	void process0x650(std::string strPayload);
	void process0x651(std::string strPayload);
	void process0x652(std::string strPayload);
	private:
	void unpackBit8(std::string * pstrOut, uint8_t u8Val);
	void unpackBit32(std::string * pstrOut, uint32_t uiVal);
	void finalizeString(std:: string *pStrOut,
						uint32_t lBitMask,
						uint32_t lMcStatus);

};
extern IProcessMC iProcessMC;
#endif // IPROCESSMC_H
