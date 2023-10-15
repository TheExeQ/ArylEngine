#include "Host.h"

#include "Aryl/Core/Application.h"
#include "Aryl/Network/Socket/UdpSocketBuilder.h"

namespace Aryl
{
    Host::Host(HostSettings hostSettings)
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
                myReceiver = UdpSocketReceiver::Create(socket, YL_BIND_EVENT_FN(Host::HandleMessage));
                YL_CORE_TRACE("Starting UDP receiver on {0}:{1}", hostEndpoint.GetAddress().GetAddressString(),
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
                packet->header.id = NetMessageType::Disconnect;

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

    bool Host::SendMessage(Ref<NetPacket> packet)
    {
        YL_CORE_INFO("[Host] Send Message");
        if (mySender)
        {
            return mySender->Send(packet, Endpoint);
        }
        return false;
    }

    void Host::HandleMessage(NetPacket packet)
    {
        YL_CORE_INFO("[Host] Handle Incoming");

        // TESTING
        if (packet.header.id == Aryl::NetMessageType::StringMessage)
        {
            std::string str((const char*)packet.data.data());
            YL_CORE_TRACE(str + " from {0}:{1}", packet.endpoint.GetAddress().GetAddressString(), packet.endpoint.GetPort());
        }
    }
}
