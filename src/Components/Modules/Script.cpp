#include "STDInclude.hpp"

namespace Components
{
	std::string Script::ScriptName;
	std::vector<int> Script::ScriptHandles;
	std::vector<Script::Function> Script::ScriptFunctions;
	std::vector<std::string> Script::ScriptNameStack;
	unsigned short Script::FunctionName;

	Utils::Signal<Scheduler::Callback> Script::VMShutdownSignal;

	void Script::FunctionError()
	{
		std::string funcName = Game::SL_ConvertToString(Script::FunctionName);

		Game::Scr_ShutdownAllocNode();

		Logger::Print(23, "\n");
		Logger::Print(23, "******* script compile error *******\n");
		Logger::Print(23, "Error: unknown function %s in %s\n", funcName.data(), Script::ScriptName.data());
		Logger::Print(23, "************************************\n");

		Logger::Error(5, "script compile error\nunknown function %s\n%s\n\n", funcName.data(), Script::ScriptName.data());
	}

	__declspec(naked) void Script::StoreFunctionNameStub()
	{
		__asm
		{
			mov eax, [esp - 8h]
			mov Script::FunctionName, ax

			sub esp, 0Ch
			push 0
			push edi

			mov eax, 612DB6h
			jmp eax
		}
	}

	void Script::StoreScriptName(const char* name)
	{
		Script::ScriptNameStack.push_back(Script::ScriptName);
		Script::ScriptName = name;

		if (!Utils::String::EndsWith(Script::ScriptName, ".gsc"))
		{
			Script::ScriptName.append(".gsc");
		}
	}

	__declspec(naked) void Script::StoreScriptNameStub()
	{
		__asm
		{
			pushad

			lea ecx, [esp + 30h]
			push ecx

			call Script::StoreScriptName
			add esp, 4h

			popad

			push ebp
			mov ebp, ds:1CDEAA8h

			push 427DC3h
			retn
		}
	}

	void Script::RestoreScriptName()
	{
		Script::ScriptName = Script::ScriptNameStack.back();
		Script::ScriptNameStack.pop_back();
	}

	__declspec(naked) void Script::RestoreScriptNameStub()
	{
		__asm
		{
			pushad
			call Script::RestoreScriptName
			popad

			mov ds:1CDEAA8h, ebp

			push 427E77h
			retn
		}
	}

	void Script::PrintSourcePos(const char* filename, unsigned int offset)
	{
		FileSystem::File script(filename);

		if (script.exists())
		{
			std::string buffer = script.getBuffer();
			Utils::String::Replace(buffer, "\t", " ");

			int line = 1;
			int lineOffset = 0;
			int inlineOffset = 0;

			for (unsigned int i = 0; i < buffer.size(); ++i)
			{
				// Terminate line
				if (i == offset)
				{
					while (buffer[i] != '\r' && buffer[i] != '\n' && buffer[i] != '\0')
					{
						++i;
					}

					buffer[i] = '\0';
					break;
				}

				if (buffer[i] == '\n')
				{
					++line;
					lineOffset = i; // Includes the line break!
					inlineOffset = 0;
				}
				else
				{
					++inlineOffset;
				}
			}

			Logger::Print(23, "in file %s, line %d:", filename, line);
			Logger::Print(23, "%s\n", buffer.data() + lineOffset);

			for (int i = 0; i < (inlineOffset - 1); ++i)
			{
				Logger::Print(23, " ");
			}

			Logger::Print(23, "*\n");
		}
		else
		{
			Logger::Print(23, "in file %s, offset %d\n", filename, offset);
		}
	}

	void Script::CompileError(unsigned int offset, const char* message, ...)
	{
		char msgbuf[1024] = { 0 };
		va_list v;
		va_start(v, message);
		_vsnprintf_s(msgbuf, sizeof(msgbuf), message, v);
		va_end(v);

		Game::Scr_ShutdownAllocNode();

		Logger::Print(23, "\n");
		Logger::Print(23, "******* script compile error *******\n");
		Logger::Print(23, "Error: %s ", msgbuf);
		Script::PrintSourcePos(Script::ScriptName.data(), offset);
		Logger::Print(23, "************************************\n\n");

		Logger::Error(5, "script compile error\n%s\n%s\n(see console for actual details)\n", msgbuf, Script::ScriptName.data());
	}

	int Script::LoadScriptAndLabel(const std::string& script, const std::string& label)
	{
		Logger::Print("Loading script %s.gsc...\n", script.data());

		if (!Game::Scr_LoadScript(script.data()))
		{
			Logger::Print("Script %s encountered an error while loading. (doesn't exist?)", script.data());
			Logger::Error(1, reinterpret_cast<char*>(0x70B810), script.data());
		}
		else
		{
			Logger::Print("Script %s.gsc loaded successfully.\n", script.data());
		}

		Logger::Print("Finding script handle %s::%s...\n", script.data(), label.data());
		int handle = Game::Scr_GetFunctionHandle(script.data(), label.data());
		if (handle)
		{
			Logger::Print("Script handle %s::%s loaded successfully.\n", script.data(), label.data());
			return handle;
		}

		Logger::Print("Script handle %s::%s couldn't be loaded. (file with no entry point?)\n", script.data(), label.data());
		return handle;
	}

	void Script::LoadGameType()
	{
		for (auto handle : Script::ScriptHandles)
		{
			Game::Scr_FreeThread(Game::Scr_ExecThread(handle, 0));
		}

		Game::Scr_LoadGameType();
	}

	void Script::LoadGameTypeScript()
	{
		Script::ScriptHandles.clear();

		auto list = FileSystem::GetFileList("scripts/", "gsc");

		for (auto file : list)
		{
			file = "scripts/" + file;

			if (Utils::String::EndsWith(file, ".gsc"))
			{
				file = file.substr(0, file.size() - 4);
			}

			int handle = Script::LoadScriptAndLabel(file, "init");
			if (handle) Script::ScriptHandles.push_back(handle);
			else
			{
				handle = Script::LoadScriptAndLabel(file, "main");
				if (handle) Script::ScriptHandles.push_back(handle);
			}
		}

		Game::GScr_LoadGameTypeScript();
	}

	void Script::AddFunction(const std::string& name, Game::scr_function_t function, bool isDev)
	{
		for (auto i = Script::ScriptFunctions.begin(); i != Script::ScriptFunctions.end();)
		{
			if (i->getName() == name)
			{
				i = Script::ScriptFunctions.erase(i);
				continue;
			}

			++i;
		}

		Script::ScriptFunctions.push_back({ name, function, isDev });
	}

	Game::scr_function_t Script::GetFunction(void* caller, const char** name, int* isDev)
	{
		for (auto& function : Script::ScriptFunctions)
		{
			if (name && *name)
			{
				if (Utils::String::ToLower(*name) == Utils::String::ToLower(function.getName()))
				{
					*name = function.getName();
					*isDev = function.isDev();
					return function.getFunction();
				}
			}
			else if (caller == reinterpret_cast<void*>(0x465781))
			{
				Game::Scr_RegisterFunction(function.getFunction());
			}
		}

		return nullptr;
	}

	__declspec(naked) void Script::GetFunctionStub()
	{
		__asm
		{
			test eax, eax
			jnz returnSafe

			sub esp, 8h
			push [esp + 10h]
			call Script::GetFunction
			add esp, 0Ch

		returnSafe:
			pop edi
			pop esi
			retn
		}
	}

	void Script::OnVMShutdown(Utils::Slot<Scheduler::Callback> callback)
	{
		Script::VMShutdownSignal.connect(callback);
	}

	void Script::ScrShutdownSystemStub(int num)
	{
		Script::VMShutdownSignal();

		// Scr_ShutdownSystem
		Utils::Hook::Call<void(int)>(0x421EE0)(num);
	}

	int Script::SetExpFogStub()
	{
		if (Game::Scr_GetNumParam() == 6)
		{
			std::memmove(&Game::scriptContainer->stack[-4], &Game::scriptContainer->stack[-5], sizeof(Game::VariableValue) * 6);
			Game::scriptContainer->stack += 1;
			Game::scriptContainer->stack[-6].type = Game::VAR_FLOAT;
			Game::scriptContainer->stack[-6].u.floatValue = 0;

			++Game::scriptContainer->numParam;
		}

		return Game::Scr_GetNumParam();
	}

	Game::gentity_t* Script::getEntFromEntRef(Game::scr_entref_t entref)
	{
		Game::gentity_t* gentity = &Game::g_entities[entref];
		return gentity;
	}

	Game::client_t* Script::getClientFromEnt(Game::gentity_t* gentity)
	{
		if (!gentity->client)
		{
			Logger::Error(5, "Entity: %i is not a client", gentity);
		}
		return &Game::svs_clients[gentity->number];
	}

	Script::Script()
	{
		Utils::Hook(0x612DB0, Script::StoreFunctionNameStub, HOOK_JUMP).install()->quick();
		Utils::Hook(0x427E71, Script::RestoreScriptNameStub, HOOK_JUMP).install()->quick();
		Utils::Hook(0x427DBC, Script::StoreScriptNameStub, HOOK_JUMP).install()->quick();

		Utils::Hook(0x612E8D, Script::FunctionError, HOOK_CALL).install()->quick();
		Utils::Hook(0x612EA2, Script::FunctionError, HOOK_CALL).install()->quick();
		Utils::Hook(0x434260, Script::CompileError, HOOK_JUMP).install()->quick();

		Utils::Hook(0x48EFFE, Script::LoadGameType, HOOK_CALL).install()->quick();
		Utils::Hook(0x45D44A, Script::LoadGameTypeScript, HOOK_CALL).install()->quick();

		Utils::Hook(0x44E736, Script::GetFunctionStub, HOOK_JUMP).install()->quick(); // Scr_GetFunction
		Utils::Hook(0x4EC8E5, Script::GetFunctionStub, HOOK_JUMP).install()->quick(); // Scr_GetMethod

		Utils::Hook(0x5F41A3, Script::SetExpFogStub, HOOK_CALL).install()->quick();

		Utils::Hook(0x47548B, Script::ScrShutdownSystemStub, HOOK_CALL).install()->quick();
		Utils::Hook(0x4D06BA, Script::ScrShutdownSystemStub, HOOK_CALL).install()->quick();

		Script::AddFunction("debugBox", [](Game::scr_entref_t)
		{
			MessageBoxA(nullptr, Game::Scr_GetString(0), "DEBUG", 0);
		}, true);

		// Script::AddFunction("playviewmodelfx", [](Game::scr_entref_t /*index*/)
		// {
		// 	/*auto Scr_Error = Utils::Hook::Call<void(const char*)>(0x42EF40);
		// 	if (index >> 16)
		// 	{
		// 		Scr_Error("not an entity");
		// 		return;
		// 	}*/

		// 	// obtain FX name
		// 	auto fxName = Game::Scr_GetString(0);
		// 	auto fx = Game::DB_FindXAssetHeader(Game::XAssetType::ASSET_TYPE_FX, fxName).fx;

		// 	auto tagName = Game::Scr_GetString(1);
		// 	auto tagIndex = Game::SL_GetString(tagName, 0);

		// 	/*char boneIndex = -2;
		// 	if (!Game::CG_GetBoneIndex(2048, tagIndex, &boneIndex))
		// 	{
		// 		Scr_Error(Utils::String::VA("Unknown bone %s.\n", tagName));
		// 		return;
		// 	}*/

		// 	Game::CG_PlayBoltedEffect(0, fx, 2048, tagIndex);
		// });
	}

	Script::~Script()
	{
		Script::ScriptName.clear();
		Script::ScriptHandles.clear();
		Script::ScriptNameStack.clear();
		Script::ScriptFunctions.clear();
		Script::VMShutdownSignal.clear();
	}
}
