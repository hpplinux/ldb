// ldb_server.cc (2014-12-23)
// WangPeng (1245268612@qq.com)

#include <stdio.h>
#include <errno.h>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

#include "server.h"
#include "string_type.h"

Server::Server(): daemonize_(false), configure_(NULL), logfile_(NULL)
{
    options_.create_if_missing = true;

    leveldb::Status status 
        = leveldb::DB::Open(options_, "/tmp/test_ldb", &db_);
    assert(status.ok());
}

Server::~Server()
{
    configure_ = NULL;
    logfile_ = NULL;
}

int Server::Insert(const leveldb::Slice& key, const leveldb::Slice& value)
{
    leveldb::Status status;
    status = db_->Put(write_options_, key, value);
    if (status.ok())
        return 0;
    else 
        return -1;
}

int Server::Get(const leveldb::Slice& key, std::string* value)
{
    leveldb::Status status;
    status = db_->Get(read_options_, key, value);
    if (status.ok())
        return 0;
    else 
        return -1;
}

int Server::Delete(const leveldb::Slice& key)
{
    leveldb::Status status;
    status = db_->Delete(write_options_, key);
    if (status.ok())
        return 0;
    else 
        return -1;
}

void Server::AddCommand(Command &com)
{
   commands_.push_back(com);
}

void Server::AddClient(Client *cli)
{
    clients_.push_back(cli);
}

void Server::DeleteClient(int fd)
{
    std::vector<Client *>::iterator i;
    for (i = clients_.begin(); i != clients_.end(); i++)
    {
        if ((*i)->link_->getFd() == fd) {
            clients_.erase(i);
            break;
        }
    }
}

Client *Server::FindClient(int fd)
{
    std::vector<Client *>::iterator i;
    for (i = clients_.begin(); i != clients_.end(); i++)
    {
        if ((*i)->link_->getFd() == fd) {
            return (*i);
        }
    }
}

Command *Server::FindCommand(char *name)
{
    std::vector<Command>::iterator i;
    for (i = commands_.begin(); i != commands_.end(); i++)
    {
        if (strcmp(i->name, name) == 0 ) {
            return &(*i);
        }
    }

    return NULL;
}

void Server::CreateComTable()
{
    Command ldb_commands_table[] = {
        {"set", ldb_set_command, 3, "w"},
        {"get", ldb_get_command, 2, "r"},
        {"del", ldb_del_command, 2, "w"}/*,
        {"update", ldb_update_command, 3, "w"},
        {"lookall", ldb_lookall_command, 1, "r"},
        {"clear", ldb_clear_command, 1, "w"},
        {"select", ldb_select_command, 2, "w"}*/
    };  
    
    AddCommand(ldb_commands_table[0]);
    AddCommand(ldb_commands_table[1]);
    AddCommand(ldb_commands_table[2]);
#if 0
    AddCommand(ldb_commands_table[3]);
    AddCommand(ldb_commands_table[4]);
    AddCommand(ldb_commands_table[5]);
    AddCommand(ldb_commands_table[6]);
#endif

}

void Server::Run()
{
    CreateComTable();                                          

    socket_.setNoblock();
    socket_.setNoNagle();

    int backlog = 512;
    socket_.Listen("0.0.0.0", 8899, backlog);

    event_.addReadEvent(socket_.getFd());

    //if ( daemonize_ ) ldb_daemon();
}
