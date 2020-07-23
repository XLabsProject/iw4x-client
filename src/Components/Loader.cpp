#include "STDInclude.hpp"

namespace Components
{
	bool Loader::Pregame = true;
	bool Loader::Postgame = false;
	bool Loader::Uninitializing = false;
	std::vector<Component*> Loader::Components;

	bool Loader::IsPregame()
	{
		return Loader::Pregame;
	}

	bool Loader::IsPostgame()
	{
		return Loader::Postgame;
	}

	bool Loader::IsUninitializing()
	{
		return Loader::Uninitializing;
	}

	void Loader::Initialize()
	{
		Loader::Pregame = true;
		Loader::Postgame = false;
		Loader::Uninitializing = false;
		Utils::Memory::GetAllocator()->clear();

		Loader::Register(new Flags());
		Loader::Register(new Singleton());
		Loader::Register(new Exception()); // install our exception handler as early as posssible to get better debug dumps from startup crashes

		Loader::Register(new Auth());
		Loader::Register(new Bans());
		Loader::Register(new Bots());
		Loader::Register(new Dvar());
		Loader::Register(new Lean());
		Loader::Register(new Maps());
		Loader::Register(new News());
		Loader::Register(new Node());
		Loader::Register(new RCon());
		Loader::Register(new Stats());
		Loader::Register(new Menus());
		Loader::Register(new Toast());
		Loader::Register(new Party());
		Loader::Register(new Zones());
		Loader::Register(new Colors());
		Loader::Register(new D3D9Ex());
#if (!defined(VLD_RPTHOOK_INSTALL) || defined(VLDEnable)) && defined(COMPILE_IW4MVM) // IW4MVM uses detours which produces memory leaks, but those are not really relevant
		Loader::Register(new IW4MVM());
#endif
		Loader::Register(new Logger());
		Loader::Register(new Script());
		Loader::Register(new Weapon());
		Loader::Register(new Window());
		Loader::Register(new Command());
		Loader::Register(new Console());
		Loader::Register(new Friends());
		Loader::Register(new IPCPipe());
		Loader::Register(new MapDump());
		Loader::Register(new ModList());
		Loader::Register(new Monitor());
		Loader::Register(new Network());
		Loader::Register(new Session());
		Loader::Register(new Theatre());
		//Loader::Register(new ClanTags());
		Loader::Register(new Download());
		Loader::Register(new Playlist());
		Loader::Register(new RawFiles());
		Loader::Register(new Renderer());
		Loader::Register(new UIFeeder());
		Loader::Register(new UIScript());
		Loader::Register(new AntiCheat());
		Loader::Register(new Changelog());
		Loader::Register(new Dedicated());
		Loader::Register(new Discovery());
		Loader::Register(new FastFiles());
		Loader::Register(new FrameTime());
		Loader::Register(new Gametypes());
		Loader::Register(new Materials());
		Loader::Register(new Scheduler());
		Loader::Register(new Threading());
		Loader::Register(new CardTitles());
		Loader::Register(new FileSystem());
		Loader::Register(new ModelSurfs());
		Loader::Register(new PlayerName());
		Loader::Register(new QuickPatch());
		Loader::Register(new ServerInfo());
		Loader::Register(new ServerList());
		Loader::Register(new SlowMotion());
		Loader::Register(new ArenaLength());
		Loader::Register(new StringTable());
		Loader::Register(new ZoneBuilder());
		Loader::Register(new AssetHandler());
		Loader::Register(new Localization());
		//Loader::Register(new MusicalTalent());
		Loader::Register(new ServerCommands());
		Loader::Register(new StructuredData());
		Loader::Register(new ConnectProtocol());
		Loader::Register(new StartupMessages());

		Loader::Register(new Client());

		Loader::Pregame = false;
	}

	void Loader::Uninitialize()
	{
		Loader::Uninitializing = true;
		Loader::PreDestroyNoPostGame();

		std::reverse(Loader::Components.begin(), Loader::Components.end());
		for (auto component : Loader::Components)
		{
#ifdef DEBUG
			if (!Loader::IsPerformingUnitTests())
			{
				Logger::Print("Unregistering component: %s\n", component->getName().data());
			}
#endif
			delete component;
		}

		Loader::Components.clear();
		Utils::Memory::GetAllocator()->clear();
		Loader::Uninitializing = false;
	}

	void Loader::PreDestroy()
	{
		if (!Loader::Postgame)
		{
			Loader::Postgame = true;

			auto components = Loader::Components;

			std::reverse(components.begin(), components.end());
			for (auto component : components)
			{
				component->preDestroy();
			}
		}
	}

	void Loader::PreDestroyNoPostGame()
	{
		if (!Loader::Postgame)
		{
			auto components = Loader::Components;

			std::reverse(components.begin(), components.end());
			for (auto component : components)
			{
				component->preDestroy();
			}

			Loader::Postgame = true;
		}
	}

	bool Loader::PerformUnitTests()
	{
		bool result = true;

		Logger::Print("Performing unit tests for components:\n");

		for (auto component : Loader::Components)
		{
#if defined(DEBUG) || defined(FORCE_UNIT_TESTS)
			Logger::Print("Testing '%s'...\n", component->getName().data());
#endif
			auto startTime = std::chrono::high_resolution_clock::now();
			bool testRes = component->unitTest();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime).count();
			Logger::Print("Test done (%llims): %s\n\n", duration, (testRes ? "Success" : "Error"));
			result &= testRes;
		}

		return result;
	}

	bool Loader::IsPerformingUnitTests()
	{
#if defined(DEBUG) || defined(FORCE_UNIT_TESTS)
		return Flags::HasFlag("tests");
#else
		return false;
#endif
	}

	void Loader::Register(Component* component)
	{
		if (component)
		{
#if defined(DEBUG) || defined(FORCE_UNIT_TESTS)
			if (!Loader::IsPerformingUnitTests())
			{
				Logger::Print("Component registered: %s\n", component->getName().data());
			}
#endif
			Loader::Components.push_back(component);
		}
	}
}
