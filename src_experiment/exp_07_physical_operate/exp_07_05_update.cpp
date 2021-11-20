//
// Created by sam on 2018/9/18.
//

#include <physicalplan/ExecutionPlan.h>
#include <physicalplan/TableScan.h>
#include <physicalplan/Select.h>
#include <physicalplan/Project.h>
#include <physicalplan/Join.h>

/*执行 update 语句的物理计划，返回修改的记录条数
 * 返回大于等于0的值，表示修改的记录条数；
 * 返回小于0的值，表示修改过程中出现错误。
 * */
/*TODO: plan_execute_update， update语句执行*/



int ExecutionPlan::executeUpdate(DongmenDB *db, sql_stmt_update *sqlStmtUpdate, Transaction *tx) {
    /*删除语句以select的物理操作为基础实现。
     * 1. 使用 sql_stmt_update 的条件参数，调用 physical_scan_select_create 创建select的物理计划并初始化;
     * 2. 执行 select 的物理计划，完成update操作
     * */
    int count = 0;
    Scan *scan = generateScan(db, sqlStmtUpdate->where, tx);
    scan->beforeFirst();
    char *tableName = sqlStmtUpdate->tableName;
    while (scan->next()) {
        for (size_t i = 0; i < sqlStmtUpdate->fields.size(); i++) {
            char *currentFieldName = sqlStmtUpdate->fields[i];
            variant *var = (variant *) calloc(1, sizeof(variant));
            enum data_type fieldType = scan->getField(tableName, currentFieldName)->type;
            Expression *expression = scan->evaluateExpression(sqlStmtUpdate->fieldsExpr[i], scan, var);

            if (var->type != fieldType) {
                printf("error!\n");
                return -1;
            } else if (var->type == DATA_TYPE_INT) {
                scan->setInt(tableName, currentFieldName, var->intValue);
            } else if (var->type == DATA_TYPE_CHAR) {
                scan->setString(tableName, currentFieldName, var->strValue);
            } else {
                printf("error!\n");
            }
        }
        count++;
    }
    scan->close();
    return count;
}
