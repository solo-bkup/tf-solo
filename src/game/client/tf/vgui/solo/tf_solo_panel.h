#include "vgui_controls/EditablePanel.h"
#include "tf_controls.h"
#include "vscript_client.h"
#include "vscript_utils.h"

using namespace vgui;

class CItemModelPanel;
class CItemModelPanelToolTip;
class CSoloNodeViewPanel;
class CSoloRegionPanel;
class CTFVideoPanel;
class CExButton;
class CSoloObjectiveTooltip;
class CSoloObjectivePanel;

namespace vgui
{
	class Slider;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CSoloPanel : public EditablePanel
	, public CGameEventListener
{
	DECLARE_CLASS_SIMPLE(CSoloPanel, EditablePanel);
public:
	CSoloPanel(Panel* pParent, const char* pszPanelName);
	~CSoloPanel();

	virtual void ApplySchemeSettings(IScheme* pScheme) OVERRIDE;
	virtual void ApplySettings(KeyValues* inResourceData) OVERRIDE;
	virtual void OnCommand(const char* pCommand) OVERRIDE;
	virtual void PerformLayout() OVERRIDE;
	virtual void PostChildPaint() OVERRIDE;
	virtual void SetVisible(bool bVisible) OVERRIDE;

	virtual void FireGameEvent(IGameEvent* event) OVERRIDE;

	MESSAGE_FUNC(QueueTurnInAnims, "QueueTurnInAnims");
	MESSAGE_FUNC_PARAMS(RegionSelected, "RegionSelected", pParams);
	MESSAGE_FUNC(RegionBackout, "RegionBackout");
	MESSAGE_FUNC(DisableMouseBlocker, "DisableMouseBlocker");
	MESSAGE_FUNC_PARAMS(FireTurnInStateEvent, "FireTurnInStateEvent", pParams);
	MESSAGE_FUNC_PARAMS(OnPlaySoundEntry, "PlaySoundEntry", pParams);

	virtual void OnCursorEntered();
	virtual void OnCursorExited();

	const CSoloRegionPanel* GetRegionPanel(uint32 nRegionDefIndex) const;

	CTFTextToolTip* GetTextTooltip() const { return m_pToolTip; }

	void GoToCurrentQuest();
	void ClearAllScriptPanels();
	void ForceOpen();
	void ForceClose();
	void ForceUpdateControls();
	void RunAnimationScript(const char* pszScript, bool bCanBeCancelled);
	virtual HSCRIPT CreatePanel(HSCRIPT hTable, const char* hParent);
	virtual HSCRIPT CreatePanelRoot(HSCRIPT hTable);
	virtual HSCRIPT CreatePanelInternal(HSCRIPT hTable, Panel* hParent);
	virtual void DeleteSubPanel(const char* hPanel);

	void MapStateChangeSequence();
	//void SetRegion(const CQuestMapRegion* pRegion, bool bZoomIn);
	void UpdateIntroState();
	void UpdateControls(bool bIgnoreInvalidLayout = false);
	void UpdateRegionVisibility();
	void PlayTransitionScreenEffects();
	void UpdatePassAdPanel();
	void UpdateStarsGlobalStatus();

	enum EScreenDisplay
	{
		SCREEN_INVALID,
		SCREEN_MAP,
		SCREEN_STORE,
	};
	void ChangeScreenDisplay(EScreenDisplay eScreen);

	struct ScriptPanelData
	{
		HSCRIPT m_Handle;
		Panel* m_Panel;
		bool m_RootChild;
	};

private:
	
	CSoloObjectiveTooltip* m_pQuestObjectiveTooltip;
	CSoloObjectivePanel* m_pQuestObjectivePanel;
	CSoloNodeViewPanel* m_pQuestNodeViewPanel;
	CItemModelPanel* m_pMouseOverItemPanel;
	CItemModelPanelToolTip* m_pMouseOverTooltip; // The map needs to own this so things will be sorted correctly
	CTFTextToolTip* m_pToolTip;
	vgui::EditablePanel* m_pMainContainer;
	vgui::EditablePanel* m_pToolTipEmbeddedPanel;
	vgui::EditablePanel* m_pMapAreaPanel;

	CUtlMap< uint32, CSoloRegionPanel* > m_mapRegions;
	CUtlVector< ScriptPanelData > m_scriptPanels;
	//CMsgProtoDefID	m_currentRegion;

	bool m_bTurnInSuccess = false;
	bool m_bAwaitingItemConfirm = false;
	bool m_bMapLoaded;
	EScreenDisplay m_eScreenDisplay;

};

CSoloPanel* GetSoloPanel();
