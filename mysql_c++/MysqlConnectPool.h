#ifndef _MYSQL_CONNECT_POOL_H_
#define _MYSQL_CONNECT_POOL_H_

#include <mysql++.h>
#include <string>
#include "unistd.h"

class MysqlConnectPool : public mysqlpp::ConnectionPool
{
public:
    MysqlConnectPool(std::string dbname,
                    std::string serverip,
                    std::string user,
                    std::string passwd,
                    int port,
                    std::string charset,
                    int max_size)
        :m_dbname(dbname)
        ,m_server_ip(serverip)
        ,m_user(user)
        ,m_password(passwd)
        ,m_charset(charset)
        ,m_port(port)
    {
        m_max_size = max_size;
        conns_in_use_ = 0;
        m_max_idle_time = 300;
    }

    virtual ~MysqlConnectPool()
    {
        std::cout << "bye" << std::endl;
        clear();
    }

    int getSize()
    {
        return mysqlpp::ConnectionPool::size();
    }

    const std::string& getDBName() const { return m_dbname; }
    const std::string& getServerIP() const { return m_server_ip; }
    const std::string& getUser() const { return m_user; }
    const std::string& getPassword() const { return m_password; }
    const std::string& getCharset() const { return m_charset; }
    const int getConnUse() const { return conns_in_use_;}
    int getPort() const { return m_port; }

    void setMaxIdleTime(int max_idle)
    {
        m_max_idle_time = max_idle;
    }

    virtual mysqlpp::Connection* grab()
    {   
        while(conns_in_use_ > m_max_size) {
            std::cout << "wait conn release" << std::endl;
            sleep(1);
        }
        ++conns_in_use_;
        return mysqlpp::ConnectionPool::grab();
    }

    void remove(const mysqlpp::Connection* pc)
    {
        mysqlpp::ConnectionPool::remove(pc);
    }

    virtual void release(const mysqlpp::Connection* pc)
    {
        --conns_in_use_;
        mysqlpp::ConnectionPool::release(pc);
    }

protected:
    virtual mysqlpp::Connection* create()
    {
        mysqlpp::Connection* conn = new mysqlpp::Connection(true);
        mysqlpp::SetCharsetNameOption* pOpt = new  mysqlpp::SetCharsetNameOption(m_charset.c_str());
        conn->set_option( pOpt );

        conn->connect(m_dbname.empty() ? 0 : m_dbname.c_str(),
                        m_server_ip.empty() ? 0 : m_server_ip.c_str(),
                        m_user.empty() ? 0 : m_user.c_str(),
                        m_password.empty() ? "" : m_password.c_str(),
                        m_port);
        std::cout << "create" << std::endl; 
        return conn;
    }

    virtual void destroy(mysqlpp::Connection* cp)
    {   
        std::cout << "destroy" << std::endl;
        delete cp;
    }

    virtual unsigned int max_idle_time()
    {
        return m_max_idle_time;
    }

private:
    std::string m_dbname;
    std::string m_server_ip;
    std::string m_user;
    std::string m_password;
    std::string m_charset;
    int         m_port;
    int         m_max_idle_time;
    unsigned int conns_in_use_;
    int m_max_size;
};

#endif