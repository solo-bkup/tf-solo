//========== Copyright © 2008, Valve Corporation, All rights reserved. ========
//
// Purpose:
//
//=============================================================================

#ifndef VSCRIPT_SERVER_H
#define VSCRIPT_SERVER_H

#include "vscript/ivscript.h"
#include "vscript_shared.h"

#if defined( _WIN32 )
#pragma once
#endif

extern IScriptVM * g_pScriptVM;

// Only allow scripts to create entities during map initialization
bool IsEntityCreationAllowedInScripts( void );

class CVScriptGameEventListener : public CGameEventListener
{
public:
	virtual void FireGameEvent(IGameEvent* event);
	bool FireScriptHook(const char* pszHookName, HSCRIPT params);

	void RunGameEventCallbacks(const char* szName, HSCRIPT params);
	void RunScriptHookCallbacks(const char* szName, HSCRIPT params);

	void Init();
	void CollectGameEventCallbacksInScope(HSCRIPT scope);

	void ListenForScriptHook(const char* szName);
	bool HasScriptHook(const char* szName);
	void ClearAllScriptHooks();

private:

	CUtlSymbolTable m_ScriptHooks;

	HSCRIPT m_RunGameEventCallbacksFunc;
	HSCRIPT m_CollectGameEventCallbacksFunc;
	HSCRIPT m_ScriptHookCallbacksFunc;
};

extern CVScriptGameEventListener g_VScriptGameEventListener;

bool ScriptHooksEnabled(void);
bool ScriptHookEnabled(const char* pszName);
bool RunScriptHook(const char* pszHookName, HSCRIPT params);

// ----------------------------------------------------------------------------
// KeyValues access
// ----------------------------------------------------------------------------
class CScriptKeyValues
{
public:
	CScriptKeyValues(KeyValues* pKeyValues = NULL);
	~CScriptKeyValues();

	HSCRIPT ScriptFindKey(const char* pszName);
	HSCRIPT ScriptGetFirstSubKey(void);
	HSCRIPT ScriptGetNextKey(void);
	int ScriptGetKeyValueInt(const char* pszName);
	float ScriptGetKeyValueFloat(const char* pszName);
	const char* ScriptGetKeyValueString(const char* pszName);
	bool ScriptIsKeyValueEmpty(const char* pszName);
	bool ScriptGetKeyValueBool(const char* pszName);
	void ScriptReleaseKeyValues();

	KeyValues* m_pKeyValues;	// actual KeyValue entity
};


#endif // VSCRIPT_SERVER_H
