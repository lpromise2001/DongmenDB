//
// Created by Sam on 2018/1/26.
//

#ifndef DONGMENDB_INTEGRITYMANAGER_H
#define DONGMENDB_INTEGRITYMANAGER_H

#include "Recordfile.h"


/**
 * 完整性管理
 *  完整性管理：实体完整性，参照完整性，自定义完整性（check，null等）
 *  数据保存在元数据表中。
 */
typedef enum{
    SQL_UPDATE,
    SQL_INSERT,
    SQL_DELETE
}sql_update_type;

typedef struct record_value_ record_value;

class IntegrityManager {
    const char *message;

    int integrity_manager_entity_constraint_check(table_info *tableInfo, record_value *recordValue, sql_update_type sqlUpdateType);
    int integrity_manager_reference_constraint_check(table_info *tableInfo, record_value *recordValue, sql_update_type sqlUpdateType);
    int integrity_manager_check_constraint_check(table_info *tableInfo, record_value *recordValue);
    int integrity_manager_null_constraint_check(table_info *tableInfo, record_value *recordValue);

} ;




#endif //DONGMENDB_INTEGRITYMANAGER_H