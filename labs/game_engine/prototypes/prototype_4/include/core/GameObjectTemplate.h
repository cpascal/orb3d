#pragma once

namespace engine
{
	class GameObjectTemplate
	{
	public:
		GameObjectTemplate(const std::wstring& name);
		virtual ~GameObjectTemplate(void);

		const std::wstring&				GetName();
		void							SetName(const std::wstring& name);
	private:
		std::wstring					m_name;
	};


}