
#include "coevent.h"
#include "coevent_itnl.h"
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>

using namespace andrewmc::libcoevent;

typedef Event _super;

// ==========
// public functions
#define __PUBLIC_FUNCTIONS
#ifdef __PUBLIC_FUNCTIONS

UDPClient *Server::new_UDP_client(NetType_t network_type, void *user_arg)
{
    UDPItnlClient *client = new UDPItnlClient;
    client->init(this, _coroutine(), network_type, user_arg);
    _client_chain.insert(client);
    return (UDPClient *)client;
}


struct Error Server::delete_client(UDPClient *client)
{
    if (NULL == client) {
        _status.set_app_errno(ERR_PARA_NULL);
        return _status;
    }

    std::set<Client *>::iterator it = _client_chain.find(client);
    if (it != _client_chain.end())
    {
        _status.clear_err();
        DEBUG("Delete client '%s' from '%s'", client->identifier().c_str(), _identifier.c_str());
        owner()->delete_event_under_control(*it);
        _client_chain.erase(client);
    }
    else {
        DEBUG("Client '%s' is not in '%s'", client->identifier().c_str(), _identifier.c_str());
        _status.set_app_errno(ERR_OBJ_NOT_FOUND);
    }

    return _status;
}


Server::~Server()
{
    DEBUG("Delete Server client chain of %s", _identifier.c_str());

    // free all clients under control
    for (std::set<Client *>::iterator it = _client_chain.begin(); 
        it != _client_chain.end(); 
        it ++)
    {
        owner()->delete_event_under_control(*it);
    }
    _client_chain.clear();

    return;
}


#endif


// end of file