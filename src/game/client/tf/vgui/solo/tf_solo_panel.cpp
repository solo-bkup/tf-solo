#include "cbase.h"
#include "tf_solo_node_panel.h"
#include <vgui/IInput.h>
#include "tf_solo_region_panel.h"
#include "clientmode_tf.h"
#include <vgui_controls/AnimationController.h>
#include "item_model_panel.h"
#include "tf_solo_node_view_panel.h"
#include "tf_vgui_video.h"
#include "tf_matchmaking_dashboard.h"
#include "tf_hud_mainmenuoverride.h"
#include "vguicenterprint.h"
#include "tf_solo_panel.h"
#include "tf_playermodelpanel.h"
#include "vscript_client.h"
#include "vscript_utils.h"

class CSoloTooltip : public CTFTextToolTip
{
public:
	CSoloTooltip(vgui::Panel* parent) : CTFTextToolTip(parent) {}
	// Force the panel to reposition itself every frame.  If the tooltip is coming
	// from an image panel it won't do this, but for the quest panel this is what we want
	virtual void PerformLayout() { CTFTextToolTip::PerformLayout(); _isDirty = true; }
};

DECLARE_BUILD_FACTORY(CSoloPanel);
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CSoloPanel* g_pSoloPanel = NULL;

CSoloPanel* GetSoloPanel()
{
	if (g_pSoloPanel == NULL)
	{
		CHudMainMenuOverride* pMMOverride = (CHudMainMenuOverride*)(gViewPortInterface->FindPanelByName(PANEL_MAINMENUOVERRIDE));
		g_pSoloPanel = new CSoloPanel(pMMOverride, "Solo");
		g_pSoloPanel->MakeReadyForUse();
	}

	return g_pSoloPanel;
}

CSoloPanel::CSoloPanel(Panel* pParent, const char* pszPanelName)
	: BaseClass(pParent, pszPanelName)
	, m_mapRegions(DefLessFunc(uint32))
	, m_bMapLoaded(false)
{
	// So we can paint the radio needle *on top* of our children
	SetPostChildPaintEnabled(true);

	//m_currentRegion.set_type(DEF_TYPE_QUEST_MAP_REGION);

	if (g_pVGuiLocalize)
	{
		g_pVGuiLocalize->AddFile("resource/tf_quests_%language%.txt");
	}

	Assert(g_pSoloPanel == NULL);
	g_pSoloPanel = this;

	// Item tooltip
	m_pMouseOverItemPanel = vgui::SETUP_PANEL(new CItemModelPanel(this, "mouseoveritempanel"));
	m_pMouseOverTooltip = new CItemModelPanelToolTip(this);
	m_pMouseOverTooltip->SetupPanels(this, m_pMouseOverItemPanel);

	// Text tooltip
	m_pToolTip = new CSoloTooltip(this);
	m_pToolTipEmbeddedPanel = new vgui::EditablePanel(this, "TooltipPanel");
	m_pToolTipEmbeddedPanel->SetKeyBoardInputEnabled(false);
	m_pToolTipEmbeddedPanel->SetMouseInputEnabled(false);
	m_pToolTip->SetEmbeddedPanel(m_pToolTipEmbeddedPanel);
	m_pToolTip->SetTooltipDelay(0);

	m_pMainContainer = new EditablePanel(this, "MainContainer");

	m_pMapAreaPanel = new EditablePanel(m_pMainContainer, "MapAreaPanel");

	// Quest objective tooltip
	m_pQuestObjectiveTooltip = new CSoloObjectiveTooltip(m_pMapAreaPanel, "ObjectiveTooltip");
	m_pQuestObjectivePanel = new CSoloObjectivePanel(m_pMapAreaPanel, "QuestObjective");
	m_pQuestObjectiveTooltip->SetObjectivePanel(m_pQuestObjectivePanel);
	// Node view
	m_pQuestNodeViewPanel = new CSoloNodeViewPanel(m_pMapAreaPanel, "SelectedNodeInfoPanel");
	m_pQuestNodeViewPanel->SetItemModelPanelTooltip(m_pMouseOverTooltip);
	m_pQuestNodeViewPanel->SetTextTooltip(m_pToolTip);
	m_pQuestNodeViewPanel->SetObjectiveTooltip(m_pQuestObjectiveTooltip);

	ListenForGameEvent("gameui_hidden");

	// TODO: Tutorial check here
	m_eScreenDisplay = SCREEN_INVALID;
	ChangeScreenDisplay(SCREEN_STORE); // This needs to be after all the panel pointers are setup

	g_pScriptVM->RegisterInstance(this, "SoloPanel");
}

CSoloPanel::~CSoloPanel()
{
	Assert(g_pSoloPanel == this);
	g_pSoloPanel = NULL;
}

void CSoloPanel::ApplySchemeSettings(IScheme* pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);

	LoadControlSettings("Resource/UI/solo/SoloPanel.res");
}

void CSoloPanel::ApplySettings(KeyValues* inResourceData)
{
	BaseClass::ApplySettings(inResourceData);
}

void CSoloPanel::OnCommand(const char* pCommand)
{
	if (ScriptHookEnabled("solopanel_command"))
	{
		IScriptVM* pVM = g_pScriptVM;
		ScriptVariant_t varTable;
		pVM->CreateTable(varTable);
		pVM->SetValue(varTable, "command", pCommand);
		pVM->SetValue(varTable, "early_out", false);
		if (RunScriptHook("solopanel_command", varTable))
		{
			if (pVM->Get<bool>(varTable, "early_out"))
				return;
		}
	}

	if (FStrEq("close", pCommand))
	{
		SetVisible(false);
		return;
	}
	else if (FStrEq("anim_close", pCommand))
	{
		SetVisible(false);
		return;
	}

	BaseClass::OnCommand(pCommand);
}

void CSoloPanel::ChangeScreenDisplay(EScreenDisplay eScreen)
{
	if (eScreen == m_eScreenDisplay)
		return;
	m_eScreenDisplay = eScreen;
	PlayTransitionScreenEffects();
	InvalidateLayout();
}

void CSoloPanel::UpdateIntroState()
{

}

void CSoloPanel::PerformLayout()
{
	BaseClass::PerformLayout();
}

void CSoloPanel::PostChildPaint()
{
	BaseClass::PostChildPaint();
}

void CSoloPanel::SetVisible(bool bVisible)
{
	if (IsVisible() == bVisible)
		return;

	UpdateControls(bVisible);

	BaseClass::SetVisible(bVisible);

	if (bVisible)
	{
		m_pQuestNodeViewPanel->SetVisible(false);
		PlaySoundEntry("CYOA.MapOpen");
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence(this, "SoloMenu_Start", false);
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence(this, m_bMapLoaded && true ? "SoloMenu_MapLoaded" : "SoloMenu_LoadingLoop", false);
	}
	else
	{
		RunScriptHook("solopanel_closed", NULL);
		ChangeScreenDisplay(SCREEN_STORE);
	}
}

void CSoloPanel::PlayTransitionScreenEffects()
{
	PlaySoundEntry("CYOA.StaticFade");
	g_pClientMode->GetViewportAnimationController()->StartAnimationSequence(this, "SoloMenu_StaticFadeOut", false);
}

void CSoloPanel::QueueTurnInAnims()
{
	
}

void CSoloPanel::FireGameEvent(IGameEvent* event)
{
	if (FStrEq(event->GetName(), "gameui_hidden"))
	{
		// When the gameui hides, we need to hide so we're not still open if the gameui re-opens
		SetVisible(false);
	}
}

void CSoloPanel::UpdatePassAdPanel()
{
	
}

void CSoloPanel::RegionSelected(KeyValues* pParams)
{

}

void CSoloPanel::RegionBackout()
{
	
}

void CSoloPanel::DisableMouseBlocker()
{
	
}


void CSoloPanel::FireTurnInStateEvent(KeyValues* pParams)
{
	
}

void CSoloPanel::OnPlaySoundEntry(KeyValues* pParams)
{
	PlaySoundEntry(pParams->GetString("sound"));
}

void CSoloPanel::MapStateChangeSequence()
{
	
}

void CSoloPanel::OnCursorEntered()
{
	UpdateIntroState();
}

void CSoloPanel::OnCursorExited()
{
	UpdateIntroState();
}

const CSoloRegionPanel* CSoloPanel::GetRegionPanel(uint32 nRegionDefIndex) const
{
	auto idx = m_mapRegions.Find(nRegionDefIndex);
	if (idx == m_mapRegions.InvalidIndex())
		return NULL;

	return m_mapRegions[idx];
}

void CSoloPanel::UpdateControls(bool bIgnoreInvalidLayout)
{
	if (!bIgnoreInvalidLayout && IsLayoutInvalid())
		return;

	// Just got the map loaded.  Transition in
	if (!m_bMapLoaded && IsVisible())
	{
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence(this, "SoloMenu_MapLoaded", false);
	}

	m_bMapLoaded = true;

	InvalidateLayout();
}


void CSoloPanel::UpdateStarsGlobalStatus()
{
	
}

void CSoloPanel::UpdateRegionVisibility()
{
	
}

void CSoloPanel::GoToCurrentQuest()
{
	MakeReadyForUse();
}

// ----------------------------------------------------------------------------

void CSoloPanel::ForceOpen()
{
	if (engine->IsInGame())
	{
		engine->ClientCmd_Unrestricted("gameui_activate");
	}
	GetSoloPanel()->SetVisible(true);
	GetSoloPanel()->GoToCurrentQuest();
}
void CSoloPanel::ForceClose()
{
	if (GetSoloPanel()->IsVisible())
	{
		if (engine->IsInGame())
		{
			engine->ClientCmd_Unrestricted("gameui_hide");
		}
		GetSoloPanel()->SetVisible(false);
	}
}
void CSoloPanel::ForceUpdateControls()
{
	UpdateControls();
}

BEGIN_SCRIPTDESC_ROOT(CSoloPanel, SCRIPT_SINGLETON "Used to access the main solo interface")

DEFINE_SCRIPTFUNC(ForceOpen, "")
DEFINE_SCRIPTFUNC(ForceClose, "")
DEFINE_SCRIPTFUNC(ForceUpdateControls, "")
DEFINE_SCRIPTFUNC(CreatePanel, "")
DEFINE_SCRIPTFUNC(CreatePanelRoot, "")
DEFINE_SCRIPTFUNC(DeleteSubPanel, "")
DEFINE_SCRIPTFUNC(ClearAllScriptPanels, "")
DEFINE_SCRIPTFUNC(PlayTransitionScreenEffects, "")

END_SCRIPTDESC();

BEGIN_SCRIPTDESC_ROOT(Panel, "Used to access a generic panel")

DEFINE_SCRIPTFUNC(SetName, "")
DEFINE_SCRIPTFUNC(GetName, "")
DEFINE_SCRIPTFUNC(GetClassName, "")
DEFINE_SCRIPTFUNC(MakeReadyForUse, "")
DEFINE_SCRIPTFUNC(SetPos, "")
//DEFINE_SCRIPTFUNC(GetPos, "")
DEFINE_SCRIPTFUNC(GetXPos, "")
DEFINE_SCRIPTFUNC(GetYPos, "")
DEFINE_SCRIPTFUNC(SetSize, "")
//DEFINE_SCRIPTFUNC(GetSize, "")
DEFINE_SCRIPTFUNC(SetBounds, "")
//DEFINE_SCRIPTFUNC(GetBounds, "")
DEFINE_SCRIPTFUNC(GetWide, "")
DEFINE_SCRIPTFUNC(SetWide, "")
DEFINE_SCRIPTFUNC(GetTall, "")
DEFINE_SCRIPTFUNC(SetTall, "")
DEFINE_SCRIPTFUNC(SetMinimumSize, "")
//DEFINE_SCRIPTFUNC(GetMinimumSize, "")
DEFINE_SCRIPTFUNC(IsBuildModeEditable, "")
DEFINE_SCRIPTFUNC(SetBuildModeEditable, "")
DEFINE_SCRIPTFUNC(IsBuildModeDeletable, "")
DEFINE_SCRIPTFUNC(SetBuildModeDeletable, "")
DEFINE_SCRIPTFUNC(IsBuildModeActive, "")
DEFINE_SCRIPTFUNC(SetZPos, "")
DEFINE_SCRIPTFUNC(GetZPos, "")
DEFINE_SCRIPTFUNC(SetAlpha, "")
DEFINE_SCRIPTFUNC(GetAlpha, "")
DEFINE_SCRIPTFUNC(SetVisible, "")
DEFINE_SCRIPTFUNC(IsVisible, "")

DEFINE_SCRIPTFUNC(IsWithin, "")
//DEFINE_SCRIPTFUNC(LocalToScreen, "")
//DEFINE_SCRIPTFUNC(ScreenToLocal, "")
//DEFINE_SCRIPTFUNC(ParentLocalToScreen, "")

DEFINE_SCRIPTFUNC(GetChildCount, "")
DEFINE_SCRIPTFUNC(FindChildIndexByName, "")
//DEFINE_SCRIPTFUNC(CallParentFunction, "")

DEFINE_SCRIPTFUNC(SetAutoDelete, "")
DEFINE_SCRIPTFUNC(IsAutoDeleteSet, "")
DEFINE_SCRIPTFUNC(DeletePanel, "")


END_SCRIPTDESC();

BEGIN_SCRIPTDESC(EditablePanel, Panel, "")
END_SCRIPTDESC();
BEGIN_SCRIPTDESC(CExScrollingEditablePanel, Panel, "")
END_SCRIPTDESC();
BEGIN_SCRIPTDESC(ScrollBar, Panel, "")
END_SCRIPTDESC();
BEGIN_SCRIPTDESC(CExLabel, Panel, "")
END_SCRIPTDESC();
BEGIN_SCRIPTDESC(CExImageButton, Panel, "")
END_SCRIPTDESC();
BEGIN_SCRIPTDESC(ImagePanel, Panel, "")
END_SCRIPTDESC();
BEGIN_SCRIPTDESC(CTFImagePanel, Panel, "")
END_SCRIPTDESC();
BEGIN_SCRIPTDESC(CBaseModelPanel, Panel, "")
END_SCRIPTDESC();
BEGIN_SCRIPTDESC(CItemModelPanel, Panel, "")
END_SCRIPTDESC();
BEGIN_SCRIPTDESC(CTFPlayerModelPanel, Panel, "")
END_SCRIPTDESC();
BEGIN_SCRIPTDESC(CTFVideoPanel, Panel, "")
END_SCRIPTDESC();
BEGIN_SCRIPTDESC(CExRichText, Panel, "")
END_SCRIPTDESC();
BEGIN_SCRIPTDESC(CRichTextWithScrollbarBorders, Panel, "")
END_SCRIPTDESC();
BEGIN_SCRIPTDESC(Label, Panel, "")
END_SCRIPTDESC();

void CSoloPanel::ClearAllScriptPanels()
{
	FOR_EACH_VEC(m_scriptPanels, pIter)
	{
		ScriptPanelData item = m_scriptPanels[pIter];
		g_pScriptVM->RemoveInstance(item.m_Handle);
		item.m_Panel->DeletePanel();
	}
	m_scriptPanels.Purge();
}

void CSoloPanel::DeleteSubPanel(HSCRIPT hPanel)
{
	FOR_EACH_VEC(m_scriptPanels, pIter)
	{
		ScriptPanelData item = m_scriptPanels[pIter];
		if (item.m_Handle == hPanel)
		{
			g_pScriptVM->RemoveInstance(item.m_Handle);
			item.m_Panel->DeletePanel();
			return;
		}
	}
}

HSCRIPT CSoloPanel::CreatePanel(HSCRIPT hTable, HSCRIPT hParentTarget)
{
	FOR_EACH_VEC(m_scriptPanels, pIter)
	{
		ScriptPanelData item = m_scriptPanels[pIter];
		if (item.m_Handle == hParentTarget)
		{
			return CreatePanelInternal(hTable, item.m_Panel);
		}
	}
	return NULL;
}

HSCRIPT CSoloPanel::CreatePanelRoot(HSCRIPT hTable)
{
	return CreatePanelInternal(hTable, NULL);
}

HSCRIPT CSoloPanel::CreatePanelInternal(HSCRIPT hTable, Panel* hParentTarget)
{
	KeyValues* hKV = ScriptTableToKeyValues(g_pScriptVM, "PanelSettings", hTable);
	const char* pszPanelType = hKV->GetString("ControlName", "Panel");
	if (!pszPanelType)
	{
		return NULL;
	}
	Panel* hParent = this;
	if (hParentTarget)
	{
		hParent = hParentTarget;
	}

	Panel* pPanel;
	ScriptClassDesc_t* pDesc = GetScriptDescForClass(Panel);
	const char* pszControlName = hKV->GetString("fieldName", "NoName");
	if (FStrEq(pszPanelType, "EditablePanel"))
	{
		EditablePanel* pBasePanel = new EditablePanel(hParent, pszControlName);
		pPanel = pBasePanel;
		pDesc = GetScriptDescForClass(EditablePanel);
	}
	else if (FStrEq(pszPanelType, "Panel"))
	{
		Panel* pBasePanel = new Panel(hParent, pszControlName);
		pPanel = pBasePanel;
	}
	else if (FStrEq(pszPanelType, "CExScrollingEditablePanel"))
	{
		CExScrollingEditablePanel* pBasePanel = new CExScrollingEditablePanel(hParent, pszControlName);
		pPanel = pBasePanel;
		pDesc = GetScriptDescForClass(CExScrollingEditablePanel);
	}
	else if (FStrEq(pszPanelType, "ScrollBar"))
	{
		int isVertical = hKV->GetInt("isVertical");
		ScrollBar* pBasePanel = new ScrollBar(hParent, pszControlName, isVertical != 0);
		pPanel = pBasePanel;
		pDesc = GetScriptDescForClass(ScrollBar);
	}
	else if (FStrEq(pszPanelType, "CExLabel"))
	{
		CExLabel* pLabel = new CExLabel(hParent, pszControlName, (const char*)NULL);
		pPanel = pLabel;
		pDesc = GetScriptDescForClass(CExLabel);
	}
	else if (FStrEq(pszPanelType, "CExImageButton"))
	{
		CExImageButton* pButton = new CExImageButton(hParent, pszControlName, (const char*)NULL, this);
		pPanel = pButton;
		pDesc = GetScriptDescForClass(CExImageButton);
	}
	else if (FStrEq(pszPanelType, "ImagePanel"))
	{
		ImagePanel* pLabel = new ImagePanel(hParent, pszControlName);
		pPanel = pLabel;
		pDesc = GetScriptDescForClass(ImagePanel);
	}
	else if (FStrEq(pszPanelType, "CTFImagePanel"))
	{
		CTFImagePanel* pLabel = new CTFImagePanel(hParent, pszControlName);
		pPanel = pLabel;
		pDesc = GetScriptDescForClass(CTFImagePanel);
	}
	else if (FStrEq(pszPanelType, "CBaseModelPanel"))
	{
		CBaseModelPanel* pButton = new CBaseModelPanel(hParent, pszControlName);
		pPanel = pButton;
		pDesc = GetScriptDescForClass(CBaseModelPanel);
	}
	else if (FStrEq(pszPanelType, "CItemModelPanel"))
	{
		CItemModelPanel* pButton = new CItemModelPanel(hParent, pszControlName);
		pPanel = pButton;
		pDesc = GetScriptDescForClass(CItemModelPanel);
	}
	else if (FStrEq(pszPanelType, "CTFPlayerModelPanel"))
	{
		CTFPlayerModelPanel* pButton = new CTFPlayerModelPanel(hParent, pszControlName);
		pPanel = pButton;
		pDesc = GetScriptDescForClass(CTFPlayerModelPanel);
	}
	else if (FStrEq(pszPanelType, "VideoPanel"))
	{
		CTFVideoPanel* pVideo = new CTFVideoPanel(hParent, pszControlName);
		pPanel = pVideo;
		pDesc = GetScriptDescForClass(CTFVideoPanel);
	}
	else if (FStrEq(pszPanelType, "CExRichText"))
	{
		CExRichText* pVideo = new CExRichText(hParent, pszControlName);
		pPanel = pVideo;
		pDesc = GetScriptDescForClass(CExRichText);
	}
	else if (FStrEq(pszPanelType, "CRichTextWithScrollbarBorders"))
	{
		CRichTextWithScrollbarBorders* pVideo = new CRichTextWithScrollbarBorders(hParent, pszControlName);
		pPanel = pVideo;
		pDesc = GetScriptDescForClass(CRichTextWithScrollbarBorders);
	}
	else if (FStrEq(pszPanelType, "Label"))
	{
		Label* pVideo = new Label(hParent, pszControlName, (const char*)NULL);
		pPanel = pVideo;
		pDesc = GetScriptDescForClass(Label);
	}

	if (!pPanel)
	{
		return NULL;
	}

	pPanel->ApplySettings(hKV);

	char szName[1024];
	g_pScriptVM->GenerateUniqueKey((pszControlName != NULL_STRING) ? STRING(pszControlName) : pszPanelType, szName, 1024);
	string_t m_iszScriptId = AllocPooledString(szName);

	HSCRIPT m_hScriptInstance = g_pScriptVM->RegisterInstance(pDesc, pPanel);
	g_pScriptVM->SetInstanceUniqeId(m_hScriptInstance, STRING(m_iszScriptId));

	ScriptPanelData PanelData;
	PanelData.m_Handle = m_hScriptInstance;
	PanelData.m_Panel = pPanel;

	m_scriptPanels.AddToTail(PanelData);

	return m_hScriptInstance;
}

CON_COMMAND(tfsolo_show_menu, "Show the solo menu")
{
	if (GetSoloPanel()->IsVisible())
	{
		if (engine->IsInGame())
		{
			engine->ClientCmd_Unrestricted("gameui_hide");
		}
		GetSoloPanel()->SetVisible(false);
	}
	else
	{
		if (engine->IsInGame())
		{
			engine->ClientCmd_Unrestricted("gameui_activate");
		}
		GetSoloPanel()->SetVisible(true);
		GetSoloPanel()->GoToCurrentQuest();
	}
}
CON_COMMAND(tfsolo_reset_menu, "Reset the solo menu")
{
	GetSoloPanel()->SetVisible(false);
	GetSoloPanel()->ClearAllScriptPanels();
}