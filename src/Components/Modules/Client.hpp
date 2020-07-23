#pragma once

namespace Components
{
	class Client : public Component
	{
	public:
		Client();
		~Client();

	private:

		static void AddFunctions();
		static void AddMethods();
		static void AddCommands();
	};
}
