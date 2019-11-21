#ifndef IPROCESSBASE_H
#define IPROCESSBASE_H
#include <string>
#include <vector>

struct ITableFieldData {
	std::string strFieldName;
	std::string strType;
	std::string strValue;
};

class ITableData
{
	public: // . . . . . . . . . . . . . . . . . .
	ITableData(std::string sTableName,uint8_t u8FieldNum);
	void clear(void);
	uint8_t getNumEl(void){return (u8FieldNumber);}
	std::string getTableName(void );
	void setField(uint8_t u8FieldNum ,
				  std::string sFieldName,
				  std::string sFieldType);
	void setValue(uint8_t u8FieldNum ,
				  std::string sValue);
	std::string getField( uint8_t u8FieldNum );
	std::string getType( uint8_t u8FieldNum );
	std::string getValue( uint8_t u8FieldNum );

	private: // . . . . . . . . . . . . . . . . . .
	std::string strTableName;
	ITableFieldData * pFldData;
	uint8_t u8FieldNumber;
};

constexpr unsigned int strdjb(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (strdjb(str, h+1) * 33) ^ static_cast<uint32_t>(str[h]);
}
class IProcessBase
{
	public:
	IProcessBase();
	virtual void initTable(void) = 0;
	unsigned long timeStr2ms(std::string strTime);
	static void removeCharsUntil(std::string * strProcessed,
								 const std::string strMatchToFind);
	static void removeChars(std::string * strProcessed,
							std::string strMatchToFind);
	static void removeCharOccurrence(std::string * str,
									 const char u8Val);
	std::string substrFind(std::string *str, std::string delimiter);
	std::string substrCut(std::string *str, std::string delimiter);
    std::string addRecord(ITableData * pStructTable);
    std::vector<std::string> addRecord2(ITableData * pStructTable);

};

#endif // IPROCESSBASE_H
