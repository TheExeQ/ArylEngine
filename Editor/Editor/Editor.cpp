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

		myOrthoCamera = CreateRef<Aryl::Camera>(-1.0f * aspectRatio, 1.f * aspectRatio, -1.0f, 1.0f, 0.1f, 10000.f);
		myOrthoCamera->SetPosition({ 0.f, 0.f, 200.f });

		myPerspCamera = CreateRef<Aryl::Camera>(90.f, aspectRatio, 0.1f, 10000.0f);
		myPerspCamera->SetPosition({ 0.f, 0.f, 200.f });

		myCubeTransform = glm::mat4(1.f);

		Aryl::BufferLayout layout = {
			{ Aryl::ShaderDataType::Float3, "a_Position" },
			{ Aryl::ShaderDataType::Float2, "a_TexCoords" }
		};

		// Square
		{
			mySquareVertexArray = Aryl::VertexArray::Create();

			float squareVertices[8 * 5] = {
				// Front face
				-50.f, -50.f, 50.f, 0.0f, 0.0f,
				50.f, -50.f, 50.f, 1.0f, 0.0f,
				50.f, 50.f, 50.f, 1.0f, 1.0f,
				-50.f, 50.f, 50.f, 0.0f, 1.0f,

				// Back face
				-50.f, -50.f, -50.f, 0.0f, 0.0f,
				50.f, -50.f, -50.f, 1.0f, 0.0f,
				50.f, 50.f, -50.f, 1.0f, 1.0f,
				-50.f, 50.f, -50.f, 0.0f, 1.0f
			};

			auto squareVB = Aryl::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

			squareVB->SetLayout(layout);
			mySquareVertexArray->AddVertexBuffer(squareVB);

			uint32_t squareIndices[36] = {
				// Front face
				0, 1, 2,
				2, 3, 0,

				// Back face
				4, 5, 6,
				6, 7, 4,

				// Left face
				7, 3, 0,
				0, 4, 7,

				// Right face
				1, 5, 6,
				6, 2, 1,

				// Top face
				3, 2, 6,
				6, 7, 3,

				// Bottom face
				0, 1, 5,
				5, 4, 0
			};

			auto squareIB = Aryl::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

			mySquareVertexArray->SetIndexBuffer(squareIB);
		}

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoords;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_ModelTransform;

			out vec2 v_TexCoords;

			void main()
			{
				v_TexCoords = a_TexCoords;
				gl_Position = u_ViewProjection * u_ModelTransform * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec2 v_TexCoords;

			uniform sampler2D u_Texture;

			void main()
			{
				color = texture(u_Texture, v_TexCoords);
			}
		)";

		myShader = Aryl::Shader::Create(vertexSrc, fragmentSrc);

		myTexture = Aryl::Texture2D::Create("test.png");
		myTexture->Bind();

		myShader->Bind();
		myShader->UploadUniformInt("u_Texture", 0);
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
	myCubeTransform = glm::rotate(myCubeTransform, glm::radians(30.f) * e.GetTimestep(), { 0.f, 1.f, 0.f });

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
	Aryl::Renderer::BeginScene(myPerspCamera);

	Aryl::Renderer::Submit(myShader, mySquareVertexArray, myCubeTransform);

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
