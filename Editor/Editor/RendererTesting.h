#pragma once

#include <Aryl/Events/ApplicationEvent.h>

#include <Aryl/Renderer/Renderer.h>
#include <Aryl/Renderer/Shader.h>
#include <Aryl/Renderer/Buffer.h>
#include <Aryl/Renderer/Texture.h>
#include <Aryl/Renderer/VertexArray.h>
#include <Aryl/Renderer/Camera/Camera.h>

class RendererTesting
{
public:
	RendererTesting();
	~RendererTesting();

	void OnEvent(Aryl::Event& e);
	bool OnRender(Aryl::AppRenderEvent& e);
	bool OnUpdate(Aryl::AppUpdateEvent& e);

	void TempOpenGLTesting();

private:
	glm::mat4 myCubeTransform;
	Ref<Aryl::Camera> myOrthoCamera;
	Ref<Aryl::Camera> myPerspCamera;
	Ref<Aryl::Texture2D> myTexture;
	Ref<Aryl::Shader> myShader;
	Ref<Aryl::VertexArray> mySquareVertexArray;
};