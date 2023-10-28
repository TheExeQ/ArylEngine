#include "Client.h"

namespace Aryl
{
    Client::Client(HostSettings hostSettings)
        : Host(hostSettings, YL_BIND_EVENT_FN(Client::HandleMessage))
    {
    }

    Client::~Client()
    {
    }

    void Client::HandleMessage(const NetPacket& packet)
    {
        Host::HandleMessage(packet);
    }
}
