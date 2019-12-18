#ifndef IPROCESSGEN_H
#define IPROCESSGEN_H
#include <string>
#include "Processing/IProcessBase.h"


class IGenRecord
{
	public:
	std::string strTime; /// text with Time [ms] from midnight
	uint32_t u32Time; /// text with useful content
	std::string strTag; /// Type to be displayed [request,set]
	std::string strField; /// Type to be displayed [request,set]
	int32_t s32Value; /// text with useful content
	IGenRecord(){clear();}
	void clear(void){
		strTime		 .clear();
		strTag		 .clear();
		strField	 .clear();
		s32Value = 0;
	}
};


class IProcessGen: private IProcessBase
{
	public:
	enum{EN_TIME ,/// "Time    "
		 EN_TIME_MS  ,/// "Time_ms "
		 EN_DIR     ,/// "Tag     "
		 EN_FIELD   ,/// "Field   "
		 EN_VALUE   ,/// "Value   "
		};
	private:
	public:
	IProcessGen();
	void initTable(void) override;
	void process(std::string strLine);
	void process0x650(std::string strPayload);
	void process0x651(std::string strPayload);
	void process0x652(std::string strPayload);
	private:
	void unpackBit8(std::string * pstrOut, uint8_t u8Val);
	void unpackBit32(std::string * pstrOut, uint32_t uiVal);
	void finalizeString(std:: string *pStrOut,
						uint32_t lBitMask,
						uint32_t lIqStatus);

	ITableData structTable;
};
extern IProcessGen iProcessGen;
#endif // IPROCESSGEN_H
