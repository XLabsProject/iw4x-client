#pragma once

namespace Components
{
	class Stats : public Component
	{
	public:
		Stats();
		~Stats();

		static bool IsMaxLevel();

	private:
		static void UpdateClasses(UIScript::Token token);
		static void SendStats();
		static int SaveStats(char* dest, const char* folder, const char* buffer, size_t length);
		static int ReadStats(char* buffer, size_t buf_size, const char* format);

		static int64_t* GetStatsID();
	};
}
