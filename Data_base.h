#include <sqlite3.h>
#include <string>
#include <iostream>

using namespace std;

int quantity_temp;
double total_price_temp;

int cmd_exec(string cmd, int (*callback)(void*, int, char**, char**));
int Init();
int Instert(istream& is);
int Select(string ISBN);
int Updata(string ISBN, string Quantity, string Unit_price, string Total_price);
int Exist(string ISBN);

int cmd_exec(string cmd, int (*callback)(void*, int, char**, char**))
{
    sqlite3* db;
    int rc = sqlite3_open("Bookstore_Cashier_Database.db", &db);  // 打开或创建数据库文件

    if (rc != SQLITE_OK)
    {
        cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
        return 1;
    }

    char* errorMsg;
    char queryBuffer[1000];
    strcpy_s(queryBuffer, cmd.c_str());

    rc = sqlite3_exec(db, queryBuffer, callback, nullptr, &errorMsg);

    if (rc != SQLITE_OK)
    {
        cerr << "SQL error: " << errorMsg << endl;
        sqlite3_free(errorMsg);
        sqlite3_close(db);
        return 1;
    }

    sqlite3_close(db);

    return 0;
}

int Init()
{
    string cmd = "CREATE TABLE Sales_data(ISBN Text, Quantity INT, Unit_price BLOB, Total_price BLOB);";
    if (cmd_exec(cmd, nullptr))
        return 1;
    else
        return 0;
}

int Instert(string ISBN,int quantity,double unit_price)
{
    double total_price;

    total_price = quantity * unit_price;

    if (!Exist(ISBN))
    {
        string cmd = "INSERT INTO Sales_data (ISBN, Quantity, Unit_price, Total_price)" + string(" VALUES ") + string("(") + string("'") + ISBN + string("'") + string(",") + to_string(quantity) + string(",") + to_string(unit_price) + string(",") + to_string(total_price) + string(");");
        //cout << cmd << endl;
        if (cmd_exec(cmd, nullptr))
            return 1;
        else
            return 0;
    }
    else
    {
        Select(ISBN);
        total_price += total_price_temp;
        quantity += quantity_temp;
        unit_price = total_price / quantity;
        Updata(ISBN, to_string(quantity), to_string(unit_price), to_string(total_price));
        return 1;
    }
}

int callback_select(void* data, int argc, char** argv, char** azColName) {
    // 回调函数用于处理查询结果
    for (int i = 0; i < argc; i++) {
        //cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << endl;
    }

    quantity_temp = stoi(argv[1]);
    total_price_temp = stod(argv[3]);
    //cout << quantity_temp << "  " << total_price_temp;
    return 0;
}

int Select(string ISBN)
{

    string cmd = "SELECT * FROM Sales_data WHERE ISBN = "+ISBN;
    if (cmd_exec(cmd, callback_select))
        return 1;
    else
        return 0;
}

int Updata(string ISBN,string Quantity,string Unit_price,string Total_price)
{
    string cmd = "UPDATE Sales_data SET Quantity = "+ Quantity+", Unit_price = "+ Unit_price+", Total_price = "+ Total_price+" WHERE ISBN = '"+ISBN+"'";
    //string cmd = "UPDATE Sales_data SET Quantity = 8, Unit_price = 45, Total_price = 360 WHERE ISBN = '123'"; // 替换为你的更新语句和条件
    //cout << cmd << endl;
    if (cmd_exec(cmd, nullptr))
        return 1;
    else
        return 0;
}

int Exist(string ISBN)
{
    sqlite3* db;
    int flag;
    int rc = sqlite3_open("Bookstore_Cashier_Database.db", &db); // 打开数据库文件 example.db
    if (rc != SQLITE_OK) {
        std::cout << "无法打开数据库: " << sqlite3_errmsg(db) << std::endl;
    }

    string cmd = "SELECT EXISTS(SELECT 1 FROM Sales_data WHERE ISBN = '" + ISBN + "' LIMIT 1);";
    //cout << cmd << endl;
    char queryBuffer[1000];
    strcpy_s(queryBuffer, cmd.c_str());
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, queryBuffer, -1, &stmt, nullptr); // 准备查询语句
    if (rc != SQLITE_OK) {
        std::cout << "查询准备错误: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
    }

    rc = sqlite3_step(stmt); // 执行查询
    if (rc == SQLITE_ROW) {
        int result = sqlite3_column_int(stmt, 0); // 获取查询结果
        if (result == 1) {
            //cout << "exist" << endl;
            flag = 1;       //数据存在
        }
        else {
            //cout << "no exist" << endl;
            flag = 0;       //数据不存在
        }
    }
    else {
        std::cout << "查询执行错误: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt); // 释放查询语句资源
    sqlite3_close(db); // 关闭数据库连接

    if (flag == 1)
        return 1;
    else
        return 0;
}


int callback_PrintData(void* data, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }
    std::cout << std::endl;

    return 0;
}

int PrintData() {

    string cmd = "SELECT * FROM Sales_data"; // 替换为你的表名
    if (cmd_exec(cmd, callback_PrintData))
        return 1;
    else
        return 0;
}