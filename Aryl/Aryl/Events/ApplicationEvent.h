#pragma once

#include "Event.h"
#include "Aryl/Core/TimeStep.h"

#include <sstream>
#include <vector>
#include <filesystem>

namespace Aryl
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
			: m_width(width), m_height(height), m_x(x), m_y(y)
		{
		}

		WindowResizeEvent(uint32_t width, uint32_t height)
			: m_width(width), m_height(height), m_x(0), m_y(0)
		{
		}

		//Getting
		inline const uint32_t GetWidth() const { return m_width; }
		inline const uint32_t GetHeight() const { return m_height; }
		inline const uint32_t GetX() const { return m_x; }
		inline const uint32_t GetY() const { return m_y; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_width << ", " << m_height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	private:
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_x;
		uint32_t m_y;
	};

	class ViewportResizeEvent : public Event
	{
	public:
		ViewportResizeEvent(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
			: m_width(width), m_height(height), m_x(x), m_y(y)
		{
		}
		//Getting
		inline const uint32_t GetWidth() const { return m_width; }
		inline const uint32_t GetHeight() const { return m_height; }
		inline const uint32_t GetX() const { return m_x; }
		inline const uint32_t GetY() const { return m_y; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "ViewportResizeEvent: " << m_width << ", " << m_height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(ViewportResize);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	private:
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_x;
		uint32_t m_y;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent(Timestep timestep)
			: m_timestep(timestep)
		{
		}

		inline const Timestep& GetTimestep() { return m_timestep; }

		EVENT_CLASS_TYPE(AppUpdate);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	private:
		float m_timestep;
	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;

		EVENT_CLASS_TYPE(AppRender);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class AppImGuiUpdateEvent : public Event
	{
	public:
		AppImGuiUpdateEvent() = default;

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "AppImGuiUpdateEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(AppImGuiUpdate);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class WindowDragDropEvent : public Event
	{
	public:
		WindowDragDropEvent(int32_t count, const char** paths)
		{
			for (int32_t i = 0; i < count; ++i)
			{
				m_paths.push_back(paths[i]);
			}
		}

		inline const std::vector <std::filesystem::path>& GetPaths() const { return m_paths; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowDragDropEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowDragDrop);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	private:
		std::vector<std::filesystem::path> m_paths;
	};

	class OnScenePlayEvent : public Event
	{
	public:
		OnScenePlayEvent() = default;

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "OnScenePlayEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(OnScenePlay);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class OnSceneStopEvent : public Event
	{
	public:
		OnSceneStopEvent() = default;

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "OnSceneStopEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(OnSceneStop);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class Scene;
	class OnSceneLoadedEvent : public Event
	{
	public:
		OnSceneLoadedEvent(Ref<Aryl::Scene> aScene)
			: myScene(aScene)
		{
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "OnSceneLoadedEvent";
			return ss.str();
		}

		inline Ref<Aryl::Scene> GetScene() const { return myScene; }

		EVENT_CLASS_TYPE(OnSceneLoaded);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

	private:
		Ref<Aryl::Scene> myScene;
	};
}