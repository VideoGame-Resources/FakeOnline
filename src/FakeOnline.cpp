#ifdef _WIN32
#include <Windows.h>
#else
#include <string.h>
#include <sys/mman.h>
#endif
#include <set>
#include <array> 
#include "urmem.hpp"

#include "SDK/amx/amx.h"
#include "SDK/plugincommon.h"

using namespace std;
using namespace urmem;

using logprintf_t = void(*)(char* format, ...);

logprintf_t logprintf;

extern void *pAMXFunctions;

class FakeOnline
{
public:

	static bool Init(void)
	{
		_address_cplayerpool__getplayercount = 
#ifdef _WIN32
		{ 0x497D70, 0x497ED1, 0x497FE8 };
#else
		{ 0x80D3573, 0x80D36F5, 0x80D38B0 };
#endif

		_hook_cplayerpool__getplayercount[0]
			= hook::create("cplayerpool__getplayercount0",
				_address_cplayerpool__getplayercount[0],
				reinterpret_cast<address_t>(HOOK_CPlayerPool__GetPlayerCount0),
				hook::type::call);

		_hook_cplayerpool__getplayercount[1]
			= hook::create("cplayerpool__getplayercount1",
				_address_cplayerpool__getplayercount[1],
				reinterpret_cast<address_t>(HOOK_CPlayerPool__GetPlayerCount1),
				hook::type::call);

		_hook_cplayerpool__getplayercount[2]
			= hook::create("cplayerpool__getplayercount2",
				_address_cplayerpool__getplayercount[2],
				reinterpret_cast<address_t>(HOOK_CPlayerPool__GetPlayerCount2),
				hook::type::call);

		logprintf("");
		logprintf(">> FakeOnline plugin by urShadow loaded.");
		logprintf("");

		return true;
	}

	static void UnInit(void)
	{
		logprintf("");
		logprintf(">> FakeOnline plugin by urShadow unloaded.");
		logprintf("");
	}

	static void OnAmxLoad(AMX *amx)
	{
		GetAMXs().insert(amx);
	}

	static void OnAmxUnload(AMX *amx)
	{
		GetAMXs().erase(amx);
	}

private:

#ifdef _WIN32
	static int __thiscall HOOK_CPlayerPool__GetPlayerCount0(void *_this)
#else
	static int HOOK_CPlayerPool__GetPlayerCount0(void *_this)
#endif
	{
		hook::context context(_hook_cplayerpool__getplayercount[0]);

		int result = context.call_original<int>(calling_convention::thiscall, _this);

		return OnSendPlayerCount(result);
	}

#ifdef _WIN32
	static int __thiscall HOOK_CPlayerPool__GetPlayerCount1(void *_this)
#else
	static int HOOK_CPlayerPool__GetPlayerCount1(void *_this)
#endif
	{
		hook::context context(_hook_cplayerpool__getplayercount[1]);

		int result = context.call_original<int>(calling_convention::thiscall, _this);

		return OnSendPlayerCount(result);
	}

#ifdef _WIN32
	static int __thiscall HOOK_CPlayerPool__GetPlayerCount2(void *_this)
#else
	static int HOOK_CPlayerPool__GetPlayerCount2(void *_this)
#endif
	{
		hook::context context(_hook_cplayerpool__getplayercount[2]);

		int result = context.call_original<int>(calling_convention::thiscall, _this);

		return OnSendPlayerCount(result);
	}

	// forward OnSendPlayerCount(count);
	static int OnSendPlayerCount(const int &count)
	{
		int result = -1, idx = -1;

		auto &AMXs = GetAMXs();

		for (auto &amx : AMXs)
		{
			if (amx_FindPublic(amx, "OnSendPlayerCount", &idx) == AMX_ERR_NONE)
			{
				amx_Push(amx, count);

				amx_Exec(amx, &result, idx);

				return result;
			}
		}

		return count;
	}

	static set<AMX *> &GetAMXs(void)
	{
		static set<AMX *> AMXs;

		return AMXs;
	}

	static array<shared_ptr<hook>, 3>	_hook_cplayerpool__getplayercount;
	static array<address_t, 3>			_address_cplayerpool__getplayercount;
};

array<shared_ptr<hook>, 3>	FakeOnline::_hook_cplayerpool__getplayercount;
array<address_t, 3>			FakeOnline::_address_cplayerpool__getplayercount;

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = reinterpret_cast<logprintf_t>(ppData[PLUGIN_DATA_LOGPRINTF]);

	return FakeOnline::Init();
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	return FakeOnline::UnInit();
}

PLUGIN_EXPORT void PLUGIN_CALL AmxLoad(AMX *amx)
{
	return FakeOnline::OnAmxLoad(amx);
}

PLUGIN_EXPORT void PLUGIN_CALL AmxUnload(AMX *amx)
{
	return FakeOnline::OnAmxUnload(amx);
}
