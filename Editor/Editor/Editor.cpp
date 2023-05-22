#include "Editor.h"

#include <Aryl/Core/Base.h>
#include <Aryl/Core/Application.h>

#include <Aryl/Components/Components.h>
#include <Aryl/Scene/Entity.h>
#include <Aryl/Scene/Scene.h>
#include <Aryl/Scene/SceneManager.h>

#include <Aryl/Events/KeyEvent.h>
#include <Aryl/Input/KeyCodes.h>

#include <Aryl/Networking/Socket/UdpSocketBuilder.h>

#include <ImGui.h>
#include <entt/entt.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb/stb_image.h>

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
	// ENTT REFLECTION TESTING
	if (false)
	{
		auto& registry = Aryl::SceneManager::GetActiveScene()->GetRegistry();
		myTestingEntity = Aryl::Entity(registry.create(), Aryl::SceneManager::GetActiveScene().get());
		registry.emplace<Aryl::TestComponent>(myTestingEntity.GetId());

		Aryl::RegisterReflection();

		auto& comp = myTestingEntity.GetComponent<Aryl::TestComponent>();

		YL_CORE_TRACE(comp.intValue);
		YL_CORE_TRACE(comp.floatValue);
		YL_CORE_TRACE(comp.boolValue);

		auto type = entt::resolve(entt::hashed_string(typeid(Aryl::TestComponent).name()));
		if (type)
		{
			auto data = type.data(entt::hashed_string("intValue"));
			auto data2 = type.data(entt::hashed_string("floatValue"));
			auto data3 = type.data(entt::hashed_string("boolValue"));

			auto type_id = entt::type_id<Aryl::TestComponent>().hash();
			auto storage = registry.storage(type_id);

			YL_CORE_TRACE("TypeId: {0}", type_id);

			if (storage)
			{
				auto compontentPtr = storage->get(myTestingEntity.GetId());
				auto componentAny = type.from_void(compontentPtr);

				if (data) { data.set(componentAny, 5); }
				if (data2) { data2.set(componentAny, 4.3f); }
				if (data3) { data3.set(componentAny, true); }
			}
		}

		YL_CORE_TRACE(comp.intValue);
		YL_CORE_TRACE(comp.floatValue);
		YL_CORE_TRACE(comp.boolValue);
	}

	// RENDERING TESTING
	//if (false)
	{
		auto aspectRatio = 16.f / 9.f;

		myOrthoCamera = CreateRef<Aryl::Camera>(-1.0f * aspectRatio, 1.f * aspectRatio, -1.0f, 1.0f, 0.1f, 100.f);
		myPerspCamera = CreateRef<Aryl::Camera>(90.f, aspectRatio, 0.1f, 100.0f);
		myPerspCamera->SetPosition({ 0.f, 0.f, 1.f });

		myVertexArray = Aryl::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		auto vertexBuffer = Aryl::VertexBuffer::Create(vertices, sizeof(vertices));

		Aryl::BufferLayout layout = {
			{ Aryl::ShaderDataType::Float3, "a_Position" },
			{ Aryl::ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);

		myVertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		auto indexBuffer = Aryl::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		myVertexArray->SetIndexBuffer(indexBuffer);

		// Square
		{
			mySquareVertexArray = Aryl::VertexArray::Create();

			float sqaureVertices[4 * 7] = {
				-0.75f, -0.75f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
				0.75f, -0.75f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
				0.75f,  0.75f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
				-0.75f,  0.75f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			};

			auto squareVB = Aryl::VertexBuffer::Create(sqaureVertices, sizeof(sqaureVertices));

			squareVB->SetLayout(layout);
			mySquareVertexArray->AddVertexBuffer(squareVB);

			uint32_t sqaureIndices[6] = { 0, 1, 2, 2, 3, 0 };
			auto squareIB = Aryl::IndexBuffer::Create(sqaureIndices, sizeof(sqaureIndices) / sizeof(uint32_t));

			mySquareVertexArray->SetIndexBuffer(squareIB);
		}

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_ModelTransform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_ModelTransform * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		myShader = Aryl::Shader::Create(vertexSrc, fragmentSrc);
	}
}

void Editor::OnDetach()
{
}

void Editor::OnEvent(Aryl::Event& e)
{
	Aryl::EventDispatcher dispatcher(e);

	dispatcher.Dispatch<Aryl::AppUpdateEvent>(YL_BIND_EVENT_FN(Editor::OnUpdate));
	dispatcher.Dispatch<Aryl::AppRenderEvent>(YL_BIND_EVENT_FN(Editor::OnRender));
	dispatcher.Dispatch<Aryl::AppImGuiUpdateEvent>(YL_BIND_EVENT_FN(Editor::OnImGuiUpdate));
}

bool Editor::OnRender(Aryl::AppRenderEvent& e)
{
	TempOpenGLTesting();

	return false;
}

bool Editor::OnUpdate(Aryl::AppUpdateEvent& e)
{
	return false;
}

bool Editor::OnImGuiUpdate(Aryl::AppImGuiUpdateEvent& e)
{
	//ArylNetExample();

	return false;
}

//////////////////////
///// RENDERING //////
//////////////////////

void Editor::TempOpenGLTesting()
{
	Aryl::Renderer::BeginScene(myOrthoCamera);

	Aryl::Renderer::Submit(myShader, mySquareVertexArray);
	Aryl::Renderer::Submit(myShader, myVertexArray);

	Aryl::Renderer::EndScene();
}

char g_sendAddress[16] = "127.0.0.1";
int g_sendPort = 44000;

//////////////////////
///// NETWORKING /////
//////////////////////

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
