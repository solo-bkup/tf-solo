#ifndef TF_SOLO_PANEL_H
#define TF_SOLO_PANEL_H


#include "vgui_controls/EditablePanel.h"
#include "local_steam_shared_object_listener.h"
#include "tf_proto_def_messages.h"
#include "tf_controls.h"
#include "item_ad_panel.h"
#include "tf_solo_panel.h"
#include "vgui/tf_quest_map_panel.h"

using namespace vgui;
using namespace GCSDK;

class CItemModelPanel;
class CItemModelPanelToolTip;
class CQuestNodeViewPanel;
class CQuestMapRegionPanel;
class CTFVideoPanel;
class CExButton;
class CQuestObjectiveTooltip;
class CQuestObjectivePanel;

namespace vgui
{
	class Slider;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CSoloPanel : public EditablePanel
	, public CGameEventListener
	, public CLocalSteamSharedObjectListener
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

	const CQuestMapRegionPanel* GetRegionPanel(uint32 nRegionDefIndex) const;

	CTFTextToolTip* GetTextTooltip() const { return m_pToolTip; }

	virtual void SOCreated(const CSteamID& steamIDOwner, const GCSDK::CSharedObject* pObject, GCSDK::ESOCacheEvent eEvent) OVERRIDE;

	void GoToCurrentQuest();
private:
	void MapStateChangeSequence();
	void SetRegion(const CQuestMapRegion* pRegion, bool bZoomIn);
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

	CQuestObjectiveTooltip* m_pQuestObjectiveTooltip;
	CQuestObjectivePanel* m_pQuestObjectivePanel;
	CQuestNodeViewPanel* m_pQuestNodeViewPanel;
	CItemModelPanel* m_pMouseOverItemPanel;
	CItemModelPanelToolTip* m_pMouseOverTooltip; // The map needs to own this so things will be sorted correctly
	CTFTextToolTip* m_pToolTip;
	vgui::EditablePanel* m_pMainContainer;
	vgui::EditablePanel* m_pToolTipEmbeddedPanel;
	vgui::EditablePanel* m_pIntroPanel;
	vgui::EditablePanel* m_pMapAreaPanel;
	vgui::EditablePanel* m_pTurnInCompletePopup;
	CExImageButton* m_pRewardsStoreButton;
	CExImageButton* m_pMapButton;
	CExImageButton* m_pPowerSwitch;
	CCyclingAdContainerPanel* m_pAdPanel;

	CUtlMap< uint32, CQuestMapRegionPanel* > m_mapRegions;
	CMsgProtoDefID	m_currentRegion;

	bool m_bTurnInSuccess = false;
	bool m_bAwaitingItemConfirm = false;
	bool m_bMapLoaded;
	bool m_bViewingTutorial;
	EScreenDisplay m_eScreenDisplay;

	CTFVideoPanel* m_pVideoPanel;

	enum EIntroState
	{
		STATE_0 = 0,
		STATE_1,
		STATE_2,
		STATE_3,
		NUM_INTRO_STATES = STATE_3,
	};
	EIntroState m_eIntroState;

	struct IntroStage_t
	{
		vgui::EditablePanel* m_pStagePanel;
		CExImageButton* m_pHoverButton;
	};
	IntroStage_t m_IntroStages[NUM_INTRO_STATES];

	KeyValues* m_pKVRewardItemPanels;
	EditablePanel* m_pRewardsShopPanel;

	CPanelAnimationVar(float, m_flTunerPos, "tuner_pos", "0");
	CPanelAnimationVar(float, m_flTunerWobble, "tuner_wobble", "0");
	float m_flNextWobbleTime;

};

CSoloPanel* GetSoloPanel();

#endif //TF_SOLO_PANEL_H
