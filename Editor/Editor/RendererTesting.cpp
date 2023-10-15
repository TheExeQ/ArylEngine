#include "RendererTesting.h"

#include <Aryl/Core/Base.h>
#include <Aryl/Core/Application.h>

#include <Aryl/Scene/Scene.h>
#include <Aryl/Scene/Entity.h>
#include <Aryl/Scene/SceneManager.h>
#include <Aryl/Components/Components.h>

#include <Aryl/Input/KeyCodes.h>
#include <Aryl/Events/KeyEvent.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

RendererTesting::RendererTesting()
{
	auto curPath = std::filesystem::current_path();
	
	auto& registry = Aryl::SceneManager::GetActiveScene()->GetRegistry();
	{
		auto newEntity = Aryl::Entity(registry.create(), Aryl::SceneManager::GetActiveScene().get());

		registry.emplace<Aryl::TransformComponent>(newEntity.GetId()).position = { 100.f, 0.f, 0.f };
		registry.emplace<Aryl::SpriteRendererComponent>(newEntity.GetId()).spritePath = "test.png";

		newEntity = Aryl::Entity(registry.create(), Aryl::SceneManager::GetActiveScene().get());

		registry.emplace<Aryl::TransformComponent>(newEntity.GetId()).position = { -100.f, 0.f, 0.f };
		registry.emplace<Aryl::SpriteRendererComponent>(newEntity.GetId()).spritePath = "test2.png";
	}

	auto aspectRatio = 16.f / 9.f;

	myOrthoCamera = CreateRef<Aryl::Camera>(-1.0f * aspectRatio, 1.f * aspectRatio, -1.0f, 1.0f, 0.1f, 10000.f);
	myOrthoCamera->SetPosition({ 0.f, 0.f, 500.f });

	myPerspCamera = CreateRef<Aryl::Camera>(90.f, aspectRatio, 0.1f, 10000.0f);
	myPerspCamera->SetPosition({ 0.f, 0.f, 500.f });

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

	for (auto ent : registry.view<Aryl::SpriteRendererComponent>())
	{
		auto path = registry.get<Aryl::SpriteRendererComponent>(ent).spritePath;
		auto pathStr = path.string();

		if (myTextureCache.find(pathStr) == myTextureCache.end())
		{
			myTextureCache[pathStr] = Aryl::Texture2D::Create(path);
			myTextureCache.at(pathStr)->Bind();
		}
	}

	myShader->Bind();
	myShader->UploadUniformInt("u_Texture", 0);
}

RendererTesting::~RendererTesting()
{
}

void RendererTesting::OnEvent(Aryl::Event& e)
{
	Aryl::EventDispatcher dispatcher(e);

	dispatcher.Dispatch<Aryl::AppUpdateEvent>(YL_BIND_EVENT_FN(RendererTesting::OnUpdate));
	dispatcher.Dispatch<Aryl::AppRenderEvent>(YL_BIND_EVENT_FN(RendererTesting::OnRender));
}

bool RendererTesting::OnRender(Aryl::AppRenderEvent& e)
{
	TempOpenGLTesting();

	return false;
}

bool RendererTesting::OnUpdate(Aryl::AppUpdateEvent& e)
{
	//myCubeTransform = glm::rotate(myCubeTransform, glm::radians(30.f) * e.GetTimestep(), { 0.f, 1.f, 0.f });

	return false;
}

void RendererTesting::TempOpenGLTesting()
{
	auto& registry = Aryl::SceneManager::GetActiveScene()->GetRegistry();

	Aryl::Renderer::BeginScene(myPerspCamera);

	for (auto ent : registry.view<Aryl::SpriteRendererComponent>())
	{
		auto path = registry.get<Aryl::SpriteRendererComponent>(ent).spritePath.string();
		myTextureCache.at(path)->Bind();

		Aryl::Renderer::Submit(myShader, mySquareVertexArray, glm::translate(glm::mat4(1.f), registry.get<Aryl::TransformComponent>(ent).position));
	}

	Aryl::Renderer::EndScene();
}
