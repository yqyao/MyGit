//example for mysql++ user
//create at 2017.7.26
//author: yyq

#include <memory>
#include <iostream>
#include <fstream>
#include <vector>

# include "unistd.h"

#include "MysqlConnectPool.h"

std::shared_ptr<MysqlConnectPool> db_pool = nullptr;
std::string db_name = "search";
std::string db_ip = "localhost";
std::string db_user = "root";
std::string db_passwd = "DeepLearning";
int db_port = 3306;
std::string db_charset = "utf8";
int max_size = 10;
int BUFF_SIZE = 1024;
char buff[1024];


char* createTableFormat = "create table %s ;";
char* checkTableFormat = "show tables like \"%s\" ;";
char* insertFormat = "insert ignore into %s(id, name, status) values(%d, \"%s\", \"%s\");";
char* deleteFormat = "delete from Student where id = %d;";
char* updateFormat = "update Student set name = \"%s\", status = \"%s\" where id = %d;";
char* searchFormat = "select name,id, status from Student where status = \"%s\" ;";

std::string getCreateString()
{
    memset(buff, 0, BUFF_SIZE);
    char* temp = "Student(id int, name varchar(20), status varchar(10))";
    snprintf(buff, sizeof(buff), createTableFormat, temp);
    std::string buffAsStdStr = buff;
    return buffAsStdStr;
}

std::string getCheckString()
{
    memset(buff, 0, BUFF_SIZE);
    char* temp = "Student";
    snprintf(buff, sizeof(buff), checkTableFormat, temp);
    std::string buffAsStdStr = buff;
    return buffAsStdStr;
}

std::string getInsertString()
{
    memset(buff, 0, BUFF_SIZE);
    char* tableName = "Student";
    int id = 3;
    char* name = "wang";
    char* status = "true";
    snprintf(buff, sizeof(buff), insertFormat, tableName, id, name, status);
    std::string buffAsStdStr = buff;
    return buffAsStdStr;
}

std::string getDeleteString()
{
    memset(buff, 0, BUFF_SIZE);
    int id = 3;
    snprintf(buff, sizeof(buff), deleteFormat, id);
    std::string buffAsStdStr = buff;
    return buffAsStdStr;
}

std::string getUpdateString()
{
    memset(buff, 0, BUFF_SIZE);
    int id = 3;
    char* name = "wang";
    char* status = "false";
    snprintf(buff, sizeof(buff), updateFormat, name, status, id);
    std::string buffAsStdStr = buff;
    return buffAsStdStr;
}

std::string getSearchString()
{
    memset(buff, 0, BUFF_SIZE);
    char* status = "false";
    snprintf(buff, sizeof(buff), searchFormat, status);
    std::string buffAsStdStr = buff;
    return buffAsStdStr;
}

void createTable(std::string sql)
{
    mysqlpp::ScopedConnection conn(*db_pool, false);
    mysqlpp::Query query = conn->query();
    query << sql;
    mysqlpp::SimpleResult res = query.execute();
    std::cout << res.info() << std::endl;
}

int checkTableExist(std::string sql)
{
    mysqlpp::ScopedConnection conn(*db_pool, false);
    mysqlpp::Query query = conn->query();
    query << sql;
    if(query.exec())
        return 1;
    else  return 0;
}

void insert(std::string sql)
{
    mysqlpp::ScopedConnection conn(*db_pool, false);
    mysqlpp::Query query = conn->query();
    query << sql;
    mysqlpp::SimpleResult res = query.execute();
    std::cout << "Inserted into Student table, ID =" << res.insert_id() << std::endl;
}

void deleteFrom(std::string sql)
{
    mysqlpp::ScopedConnection conn(*db_pool, false);
    mysqlpp::Query query = conn->query();
    query << sql;
    if (query.exec()) {
        std::cout << "deleted success!" << std::endl;
    }
}

void update(std::string sql)
{
    mysqlpp::ScopedConnection conn(*db_pool, false);
    mysqlpp::Query query = conn->query();
    query << sql;
    if (query.exec()) {
        std::cout << "updated success!" << std::endl;
    }
}

void search(std::string sql)
{
    mysqlpp::ScopedConnection conn(*db_pool, false);
    mysqlpp::Query query = conn->query();
    query << sql;
    mysqlpp::StoreQueryResult ares = query.store();
    std::cout << "ares.num_rows() = " << ares.num_rows() << std::endl;
    for (size_t i = 0; i < ares.num_rows(); i++)
    {
        std::cout << "your data" << std::endl;
        std::cout << "name: " << ares[i]["name"] << "\tid: " << ares[i]["id"] << "\tstatus: " << ares[i]["status"] << std::endl;
    }
}

int main()
{
    //get the connection pool
    db_pool = std::make_shared<MysqlConnectPool>(db_name, db_ip, db_user, db_passwd, db_port, db_charset, max_size);
    std::string createTablesql = getCreateString();
    std::cout << "createTableFormat: " << createTablesql << std::endl;
    std::string checkTableSql = getCheckString();
    std::cout << "checkTableFormat: " << checkTableSql << std::endl;
    std::string insertSql = getInsertString();
    std::cout << "insertFormat: " << insertSql << std::endl;
    std::string deleteSql = getDeleteString();
    std::cout << "deleteFormat: " << deleteSql << std::endl;
    std::string updateSql = getUpdateString();
    std::cout << "updateFormat: " << updateSql << std::endl;
    std::string searchSql = getSearchString();
    std::cout << "searchFormat: " << searchSql << std::endl;
    // int i = checkTableExist(checkTableSql);
    std::cout << "checkstatus: " << checkTableExist(checkTableSql) << std::endl;
    // insert(insertSql);
    // deleteFrom(deleteSql);
    // update(updateSql);
    // search(searchSql);
    return 0;
}