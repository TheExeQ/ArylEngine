#include "Host.h"

#include "Client.h"
#include "Server.h"
#include "Aryl/Core/Application.h"
#include "Aryl/Network/Socket/UdpSocketBuilder.h"

namespace Aryl
{
    Host::Host(HostSettings hostSettings, const std::function<void(NetPacket)>& handleMessageDelegate)
    {
        if (!mySender)
        {
            Ref<UdpSocketBuilder> builder = UdpSocketBuilder::Create(Application::Get().GetNetworkContext());
            builder->BoundToAddress(IPv4Address("0.0.0.0"));
            builder->BoundToPort(0);

            auto socket = builder->Build();

            if (socket)
            {
                mySender = UdpSocketSender::Create(socket);
            }
        }

        if (!myReceiver)
        {
            Ref<UdpSocketBuilder> builder = UdpSocketBuilder::Create(Application::Get().GetNetworkContext());

            IPv4Endpoint hostEndpoint = IPv4Endpoint(IPv4Address("0.0.0.0"), hostSettings.preferredPort);

            builder->BoundToAddress(hostEndpoint.GetAddress());
            builder->BoundToPort(hostEndpoint.GetPort());

            auto socket = builder->Build();

            if (socket)
            {
                myReceiver = UdpSocketReceiver::Create(socket, handleMessageDelegate);
                YL_CORE_TRACE("Starting UDP receiver on {0}:{1}", hostEndpoint.GetAddress().ToString(),
                              hostEndpoint.GetPort());
            }
        }
    }

    Host::~Host()
    {
        if (myReceiver)
        {
            myReceiver->Stop();

            if (mySender)
            {
                Ref<NetPacket> packet = CreateRef<NetPacket>();
                packet->header.messageType = NetMessageType::Disconnect;

                SendMessage(packet);
            }

            myReceiver = nullptr;
        }

        if (mySender)
        {
            mySender->Stop();
            mySender = nullptr;
        }
    }

    bool Host::SendMessage(const Ref<NetPacket>& packet)
    {
        if (mySender && myConnectionsMap.find(myEndpoint.ToString()) != myConnectionsMap.end())
        {
            NetConnection& connection = myConnectionsMap.at(myEndpoint.ToString());
            packet->header.id = connection.sendId;
            
            if (mySender->Send(packet, myConnectionsMap.at(myEndpoint.ToString()).listenEndpoint))
            {
                connection.sendId++;
            }
        }
        return false;
    }

    void Host::Connect(const IPv4Endpoint& endpoint)
    {
        myEndpoint = endpoint;
    }

    void Host::HandleMessage(NetPacket& packet)
    {
        // TESTING
        if (packet.header.messageType == NetMessageType::Connect)
        {
            YL_CORE_TRACE("New connection from {0} (ID: {1})", packet.endpoint.ToString(), packet.header.id);
        }
        if (packet.header.messageType == NetMessageType::Disconnect)
        {
            YL_CORE_TRACE("{0} disconnected (ID: {1})", packet.endpoint.ToString(), packet.header.id);
        }
        if (packet.header.messageType == NetMessageType::StringMessage)
        {
            std::string str((const char*)packet.data.data());
            YL_CORE_TRACE(str + " from {0} (ID: {1})", packet.endpoint.ToString(), packet.header.id);
        }
    }
}
