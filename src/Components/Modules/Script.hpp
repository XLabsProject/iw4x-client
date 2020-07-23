#pragma once
#include "Game/Structs.hpp"

namespace Components
{
	class Script : public Component
	{
	public:
		class Function
		{
		public:
			Function(const std::string& _name, Game::scr_function_t _callback, bool _dev) : name(_name), callback(_callback), dev(_dev) {}

			const char* getName() const { return this->name.data(); }
			bool isDev() const { return this->dev; }
			Game::scr_function_t getFunction() const { return this->callback; }

		private:
			std::string name;
			Game::scr_function_t callback;
			bool dev;
		};

		Script();
		~Script();

		static int LoadScriptAndLabel(const std::string& script, const std::string& label);
		static void AddFunction(const std::string& name, Game::scr_function_t function, bool isDev = false);

		static void OnVMShutdown(Utils::Slot<Scheduler::Callback> callback);

		static Game::gentity_t* getEntFromEntRef(Game::scr_entref_t entref);
		static Game::client_t* getClientFromEnt(Game::gentity_t* gentity);

	private:
		static std::string ScriptName;
		static std::vector<int> ScriptHandles;
		static std::vector<Function> ScriptFunctions;
		static std::vector<std::string> ScriptNameStack;
		static unsigned short FunctionName;

		static Utils::Signal<Scheduler::Callback> VMShutdownSignal;

		static void CompileError(unsigned int offset, const char* message, ...);
		static void PrintSourcePos(const char* filename, unsigned int offset);

		static void FunctionError();
		static void StoreFunctionNameStub();

		static void StoreScriptName(const char* name);
		static void StoreScriptNameStub();

		static void RestoreScriptName();
		static void RestoreScriptNameStub();

		static void LoadGameType();
		static void LoadGameTypeScript();

		static Game::scr_function_t GetFunction(void* caller, const char** name, int* isDev);
		static void GetFunctionStub();

		static void ScrShutdownSystemStub(int);

		static int SetExpFogStub();
	};
}
