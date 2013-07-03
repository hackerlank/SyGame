/**
 * email:ji.jin.dragon@gmail.com
 * email:jjl_2009_hi@163.com
 * Author jijinlong
 * Data 2011.10.14~2012.3.18~~
 * All Rights Resvered
 ***/

#pragma once
/*
 * ��ϵ�����ݿ� һ����ӳ�� ���ڴ���
 ***/
#include "vector"
#include "sys.h"
#include "type.h"
#define BYTES std::vector<char>
#include "stdio.h"
#include "string.h"
#include "sString.h"
#define BIND_VALUE(dname,dvalue){\
	static sys::sString sname = (dname);\
	if (sname == field.name)\
	{\
		bind((dvalue),bind_arg);\
		LOG_DEBUG("bind:%s value:%u",sname.str(),dvalue);\
		return true;\
	}\
}

#define BIND_STRING(dname,dvalue,dlen){\
	static sys::sString sname = (dname);\
	if (sname == field.name)\
	{\
		bind((dvalue), (dlen),bind_arg);\
		LOG_DEBUG("bind:%s len:%u",sname.str(),(dlen));\
		return true;\
	}\
}
//#define DEC_BIND_FUNC bool getBind(sys::sString &name,sqlite3_stmt *pStmt);
#define DEC_BIND_FUNC bool genBind(data::vField &field,MYSQL_BIND *bind_arg);
//#define IMP_BIND_FUNC_START (__class__) bool __class__::genBind(sys::sString &name,sqlite3_stmt *pStmt) {
#define IMP_BIND_FUNC_START(__class__) bool __class__::genBind(data::vField &field,MYSQL_BIND *bind_arg){

#define IMP_BIND_FUNC_END return false;}

namespace data{
	struct vRow;
	/**
	 * ���ݴ洢������̳�֮
	 **/
	struct vCoreData{
		DWORD type;  // ��������
		vCoreData()
		{
			type = 0;
		}
	};
	/*
 	 * �ֶ�ֵ
 	 ***/
	struct vValue{
		BYTES value;	// ֵ
		/**
		 * �ֶ�ֵ ����
		 * \param str ֵ������
		 **/
		vValue(const char* str)
		{
			if (str)
			{
				DWORD len = strlen(str);	
				value.resize(len+1);
				bcopy(str,&value[0],len);
			}
		}
		/**
		 * ��ֵ�� float �Ļ�ȡ
		 * \return true �ɹ�
		 **/
		bool get(float &value)
		{
			value = atof(&this->value[0]);
			return true;
		}
		/**
		 * ���ͻ�ֵ�Ļ�ȡ
		 * \return true �ɹ�
		 **/
		template<class T>
		bool get(T&  value)
		{
			value = atoi(&this->value[0]);
			return true;
		}
		/**
		 * ��ȡ���ַ��������ַ����Ļ�ȡ������
		 **/
		char *str()
		{
			return &value[0];
		}
		/**
		 * ���ָ�����ȵĻ���,��len �������ݣ�len Ϊʵ������
		 * \return �ɹ� 
		 **/
		bool get(char *bytes,DWORD& len)
		{
			if (len > value.size()) len = value.size();
			bcopy(&this->value[0],bytes,len);
			return true;
		}
		/**
		 * ��ȡ���� ������vector ��
		 * \return �ɹ�
		 **/
		bool get(BYTES& bytes)
		{
			bytes.resize(value.size());
			bcopy(&value[0],&bytes[0],value.size());
			return true;
		}
	};
	/**
	 * �ֶ�
	 **/
	struct vField{
		sys::sString name; // �ֶ���
		int type; // ����
		vField(int type,const char*name)
		{
			this->type = type;
			this->name = name;
		}
		vField()
		{
			type = 0; // ����Ϊ0
		}
	};
	
	/*
	 * vDBM ���ݿ����
	 **/
	class vDBM
	{
	public:
		/**
		 * ��������
		 * \param data ����ָ��
		 **/
		virtual void addObject(vCoreData *data) {};
		/**
		 * ��ʼ�� ʹ��localhost root 123456 ��½
		 * \return �ɹ� true ʧ��false
		 **/
		virtual bool init() {return false;};
		/**
		 * ��Դ�ͷ�
		 **/
		virtual bool release() {return false;};
		/**
		 * ��ʼ��
		 * \param dbname ���ݿ�����
		 * \param username �û�����
		 * \param pwd ����
		 * \return �ɹ� true ʧ��false
		 **/
		virtual bool init(const char *dbname,const char *username,const char *pwd){return false;}
		/**
		 * ��ʼ��
		 * \param dbname ���ݿ�����
		 * \param host ���ݿ��ַ
		 * \param username �û�����
		 * \param pwd ����
		 * \return �ɹ� true ʧ��false
		 **/
		virtual bool init(const char *host,const char * dbname,const char *username,const char *pwd){return false;}
	};
	/*
	 *	��������
	 **/
	struct vOPData{
		/**
		 * ��ʼ��Ĳ���
		 **/
		virtual void start() {};
		/**
		 * ���ݿ����ʱ �������
		 * \param field �ֶ�
		 * \param valeu �ֶ�ֵ
		 **/
		virtual void BeAttached(vField &field,vValue &value) {};
		/**
		 * ������Ĳ���
		 **/
		virtual void end() {};
		/**
		 * �������ݿ�������
		 * \param dbm ���ݿ�������
		 **/
		virtual void setDBM(vDBM *dbm) {};
		/**
		 * ����stmt
		 **/
		virtual void genStmt(){};
	};
	/*
 	* ÿ������
 	***/
	struct vRow{
		std::vector<vValue> values; // ���ݼ���
		vOPData *op; // ��������
		vRow()
		{
			op = 0;
		}
		/**
		 * ���ݿ��ֶ� ��ֵ�ĸ�ֵ
		 * \param fields �ֶμ���
		 **/
		virtual void BeAttached(std::vector<vField>& fields)
		{
			if (values.size() != fields.size()) return;
			for (DWORD i = 0; i < fields.size();i++)
			{
				if (op)
				{
					op->BeAttached(fields[i],values[i]);
				}
			}
		}
		/**
		 * �����ֶ�ֵ
		 * \param value �ֶ�ֵ
		 **/
		virtual void addValue(vValue &value){
			values.push_back(value);
		}
	};
	/**
 	 * ��
 	 ***/
	struct vTable{
		std::vector<vField> fields; // �ֶ�
		std::vector<vRow> rows;	 // �м���
		vOPData *op; // ��������
		const char *tableName;// ����
		vTable()
		{
			op = 0;
			tableName = 0;
		}
		/**
		 * ������ֵ
		 **/
		virtual bool attach()
		{
			for (int rowid = 0; rowid < rows.size();++rowid)
			{
				vRow &row  = rows[rowid];
				row.op = op;
				op->start();
				row.BeAttached(fields);
				op->end();
			}
            return true;
		}
		/**
		 * ����һ������
		 * \param row ����
		 **/
		virtual void addRow(vRow &row){
			rows.push_back(row);
		}
		/**
		 * ����һ���ֶ�
		 * \param filed �ֶ�
		 **/
		virtual void addField(vField &field){
			fields.push_back(field);
		}
	};
	/*
 	*  ���ݿ��ڴ����
 	***/
	struct vDatabase{
		std::vector<vTable> tables; // ����
		/**
		 * ���ӱ���
		 * \param vTable ��
		 **/
		void addTable(vTable &table)
		{
			tables.push_back(table);
		}
		bool release()
		{
			tables.clear();
			return true;
		}
	};
}; 
