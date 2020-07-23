#pragma once

namespace Components
{
	class Component
	{
	public:
		Component() {};
		virtual ~Component() {};

#if defined(DEBUG) || defined(FORCE_UNIT_TESTS)
		virtual std::string getName()
		{
			std::string name = typeid(*this).name();
			Utils::String::Replace(name, "class Components::", "");
			return name;
		};
#endif

		// It's illegal to spawn threads in DLLMain, and apparently it causes problems if they are destroyed there as well.
		// This method is called before DLLMain (if possible) and should to destroy threads.
		// It's not 100% guaranteed that it's called outside DLLMain, as it depends on the game, but it's 100% guaranteed, that it is called at all.
		virtual void preDestroy() {};
		virtual bool unitTest() { return true; }; // Unit testing entry
	};

	class Loader
	{
	public:
		static void Initialize();
		static void Uninitialize();
		static void PreDestroy();
		static void PreDestroyNoPostGame();
		static bool PerformUnitTests();
		static bool IsPerformingUnitTests();
		static void Register(Component* component);

		static bool IsPregame();
		static bool IsPostgame();
		static bool IsUninitializing();

		template <typename T>
		static T* GetInstance()
		{
			for (auto& component : Loader::Components)
			{
				if (typeid(*component) == typeid(T))
				{
					return reinterpret_cast<T*>(component);
				}
			}

			return nullptr;
		}

	private:
		static bool Pregame;
		static bool Postgame;
		static bool Uninitializing;
		static std::vector<Component*> Components;
	};
}

#include "Modules/Scheduler.hpp"
#include "Modules/Auth.hpp"
#include "Modules/Bans.hpp"
#include "Modules/Bots.hpp"
#include "Modules/Dvar.hpp"
#include "Modules/Lean.hpp"
#include "Modules/Maps.hpp"
#include "Modules/News.hpp"
#include "Modules/Flags.hpp"
#include "Modules/Menus.hpp"
#include "Modules/Toast.hpp"
#include "Modules/Zones.hpp"
#include "Modules/Colors.hpp"
#include "Modules/D3D9Ex.hpp"
#include "Modules/Script.hpp"
#include "Modules/Weapon.hpp"
#include "Modules/Window.hpp"
#include "Modules/Command.hpp"
#include "Modules/Console.hpp"
#include "Modules/UIScript.hpp"
#include "Modules/ModList.hpp"
#include "Modules/Monitor.hpp"
#include "Modules/Network.hpp"
#include "Modules/Theatre.hpp"
#include "Modules/QuickPatch.hpp"
#include "Modules/Node.hpp"
#include "Modules/RCon.hpp"
#include "Modules/Party.hpp" // Destroys the order, but requires network classes :D
#include "Modules/IW4MVM.hpp"
#include "Modules/Logger.hpp"
#include "Modules/Friends.hpp"
#include "Modules/IPCPipe.hpp"
#include "Modules/MapDump.hpp"
#include "Modules/Session.hpp"
#include "Modules/ClanTags.hpp"
#include "Modules/Download.hpp"
#include "Modules/Playlist.hpp"
#include "Modules/RawFiles.hpp"
#include "Modules/Renderer.hpp"
#include "Modules/UIFeeder.hpp"
#include "Modules/AntiCheat.hpp"
#include "Modules/Changelog.hpp"
#include "Modules/Dedicated.hpp"
#include "Modules/Discovery.hpp"
#include "Modules/Exception.hpp"
#include "Modules/FastFiles.hpp"
#include "Modules/FrameTime.hpp"
#include "Modules/Gametypes.hpp"
#include "Modules/Materials.hpp"
#include "Modules/Singleton.hpp"
#include "Modules/Threading.hpp"
#include "Modules/CardTitles.hpp"
#include "Modules/FileSystem.hpp"
#include "Modules/ModelSurfs.hpp"
#include "Modules/PlayerName.hpp"
#include "Modules/ServerInfo.hpp"
#include "Modules/ServerList.hpp"
#include "Modules/SlowMotion.hpp"
#include "Modules/ArenaLength.hpp"
#include "Modules/StringTable.hpp"
#include "Modules/ZoneBuilder.hpp"
#include "Modules/AssetHandler.hpp"
#include "Modules/Localization.hpp"
#include "Modules/MusicalTalent.hpp"
#include "Modules/ServerCommands.hpp"
#include "Modules/StructuredData.hpp"
#include "Modules/ConnectProtocol.hpp"
#include "Modules/StartupMessages.hpp"
#include "Modules/Stats.hpp"

#include "Modules/Client.hpp"