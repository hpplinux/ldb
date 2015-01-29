// ldb_server.h (2014-12-23)
// WangPeng (1245268612@qq.com)

#ifndef _LDB_SERVER_H_
#define _LDB_SERVER_H_

#include <string>
#include <iostream>
#include <vector>
#include <leveldb/db.h>
#include <leveldb/slice.h>

#include <net/event.h>
#include <net/socket.h>
#include <util/config.h>

class Client;
class Acceptor;
class Slave;

class Server {
public:
    Server();
    ~Server();

    int Insert(const leveldb::Slice& key, const leveldb::Slice& value);
    int Get(const leveldb::Slice& key, std::string* value);
    int Delete(const leveldb::Slice& key);

    int Run(const char *config_file);
    
    Client *FindClient(int fd);

private:
    int ProcessEvent();
    void ProcessReadEvent();
    void ProcessWriteEvent();
    void ProcessTimeEvent();
    void DeleteClient(Client *c);

    void AddClient(Client *cli);
    void DeleteClient(int fd);

    void ConnectSlave();

    int CreateServer();

private:
    leveldb::Options options_;
    leveldb::WriteOptions write_options_;
    leveldb::ReadOptions read_options_;
    leveldb::DB *db_;
    
    std::vector<Client *> clients_;
    
    int time_out_;

public:
    Epoll event_;
    Event fired_read_[1024];
    Event fired_write_[1024];
    
    std::vector<TimeEvent> time_event_;

    Acceptor *socket_;
    
    Socket *slave1_;
    Socket *slave2_;
    
    Slave *slave_1_;

    Config config_;

    bool server_can_write_;

};


#endif
