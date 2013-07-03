#pragma once
extern "C"{
#include "sqlite3.h"
}
#include "map"
#include <vector>
#include <string>
#include <sys.h>
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
	class sqlite3DBM;
	struct sqliteOPB{
		/**
 		 * �������ݿ�������
 		 * <code> ʵ��vOPData::setDBM</code>
 		 * \param dbm ��ǰ���ݹ������
 		 * */
		virtual void setDBM(sqlite3DBM *dbm){this->dbm = dbm;}
		sqlite3DBM *dbm; // ���ݿ�������
		vCoreData *coreData; // ��ʱ���ݶ���
		sqliteOPB(){dbm = NULL;}
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
		virtual void checkValue(std::string & name,unsigned long len) = 0;

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
 	 * ���ݿ��������
 	 * */
	template<class TAOPB>
	class OPBFactory{
	public:
		/**
 		 * ���������
 		 * */
		static OPBFactory &getMe()
		{
			static OPBFactory *opb = new TAOPB();
			return *opb;
		}
		/*
 		 * ��ȡsqlite �������� һ���Ӧ�ڱ����
 		 * \param tableName ���ݿ��
 		 * \return ���������
 		 * **/
		virtual sqliteOPB * getOPB(const char * tableName) {return NULL;}
	};
	/**
 	 * sqlite���ݿ����
 	 * */
	class sqlite3DBM{
	public:
		sqlite3DBM()
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
		virtual sqliteOPB* takeTable(const char * tableName) {return NULL;};
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
	protected:
	};
	
	template <typename T>
	class tSqlite3DBM:public sqlite3DBM{
	public:
		/**
		 * ��ȡ�����Ĳ�������
		 * \return sqlite3 ���������
		 * */
		virtual sqliteOPB* takeTable(const char *tableName)
		{
			return T::getMe().getOPB(tableName);
		}
	};	
}; // end namespace data
