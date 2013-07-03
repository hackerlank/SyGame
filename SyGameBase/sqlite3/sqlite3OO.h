#pragma once
extern "C"{
#include "sqlite3.h"
}
#include "map"
#include <vector>
#include <string>
//#include <sys.h>
/**
 * sqlite3 ���ݿ������װ
 ***/
namespace data{
#pragma pack(1)
	struct vCoreData{
		unsigned int type;  // ��������
		vCoreData()
		{
			type = 0;
		}
	};
#pragma pack()
	class Sqlite3Database;
	struct Sqlite3Table{
		/**
 		 * �������ݿ�������
 		 * <code> ʵ��vOPData::setDBM</code>
 		 * \param dbm ��ǰ���ݹ������
 		 * */
		virtual void setDBM(Sqlite3Database *dbm){this->dbm = dbm;}
		Sqlite3Database *dbm; // ���ݿ�������
		vCoreData *coreData; // ��ʱ���ݶ���
		Sqlite3Table(){dbm = NULL;}
		enum{
			STORE = 1,
			LOAD = 2,
		};
		unsigned char tag; // ��ǰ�������� 1 �� 2 ȡ
		int pos; // ��ǰstmt ��Ч�ֶ�λ��	
		int fieldIndex; // ��ǰ�ֶ�����
		std::vector<std::string> fields; // ��ǰ�ֶ�ֵ
		std::string tableName; // ������
		/**
 		 * ��ȡ��ǰ��Ĺؼ���
 		 * \return �ؼ���
 		 * */
		virtual const char* getKeyName() {return NULL;}
		
		/**
 		 * �������� ��ȡ�󶨶���
 		 * \param name �ֶ�����
 		 * \param pStmt stmt 
 		 * 	Ҫ������ʵ��֮
 		 * 	�������ֶ�ʱ ����true
 		 * 	���������ֶ�ʱ ����false
 		 * \return �Ƿ���ڸ��ֶεİ�
 		 ***/
		virtual bool genBind(std::string& name,sqlite3_stmt *pStmt)= 0;
		/**
 		 * ��鳤��
 		 * \param name �ֶ���
 		 * \param len �ֶγ���
 		 * **/
		virtual void checkValue(std::string & name,unsigned long len) {}

		/**
 		 * ����������
 		 * */
		void bind(int &value,sqlite3_stmt *pStmt);
		/**
 		 * �������޷�������
 		 * */		
		void bind(unsigned int &value,sqlite3_stmt *pStmt);
		/**
 		 * ��һ���ַ�
 		 * */
		void bind(char &value,sqlite3_stmt *pStmt);
		/**
 		 * ��һ���޷����ַ�
 		 * */
		void bind(unsigned char &value,sqlite3_stmt *pStmt);
		/**
		 * �󶨶����ƴ�
		 * */
		void bind(unsigned char * values,unsigned int len,sqlite3_stmt *pStmt);
		/**
 		 * ���ַ���
 		 * */
		void bind(char *values,unsigned int len,sqlite3_stmt *pStmt);
		/**
		 * ��string
		 */
		void bind(std::string &name,sqlite3_stmt *pStmt);
		/**
		 * ���ַ���
		 */
		void bind(std::vector<unsigned char > &content,sqlite3_stmt *pStmt);
		/**
 		 * ִ�в������
 		 * \param coreData ��������
 		 * \return �ɹ� true ʧ��false
 		 * **/	
		bool execInsert(vCoreData *coreData);	
		/**
 		 * ִ�и��²���
 		 * \param coreData �����Ķ���
 		 * \return �ɹ�true ʧ��false
 		 * */
		bool execUpdate(vCoreData *coreData);
		/**
 		 * ִ�л�ȡ����
 		 * \param coreData �����Ķ���
 		 * \return �ɹ�true ʧ��false
 		 * */
		bool execSelect(vCoreData *coreData);
		
		sqlite3 *sqlite; // sqlite3 ��������
	};
	/**
 	 * sqlite���ݿ����
 	 * */
	class Sqlite3Database{
	public:
		std::string fileName;
		Sqlite3Database()
		{
			sqlite = 0;
		}
		/**
 		 * ��ʼ��  test ���ݿ�
 		 * \return �ɹ�ʧ��
 		 * */
		bool init();
		bool init(const char *fileName);
		bool init(const char *fileName,const char *createTable[]);
		/**
 		 * �ͷ���Դ
 		 * \return �ɹ���� true or false 
		 **/
		bool release();
		/**
		 * ��ȡ��
		 * \param tableName ����
		 * \return sqlite ���������
		 * */
		virtual Sqlite3Table* takeTable(const char * tableName) {return NULL;};
		/**
     		 *  ��Դ���ͷ�
     		 * */	
		virtual void BeReleased(){}
	
		sqlite3* sqlite; // sqlite3 ���
		/**
 		 * ��������
 		 * \param data ��ǰ���� ע������ʱ���� ��ָ�뺯��ִ����� ʧЧ
 		 * **/	
		virtual void addObject(vCoreData *data){}	

		/**
		 * ������������
		 */
		void begin();
		
		/**
		 * ������������
		 */
		void end();
	protected:
	};
	
	class Sqlite3DB:public Sqlite3Database{
	public:
		
		/**
		 * ��ȡ�����Ĳ�������
		 * \return sqlite3 ���������
		 * */
		virtual Sqlite3Table* takeTable(const char *tableName)
		{
			TABLES_ITER iter = _tables.find(tableName);
			if (iter != _tables.end())
			{
				return iter->second;
			}
			return NULL;
		}
		/**
		 * ����һ����
		 */
		virtual bool addTable(const char *tableName,Sqlite3Table * table)
		{
			TABLES_ITER iter = _tables.find(tableName);
			if (iter == _tables.end())
			{
				_tables[tableName] = table;
				return true;
			}
			return false;
		}
		
	private:
		std::map<std::string,Sqlite3Table*> _tables;
		typedef std::map<std::string,Sqlite3Table*>::iterator TABLES_ITER;
	};	
	/**
	 * table ����
	 */
	class TableObject:public Sqlite3Table,public vCoreData{
	public:
	};

}; // end namespace data

#define BIND_VALUE(dname,dvalue){\
	static std::string sname = (dname);\
	if (sname == name)\
	{\
		bind((dvalue),bind_arg);\
		return true;\
	}\
}

#define BIND_STRING(dname,dvalue,dlen){\
	static std::string sname = (dname);\
	if (sname == name)\
	{\
		bind((dvalue), (dlen),bind_arg);\
		return true;\
	}\
}
#define DEC_BIND_FUNC bool genBind(std::string &name,sqlite3_stmt *bind_arg);

#define IMP_BIND_FUNC_START(__class__) bool __class__::genBind(std::string &name,sqlite3_stmt *bind_arg)

