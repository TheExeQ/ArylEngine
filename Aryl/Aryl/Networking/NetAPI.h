#pragma once

namespace Aryl
{
	enum class NetAPIType
	{
		None,
		Asio,
		WinSock2
	};

	class NetAPI
	{
	public:
		static NetAPIType Current() { return s_CurrentNetAPI; }
		static void SetAPI(NetAPIType api) { s_CurrentNetAPI = api; };

	private:
		inline static NetAPIType s_CurrentNetAPI = NetAPIType::Asio;
	};
}