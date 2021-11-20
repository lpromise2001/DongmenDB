
#include <dongmensql/sqlstatement.h>
#include <parser/StatementParser.h>

/**
 * 在现有实现基础上，实现update from子句
 *
 * 支持的update语法：
 *
 * UPDATE <table_name> SET <field1> = <expr1>[, <field2 = <expr2>, ..]
 * WHERE <logical_expr>
 *
 * 解析获得 sql_stmt_update 结构
 */

// update student set sname = "tom" where sno = "001"


/*TODO: parse_sql_stmt_update， update语句解析*/
sql_stmt_update *UpdateParser::parse_sql_stmt_update() {
//fprintf(stderr, "TODO: update is not implemented yet. in parse_sql_stmt_update \n");

    char *tableName;    //保存表名
    vector<char *> fields; //set fields 被更新的字段列表,保存上面update语句的sage
    vector<Expression *> fieldsExpr;  //set fields expression 新值(或表达式)列表,保存上面update语句的20
    SRA_t *where;   //保存上面update语句的 sage=22
    //首先匹配关键词 update ，使用 int Parser::matchToken( TokenType type, char *text) 函数对 update 进行匹配。
    Token *token = this->parseNextToken();
    if (!this->matchToken(TOKEN_RESERVED_WORD, "update")) {
        return NULL;
    }

    //解析表名 tableName，判断是否为 TOKEN_WORD 类型，如果是，获得表名，并将表名包裹在 SRA_t 中（ SRA_TABLE 类型）作为 sql_stmt_update->where 的值。
    token = this->parseNextToken();
    if (token->type == TOKEN_WORD) {
        tableName = new_id_name();
        strcpy(tableName, token->text);
    } else {
        strcpy(this->parserMessage, "invalid sql: missing table name.");
        return NULL;
    }

    //匹配关键词set，使用 int Parser::matchToken( TokenType type, char *text) 函数对set进行匹配。
    token = this->parseEatAndNextToken();
    if (!this->matchToken(TOKEN_RESERVED_WORD, "set")) {
        strcpy(this->parserMessage, "invalid sql: missing table name..");
        return NULL;
    }

    // 循环获取更新的表达式，获取字段名（ TOKEN_WORD 类型），存入 fields 中；识别并跳过=（ TOKEN_EQ 类型），使用 Expression *Parser::parseExpressionRD()  函数获得新值或表达式,存入 fieldsExpr 中即可获得一个完整的表达式。
    token = this->parseNextToken();
    if (token != nullptr && (token->type == TOKEN_WORD || token->type == TOKEN_COMMA)) {
        while (token != nullptr && (token->type == TOKEN_WORD || token->type == TOKEN_COMMA)) {
            if (token->type == TOKEN_COMMA) {
                token = parseEatAndNextToken();
            }
            if (token->type == TOKEN_WORD) {
                char *fieldName = new_id_name();
                strcpy(fieldName, token->text);
                fields.push_back(fieldName);
            } else {
                strcpy(this->parserMessage, "error: file name. \n");
                return NULL;
            }
            this->parseEatAndNextToken();

            if (matchToken(TOKEN_EQ, "=") == 0) {
                strcpy(this->parserMessage, "error: '=' \n");
                return NULL;
            }
            // 匹配值
            Expression *exp = parseExpressionRD();
            fieldsExpr.push_back(exp);
            token = parseNextToken();
        }
    } else {
        strcpy(this->parserMessage, "invalid sql: missing field name.");
        return NULL;
    }

    //匹配 where 关键词，使用 int Parser::matchToken( TokenType type, char *text) 函数对set进行匹配，如果不存在则仅需构造 SRA_TABLE。
    //使用 Expression *Parser::parseExpressionRD() 函数获取值或表达式，并包裹在 SRA_t 中（ SRA_SELECT 类型），作为 sql_stmt_update->where 的值。
    where = SRATable(TableReference_make(tableName, nullptr));
    if (this->matchToken(TOKEN_RESERVED_WORD, "where")) {
        /*解析where子句中的条件表达式*/
        Expression *whereExpr = this->parseExpressionRD();
        if (this->parserStateType == PARSER_WRONG) {
            return NULL;
        }
        where = SRASelect(where, whereExpr);
    }
    //创建 sql_stmt_update 类型的指针，分配内存空间并对各字段进行赋值，返回该指针。
    sql_stmt_update *sqlStmtUpdate = (sql_stmt_update *) calloc(1, sizeof(sql_stmt_update));
    sqlStmtUpdate->tableName = tableName;
    sqlStmtUpdate->fields = fields;
    sqlStmtUpdate->fieldsExpr = fieldsExpr;
    sqlStmtUpdate->where = where;
    return sqlStmtUpdate;
};

