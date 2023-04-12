#include "Editor.h"

#include <Aryl/Core/Base.h>
#include <Aryl/Core/Application.h>

#include <Aryl/Components/Components.h>
#include <Aryl/Scene/Entity.h>
#include <Aryl/Scene/Scene.h>
#include <Aryl/Scene/SceneManager.h>

#include <Aryl/Events/KeyEvent.h>
#include <Aryl/Input/KeyCodes.h>

#include <ImGui.h>
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <entt/entt.hpp>

#include <Aryl/Networking/Socket/UdpSocketBuilder.h>

Editor::Editor()
{
	YL_ASSERT(!myInstance, "Editor already exists!");
	myInstance = this;
}

Editor::~Editor()
{
	myInstance = nullptr;
}

void Editor::OnAttach()
{
	// U3
	{
		uint32_t port;
		std::cout << "Enter Receiver Port: \n";
		while (!(std::cin >> port))
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cerr << "Invalid input. Please enter a valid number: ";
		}

		SetupSender(Aryl::IPv4Endpoint(Aryl::IPv4Address("0.0.0.0"), 0));
		SetupReceiver(Aryl::IPv4Endpoint(Aryl::IPv4Address("0.0.0.0"), port), [this](Aryl::NetPacket packet) {

			std::string id = packet.endpoint.GetAddress().GetAddressString() + ":" + std::to_string(packet.endpoint.GetPort());
			
			if (Aryl::Application::Get().IsHeadless())
			{
				if (packet.header.id == Aryl::NetMessageType::Connect)
				{
					uint32_t receiverPort = 0;
					packet >> receiverPort;
					myConnectedClients[id] = Aryl::IPv4Endpoint(packet.endpoint.GetAddress(), receiverPort);
					YL_CORE_INFO("{0} has connected with receiver port: {1}", id, receiverPort);
				}

				if (packet.header.id == Aryl::NetMessageType::Disconnect)
				{
					myConnectedClients.erase(id);
					YL_CORE_INFO("{0} has disconnected", id);
				}
			}

			if (packet.header.id == Aryl::NetMessageType::StringMessage)
			{
				std::string str((const char*)packet.data.data());
				myChatMessages.emplace_back(str);

				if (Aryl::Application::Get().IsHeadless())
				{
					std::cout << id << " sent " << str << std::endl;

					if (mySender)
					{
						for (const auto& client : myConnectedClients)
						{
							Ref<Aryl::NetPacket> newPacket = CreateRef<Aryl::NetPacket>();

							newPacket->header.id = Aryl::NetMessageType::StringMessage;
							newPacket->data = packet.data;

							mySender->Send(newPacket, client.second);
						}
					}
				}
			}
		});
	}

	// ENTT REFLECTION TESTING
	if (false)
	{
		auto& registry = Aryl::SceneManager::GetActiveScene()->GetRegistry();
		myTestingEntity = Aryl::Entity(registry.create(), Aryl::SceneManager::GetActiveScene().get());
		registry.emplace<my_type>(myTestingEntity.GetId());

		YL_REFLECT(my_type, value);
		YL_REFLECT(my_type, value2);
		YL_REFLECT(my_type, value3);

		auto& comp = myTestingEntity.GetComponent<my_type>();

		YL_CORE_TRACE(comp.value);
		YL_CORE_TRACE(comp.value2);
		YL_CORE_TRACE(comp.value3);

		auto type = entt::resolve(entt::hashed_string("my_type"));
		if (type)
		{
			auto data = type.data(entt::hashed_string("value"));
			auto data2 = type.data(entt::hashed_string("value2"));
			auto data3 = type.data(entt::hashed_string("value3"));

			auto type_id = entt::type_id<my_type>().hash();
			auto storage = registry.storage(type_id);

			YL_CORE_TRACE("TypeId: {0}", type_id);

			if (storage)
			{
				auto compontentPtr = storage->get(myTestingEntity.GetId());
				auto componentAny = type.from_void(compontentPtr);

				if (data) { data.set(componentAny, 5.f); }
				if (data2) { data2.set(componentAny, true); }
				if (data3) { data3.set(componentAny, 69.f); }
			}
		}

		YL_CORE_TRACE(comp.value);
		YL_CORE_TRACE(comp.value2);
		YL_CORE_TRACE(comp.value3);
	}
}

void Editor::OnDetach()
{
	ChatDisconnect();
}

void Editor::OnEvent(Aryl::Event& e)
{
	Aryl::EventDispatcher dispatcher(e);

	if (Aryl::Application::Get().IsHeadless())
	{
		//ChatAppHeadless();
	}

	dispatcher.Dispatch<Aryl::AppRenderEvent>(YL_BIND_EVENT_FN(Editor::OnRender));
	dispatcher.Dispatch<Aryl::AppImGuiUpdateEvent>(YL_BIND_EVENT_FN(Editor::OnImGuiUpdate));
}

bool Editor::OnRender(Aryl::AppRenderEvent& e)
{
	TempOpenGLRender();

	return false;
}

bool Editor::OnImGuiUpdate(Aryl::AppImGuiUpdateEvent& e)
{
	//ArylNetExample();
	ChatApp();

	return false;
}

void Editor::TempOpenGLRender()
{
	static ImVec4 clear_color = ImVec4(0.2f, 0.2f, 0.20f, 1.00f);
	int display_w, display_h;
	glfwGetFramebufferSize(Aryl::Application::Get().GetWindow().GetNativeWindow(), &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
}

char g_sendAddress[16] = "127.0.0.1";
int g_sendPort = 44000;

void Editor::ChatApp()
{
	static bool connected = false;
	static char messageBuffer[50] = "";

	ImGui::Begin("Chat");
	ImGui::InputText("Server Address", g_sendAddress, sizeof(g_sendAddress));
	ImGui::InputInt("Server Port", &g_sendPort);

	if (ImGui::Button("Connect"))
	{
		if (mySender)
		{
			Ref<Aryl::NetPacket> packet = CreateRef<Aryl::NetPacket>();

			packet->header.id = Aryl::NetMessageType::Connect;
			*packet << myReceiver->GetSocket()->GetEndpoint().GetPort();

			mySender->Send(packet, Aryl::IPv4Endpoint(Aryl::IPv4Address(g_sendAddress), g_sendPort));
			connected = true;
		}
	}
	if (connected)
	{
		ImGui::InputText("Message", messageBuffer, sizeof(messageBuffer));
		if (ImGui::Button("Send"))
		{
			if (mySender)
			{
				Ref<Aryl::NetPacket> packet = CreateRef<Aryl::NetPacket>();

				packet->header.id = Aryl::NetMessageType::StringMessage;

				packet->data.resize(sizeof(messageBuffer) + 1);
				std::memcpy(packet->data.data(), messageBuffer, packet->data.size());

				mySender->Send(packet, Aryl::IPv4Endpoint(Aryl::IPv4Address(g_sendAddress), g_sendPort));
			}
		}
		for (const auto& msg : myChatMessages)
		{
			ImGui::Text(msg.c_str());
		}
	}
	ImGui::End();
}

void Editor::ChatAppHeadless()
{
	static bool isSendAddressSet = false;

	static char sendAddress[16] = "127.0.0.1";
	static int sendPort = 44000;

	static char messageBuffer[50] = "";

	if (!isSendAddressSet)
	{
		// Input sendAddress
		std::cout << "Input Send Address: \n";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cin >> sendAddress;

		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Input Send Port: \n";
		while (!(std::cin >> sendPort))
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cerr << "Invalid input. Please enter a valid number: ";
		}

		std::cout << "Every line from now on will be sent as a message: \n";

		isSendAddressSet = true;
	}

	// Input message
	std::cin.clear();
	std::cin.getline(messageBuffer, sizeof(messageBuffer));

	if (mySender)
	{
		Ref<Aryl::NetPacket> packet = CreateRef<Aryl::NetPacket>();

		packet->header.id = Aryl::NetMessageType::StringMessage;

		packet->data.resize(sizeof(messageBuffer) + 1);
		std::memcpy(packet->data.data(), messageBuffer, packet->data.size());

		mySender->Send(packet, Aryl::IPv4Endpoint(Aryl::IPv4Address(sendAddress), sendPort));
	}
}

void Editor::ChatDisconnect()
{
	if (mySender)
	{
		Ref<Aryl::NetPacket> packet = CreateRef<Aryl::NetPacket>();

		packet->header.id = Aryl::NetMessageType::Disconnect;

		mySender->Send(packet, Aryl::IPv4Endpoint(Aryl::IPv4Address(g_sendAddress), g_sendPort));
	}
}

void Editor::ArylNetExample()
{
	static char hostAddress[16] = "127.0.0.1";
	static char currentHostAddress[16] = "127.0.0.1";
	static int hostPort = 44000;
	static int currentHostPort = hostPort;

	static char sendAddress[16] = "127.0.0.1";
	static int sendPort = 44000;

	ImGui::Begin("ArylNet");
	ImGui::InputText("Host Address", hostAddress, sizeof(hostAddress));
	ImGui::InputInt("Host Port", &hostPort);
	ImGui::NewLine();
	ImGui::InputText("Send Address", sendAddress, sizeof(sendAddress));
	ImGui::InputInt("Send Port", &sendPort);

	if (ImGui::Button("Sender"))
	{
		SetupSender(Aryl::IPv4Endpoint(Aryl::IPv4Address(hostAddress), 0));
	}

	ImGui::SameLine();
	if (ImGui::Button("Receiver"))
	{
		SetupReceiver(Aryl::IPv4Endpoint(Aryl::IPv4Address(hostAddress), hostPort), [this](Aryl::NetPacket packet) {
			if (packet.header.id == Aryl::NetMessageType::StringMessage)
			{
				std::string str((const char*)packet.data.data());
				YL_CORE_TRACE(str + " from {0}:{1}", packet.endpoint.GetAddress().GetAddressString(), packet.endpoint.GetPort());
			}
		});
	}

	if (mySender)
	{
		ImGui::SameLine();
		if (ImGui::Button("Ping"))
		{
			std::string data = "Ping";

			Ref<Aryl::NetPacket> packet = CreateRef<Aryl::NetPacket>();

			packet->header.id = Aryl::NetMessageType::StringMessage;

			packet->data.resize(data.size() + 1);
			std::memcpy(packet->data.data(), data.c_str(), packet->data.size());

			mySender->Send(packet, Aryl::IPv4Endpoint(Aryl::IPv4Address(sendAddress), sendPort));
		}
	}

	ImGui::NewLine();

	// Debug info

	if (myReceiver || mySender)
	{
		ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, "Connected");
		if (myReceiver)
		{
			std::string text = "Receiver Active on port: " + std::to_string(currentHostPort);
			ImGui::TextColored({ 0.f, 0.5f, 0.f, 1.f }, text.c_str());
		}
		if (mySender)
		{
			ImGui::TextColored({ 0.f, 0.5f, 0.f, 1.f }, "Sender Active");
		}
	}
	else
	{
		ImGui::TextColored({ 1.f, 0.f, 0.f, 1.f }, "Disconnected");
	}

	ImGui::End();

	if (!myTestingEntity.IsNull())
	{
		auto& comp = myTestingEntity.GetComponent<Aryl::TestComponent>();

		ImGui::Begin("Example Component");
		ImGui::InputInt("ComponentInt", &comp.intValue);
		ImGui::InputFloat("ComponentFloat", &comp.floatValue);
		ImGui::Checkbox("ComponentBool", &comp.boolValue);
		if (ImGui::Button("Sync"))
		{
			// Notify server/client of changes
		}
		ImGui::End();
	}
}

void Editor::SetupSender(Aryl::IPv4Endpoint endpoint)
{
	if (mySender)
	{
		mySender->Stop();
		mySender = nullptr;
	}

	Ref<Aryl::UdpSocketBuilder> builder = Aryl::UdpSocketBuilder::Create(Aryl::Application::Get().GetNetworkContext());
	builder->BoundToAddress(endpoint.GetAddress());
	builder->BoundToPort(0);

	auto socket = builder->Build();

	if (socket)
	{
		mySender = Aryl::UdpSocketSender::Create(socket);
	}
}

void Editor::SetupReceiver(Aryl::IPv4Endpoint endpoint, std::function<void(Aryl::NetPacket)> callback)
{
	if (myReceiver)
	{
		bool hasSender = mySender != nullptr;
		if (!hasSender)
		{
			Ref<Aryl::UdpSocketBuilder> builder = Aryl::UdpSocketBuilder::Create(Aryl::Application::Get().GetNetworkContext());
			builder->BoundToAddress(endpoint.GetAddress());
			builder->BoundToPort(0);

			mySender = Aryl::UdpSocketSender::Create(builder->Build());
		}
		myReceiver->Stop();

		Ref<Aryl::NetPacket> packet = CreateRef<Aryl::NetPacket>();
		packet->header.id = Aryl::NetMessageType::Disconnect;

		mySender->Send(packet, endpoint);

		myReceiver = nullptr;

		if (!hasSender)
		{
			mySender->Stop();
			mySender = nullptr;
		}
	}

	Ref<Aryl::UdpSocketBuilder> builder = Aryl::UdpSocketBuilder::Create(Aryl::Application::Get().GetNetworkContext());
	builder->BoundToAddress(endpoint.GetAddress());
	builder->BoundToPort(endpoint.GetPort());

	auto socket = builder->Build();

	if (socket)
	{
		myReceiver = Aryl::UdpSocketReceiver::Create(socket, callback);
		YL_CORE_TRACE("Starting UDP server on {0}:{1}", endpoint.GetAddress().GetAddressString(), endpoint.GetPort());
	}
}
