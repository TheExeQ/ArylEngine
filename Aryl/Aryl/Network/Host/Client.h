#pragma once
#include "Host.h"

namespace Aryl
{
    class Client : public Host
    {
    public:
        Client(HostSettings hostSettings);
        ~Client();

        void Connect(const IPv4Endpoint& endpoint) override;
        void PingServer();
        
    protected:
        void HandleMessage(NetPacket& packet) override;
        static void ProcessDeletions(std::unordered_map<uint32_t, uint32_t>& map, std::vector<uint32_t>& vec);

    private:
        std::unordered_map<uint32_t, uint32_t> myServerToClientEntityMap;
        std::vector<uint32_t> myQueuedDelete;
    };
}
