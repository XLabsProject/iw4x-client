#pragma once

namespace Components
{
	class News : public Component
	{
	public:
		News();

		void preDestroy() override;
		bool unitTest() override;

	private:
		static std::thread Thread;

		static bool Terminate;
		static bool DownloadUpdater();

		static const char* GetNewsText();
	};
}
