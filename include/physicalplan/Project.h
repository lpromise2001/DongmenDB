//
// Created by sam on 2018/11/15.
//

#ifndef DONGMENDB_PROJECT_H
#define DONGMENDB_PROJECT_H

#include <physicalplan/Scan.h>


#ifdef __cplusplus

class Project : public Scan {
    public:
    arraylist *original_expr_list;//select语句中指定的原始表达式列表
    arraylist *expr_list; //将 * 等解析后得到的表达式列表，在prepare时处理
    arraylist *order_by; //order_by_expr的列表
    int distinct;
    arraylist *group_by; // expression列表

    Scan* scan;

    Project(Scan* scan) : scan(scan){};

    int beforeFirst();

    int next();

    int close();

    variant* getValueByIndex(int index);

    int getIntByIndex(int index);

    string getStringByIndex(int index);

    int getInt(string tableName, string fieldName);

    variant* getValue(string fieldName);

    string getString(string tableName, string fieldName);

    int hasField(string tableName, string fieldName);

    field_info *getField(string tableName, string fieldName);

    arraylist *getFieldsName(string tableName);

    int setInt(string tableName, string fieldName, int value);

    int setString(string tableName, string fieldName, string value);

    int deleteRecord();

    int insertRecord();

    int getRID(record_id *recordID);

    int moveTo(record_id *recordID);
};

#endif

#endif //DONGMENDB_PROJECT_H