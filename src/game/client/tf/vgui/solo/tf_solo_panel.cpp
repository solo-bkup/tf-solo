#include "cbase.h"
#include "tf_quest_map_panel.h"
#include "tf_quest_map_node_panel.h"
#include "tf_quest_map.h"
#include <vgui/IInput.h>
#include "tf_gc_client.h"
#include "tf_quest_map_node.h"
#include "tf_quest_map_utils.h"
#include "tf_quest_map_controller.h"
#include "tf_quest_map_editor_panel.h"
#include "tf_quest_map_region_panel.h"
#include "clientmode_tf.h"
#include <vgui_controls/AnimationController.h>
#include "item_model_panel.h"
#include "tf_quest_map_node_view_panel.h"
#include "econ_item_inventory.h"
#include "tf_vgui_video.h"
#include "econ/econ_ui.h"
#include "store/store_panel.h"
#include "tf_item_inventory.h"
#include "tf_matchmaking_dashboard.h"
#include "tf_hud_mainmenuoverride.h"
#include "c_tf_player.h"
#include "vguicenterprint.h"
#include "tf_solo_panel.h"

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
	, m_bViewingTutorial(false)
	, m_eIntroState(STATE_0)
	, m_pKVRewardItemPanels(NULL)
	, m_flNextWobbleTime(0.f)
	, m_pAdPanel(NULL)
{
	// So we can paint the radio needle *on top* of our children
	SetPostChildPaintEnabled(true);

	m_currentRegion.set_type(DEF_TYPE_QUEST_MAP_REGION);

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
	m_pRewardsStoreButton = new CExImageButton(m_pMainContainer, "RewardsStoreButton", (const char*)NULL, this);
	m_pMapButton = new CExImageButton(m_pMainContainer, "MapButton", (const char*)NULL, this);
	m_pPowerSwitch = new CExImageButton(m_pMainContainer, "PowerSwitchButton", (const char*)NULL, this);

	m_pMapAreaPanel = new EditablePanel(m_pMainContainer, "MapAreaPanel");
	m_pTurnInCompletePopup = new EditablePanel(m_pMapAreaPanel, "TurnInCompletePopup");

	m_pAdPanel = new CCyclingAdContainerPanel(m_pMapAreaPanel, "CyclingAd");

	// Quest objective tooltip
	m_pQuestObjectiveTooltip = new CQuestObjectiveTooltip(m_pMapAreaPanel, "ObjectiveTooltip");
	m_pQuestObjectivePanel = new CQuestObjectivePanel(m_pMapAreaPanel, "QuestObjective");
	m_pQuestObjectiveTooltip->SetObjectivePanel(m_pQuestObjectivePanel);
	// Node view
	m_pQuestNodeViewPanel = new CQuestNodeViewPanel(m_pMapAreaPanel, "SelectedNodeInfoPanel");
	m_pQuestNodeViewPanel->SetItemModelPanelTooltip(m_pMouseOverTooltip);
	m_pQuestNodeViewPanel->SetTextTooltip(m_pToolTip);
	m_pQuestNodeViewPanel->SetObjectiveTooltip(m_pQuestObjectiveTooltip);

	// Rewards Store
	m_pRewardsShopPanel = new EditablePanel(m_pMapAreaPanel, "RewardsShop");

	m_pIntroPanel = new EditablePanel(m_pMapAreaPanel, "Introduction");
	m_IntroStages[STATE_0].m_pStagePanel = new EditablePanel(m_pIntroPanel, "IntroStage1");
	m_IntroStages[STATE_1].m_pStagePanel = new EditablePanel(m_pIntroPanel, "IntroStage2");
	m_IntroStages[STATE_2].m_pStagePanel = new EditablePanel(m_pIntroPanel, "IntroStage3");
	m_pVideoPanel = new CTFVideoPanel(m_pIntroPanel, "VideoPanel");
	m_IntroStages[STATE_0].m_pHoverButton = new CExImageButton(m_pIntroPanel, "HoverButtonStage1", (const char*)NULL, this);
	m_IntroStages[STATE_0].m_pHoverButton->PassMouseTicksTo(this, true);

	m_IntroStages[STATE_1].m_pHoverButton = new CExImageButton(m_pIntroPanel, "HoverButtonStage2", (const char*)NULL, this);
	m_IntroStages[STATE_1].m_pHoverButton->PassMouseTicksTo(this, true);

	m_IntroStages[STATE_2].m_pHoverButton = new CExImageButton(m_pIntroPanel, "HoverButtonStage3", (const char*)NULL, this);
	m_IntroStages[STATE_2].m_pHoverButton->PassMouseTicksTo(this, true);

	ListenForGameEvent("proto_def_changed");
	ListenForGameEvent("gameui_hidden");
	ListenForGameEvent("quest_request");
	ListenForGameEvent("quest_response");
	ListenForGameEvent("quest_map_data_changed");
	ListenForGameEvent("gc_new_session");
	ListenForGameEvent("quest_turn_in_state");
	ListenForGameEvent("items_acknowledged");

	// TODO: Tutorial check here
	m_eScreenDisplay = SCREEN_INVALID;
	ChangeScreenDisplay(SCREEN_STORE); // This needs to be after all the panel pointers are setup
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

	if (m_pKVRewardItemPanels)
	{
		m_pKVRewardItemPanels->deleteThis();
		m_pKVRewardItemPanels = NULL;
	}

	KeyValues* pKVRewardKV = inResourceData->FindKey("RewardItemKV");
	if (pKVRewardKV)
	{
		m_pKVRewardItemPanels = pKVRewardKV->MakeCopy();
	}
}

void CSoloPanel::OnCommand(const char* pCommand)
{
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
	else if (FStrEq("endintro", pCommand))
	{
		m_bViewingTutorial = false;
		UpdateControls();
		m_pIntroPanel->SetVisible(false);
	}
	else if (FStrEq("rewards_store", pCommand))
	{
		ChangeScreenDisplay(SCREEN_STORE);
		return;
	}
	else if (FStrEq("view_map", pCommand))
	{
		ChangeScreenDisplay(SCREEN_MAP);
		return;
	}
	else if (FStrEq("selectteamred", pCommand))
	{
		ChangeScreenDisplay(SCREEN_MAP);
		return;
	}
	else if (FStrEq("selectteamblue", pCommand))
	{
		ChangeScreenDisplay(SCREEN_MAP);
		return;
	}
	else if (FStrEq("update_region_visiblity", pCommand))
	{
		PlayTransitionScreenEffects();
		UpdateRegionVisibility();
		return;
	}

	BaseClass::OnCommand(pCommand);
}

void CSoloPanel::ChangeScreenDisplay(EScreenDisplay eScreen)
{

	switch (eScreen)
	{
	case SCREEN_MAP:
	{
		// Nothing needed
	}
	break;

	case SCREEN_STORE:
	{
		CExScrollingEditablePanel* pItemScroller = m_pRewardsShopPanel->FindControl< CExScrollingEditablePanel >("ItemsScroller", true);
		if (pItemScroller)
		{
			pItemScroller->ResetScrollAmount();
		}

		// Have the rewards re-evaluate their state
		//FOR_EACH_VEC(m_vecRewardsShopItemPanels, i)
		//{
		//	m_vecRewardsShopItemPanels[i]->InvalidateLayout();
		//}

		// Hide the node view
		m_pQuestNodeViewPanel->SetVisible(false);
	}
	break;
	};

	m_pRewardsStoreButton->SetSelected(eScreen == SCREEN_STORE);
	m_pRewardsStoreButton->SetMouseInputEnabled(eScreen != SCREEN_STORE);
	m_pMapButton->SetSelected(eScreen == SCREEN_MAP);
	m_pMapButton->SetMouseInputEnabled(eScreen != SCREEN_MAP);

	// Not changing anything
	if (eScreen == m_eScreenDisplay)
		return;

	m_eScreenDisplay = eScreen;

	UpdatePassAdPanel();

	PlayTransitionScreenEffects();
	InvalidateLayout();
}

void CSoloPanel::UpdateIntroState()
{
	// Be default STATE_0 when not mousing over a button
	EIntroState eNewIntroState = STATE_0;

	for (int eState = STATE_1; eState <= NUM_INTRO_STATES; ++eState)
	{
		if (m_IntroStages[eState - 1].m_pHoverButton->IsArmed())
		{
			eNewIntroState = (EIntroState)eState;
			m_pVideoPanel->BeginPlayback(CFmtStr("media/cyoa_intro_stage%d.vid", eState));
			m_pVideoPanel->SetVisible(true);
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence(m_IntroStages[eState - 1].m_pStagePanel, "QuestMapIntro_StageReveal", false);
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence(m_pIntroPanel, "QuestMapIntro_ShowStage", false);
		}
	}


	if (eNewIntroState == STATE_0 && m_eIntroState != STATE_0)
	{
		m_pVideoPanel->SetVisible(false);
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence(m_pIntroPanel, "QuestMapIntro_ClearStage", false);
	}

	m_pIntroPanel->SetControlVisible("IntroStage0", eNewIntroState == STATE_0);
	m_pIntroPanel->SetControlVisible("IntroStage1", eNewIntroState == STATE_1);
	m_pIntroPanel->SetControlVisible("IntroStage2", eNewIntroState == STATE_2);
	m_pIntroPanel->SetControlVisible("IntroStage3", eNewIntroState == STATE_3);

	m_eIntroState = eNewIntroState;
}

void CSoloPanel::PerformLayout()
{
	BaseClass::PerformLayout();

	Panel* pRewardsShopPanel = m_pMapAreaPanel->FindChildByName("RewardsShop", true);
	if (pRewardsShopPanel)
	{
		pRewardsShopPanel->SetVisible(m_eScreenDisplay == SCREEN_STORE);
	}

	UpdateRegionVisibility();
	UpdateStarsGlobalStatus();

	/*
	CExScrollingEditablePanel* pItemScroller = m_pRewardsShopPanel->FindControl< CExScrollingEditablePanel >("ItemsScroller", true);
	Assert(pItemScroller);
	if (!pItemScroller)
		return;
	*/

	//FOR_EACH_VEC(m_vecRewardsShopItemPanels, i)
	//{
	//	Panel* pRewardItem = m_vecRewardsShopItemPanels[i];
	//	int nXPos = (i % 2) == 0 ? XRES(10) : m_pRewardsShopPanel->GetWide() - pRewardItem->GetWide() - XRES(10);
	//	int nYPos = (i / 2) * (pRewardItem->GetTall() + YRES(10)) + YRES(5) - pItemScroller->GetScrollAmount();
	//	pRewardItem->SetPos(nXPos, nYPos);
	//}
}

void CSoloPanel::PostChildPaint()
{
	BaseClass::PostChildPaint();

	static int snWhiteTextureID = -1;
	if (snWhiteTextureID == -1)
	{
		snWhiteTextureID = vgui::surface()->CreateNewTextureID();
		vgui::surface()->DrawSetTextureFile(snWhiteTextureID, "vgui/white", true, false);
		if (snWhiteTextureID == -1)
			return;
	}

	//
	// Check if we need to re-randomize the needle wobbling
	//
	if (Plat_FloatTime() > m_flNextWobbleTime)
	{
		// A whole bunch of randomness to make the needle wobble
		float flLerpTime = Bias(RandomFloat(0.1f, 1.f), 0.2f);
		m_flNextWobbleTime = Plat_FloatTime() + flLerpTime;

		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(this, "tuner_wobble", RandomFloat(-1.f, 1.f), 0.f, flLerpTime, vgui::AnimationController::INTERPOLATOR_BIAS, RandomFloat(0.25f, 0.75f), true, false);
	}

	//
	// Draw the needle for the radio tuner
	//
	/*
	{
		vgui::surface()->DrawSetTexture(snWhiteTextureID);
		vgui::surface()->DrawSetColor(Color(180, 0, 0, 255));

		int nYPos = YRES(396);
		int nXPos = GetWide() * 0.5 - YRES(80);
		int nStride = YRES(175);
		int nTall = YRES(35);
		int nWide = YRES(3);

		int nX = RemapVal(m_flTunerPos + (m_flTunerWobble * 0.01f), 0.f, 1.f, (float)nXPos, (float)(nXPos + nStride)) - (nWide * 0.5f);

		surface()->DrawFilledRect(nX, nYPos, nX + nWide, nYPos + nTall);
	}
	*/
}

void CSoloPanel::SetVisible(bool bVisible)
{
	if (IsVisible() == bVisible)
		return;

	UpdateControls(bVisible);

	BaseClass::SetVisible(bVisible);

	if (bVisible)
	{
		PlaySoundEntry("CYOA.MapOpen");
		// If they closed and re-opened the quest map, make sure the mouse
		// block is not visible.
		SetControlVisible("MouseBlocker", false);

		m_pQuestNodeViewPanel->SetVisible(false);

		if (true)
		{
			m_pIntroPanel->SetVisible(false);
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence(this, "QuestMap_Start", false);
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence(this, m_bMapLoaded && true ? "QuestMap_MapLoaded" : "QuestMap_LoadingLoop", false);
		}
		else
		{
			g_pClientMode->GetViewportAnimationController()->StartAnimationSequence(this, "QuestMap_Start", false);
			m_pIntroPanel->SetVisible(true);
			m_pIntroPanel->SetControlVisible("IntroStage0", true);
			m_bViewingTutorial = true;
			m_pVideoPanel->BeginPlayback("media/test.vid");
		}
	}
	else
	{
		ChangeScreenDisplay(SCREEN_STORE);
	}
}

void CSoloPanel::PlayTransitionScreenEffects()
{
	PlaySoundEntry("CYOA.StaticFade");
	g_pClientMode->GetViewportAnimationController()->StartAnimationSequence(this, "QuestMap_StaticFadeOut", false);
}

void CSoloPanel::QueueTurnInAnims()
{
	
}

void CSoloPanel::FireGameEvent(IGameEvent* event)
{
	if (FStrEq(event->GetName(), "items_acknowledged"))
	{
		if (m_bAwaitingItemConfirm)
		{
			m_bAwaitingItemConfirm = false;
			EQuestTurnInState eLastQueued = TURN_IN_BEGIN;
			float flTimeAccum = 0.f;
			auto lambdaQueue = [&](EQuestTurnInState eState, float flTimeAfterLastStep)
			{
				flTimeAccum += flTimeAfterLastStep;
				Assert(eState > eLastQueued); // Sanity
				PostMessage(this, new KeyValues("FireTurnInStateEvent", "state", eState), flTimeAccum);
				eLastQueued = eState;
			};

			auto& msgReport = GetQuestMapController().GetMostRecentProgressReport();

			lambdaQueue(TURN_IN_HIDE_NODE_VIEW, 0.5f);
			lambdaQueue(TURN_IN_SHOW_NODE_UNLOCKS, 0.5f);
			if (msgReport.reward_credits_earned() > 0)
			{
				lambdaQueue(TURN_IN_SHOW_GLOBAL_BLOOD_MONEY, 1.f);
			}
			lambdaQueue(TURN_IN_SHOW_GLOBAL_STARS, 1.f);
			lambdaQueue(TURN_IN_COMPLETE, 0.f);
		}
	}
	else if (FStrEq(event->GetName(), "quest_response"))
	{
		if (event->GetInt("request") != k_EMsgGCQuestNodeTurnIn)
			return;

		m_bTurnInSuccess = event->GetBool("success");
	}
	else if (FStrEq(event->GetName(), "quest_request"))
	{
		if (event->GetInt("request") != k_EMsgGCQuestNodeTurnIn)
			return;

		// In 5 seconds, do whatever anims we need to do.  We'll (hopefully) hear back about
		// success within that time.  If we don't, we assume failure.
		PostMessage(this, new KeyValues("QueueTurnInAnims"), k_flQuestTurnInTime);
		PostMessage(this, new KeyValues("FireTurnInStateEvent", "state", TURN_IN_BEGIN), 0.0f);
		m_bTurnInSuccess = false;
	}
	else if (FStrEq(event->GetName(), "quest_map_data_changed"))
	{
		UpdateControls();
		return;
	}
	else if (FStrEq(event->GetName(), "proto_def_changed") && (event->GetInt("type") == DEF_TYPE_QUEST_MAP_NODE))
	{
		bool bVisible = IsVisible();
		UpdateControls();
		SetVisible(bVisible);
	}
	else if (FStrEq(event->GetName(), "gc_new_session"))
	{
		UpdateControls();
		return;
	}
	else if (FStrEq(event->GetName(), "gameui_hidden"))
	{
		// When the gameui hides, we need to hide so we're not still open if the gameui re-opens
		SetVisible(false);
	}
	else if (FStrEq(event->GetName(), "quest_turn_in_state"))
	{
		EQuestTurnInState eState = (EQuestTurnInState)event->GetInt("state");
		auto& msgReport = GetQuestMapController().GetMostRecentProgressReport();

		auto lambdaShowGainsOverPanel = [&](const char* pszPanelName,
			const char* pszToken,
			int nGain)
		{
			Panel* pPanel = m_pMapAreaPanel->FindChildByName(pszPanelName, true);
			if (pPanel && nGain)
			{
				int nX, nY;
				pPanel->GetPos(nX, nY);
				pPanel->ParentLocalToScreen(nX, nY);
				CreateScrollingIndicator(nX,
					nY - YRES(20),
					LocalizeNumberWithToken(pszToken, nGain),
					"MatchMaking.XPChime",
					0.f,
					0,
					-25,
					true);
			}
		};

		switch (eState)
		{
		case TURN_IN_BEGIN:
		{
			// We don't want to allow any input while we're doing turn-in animations
			SetControlVisible("MouseBlocker", true);
			break;
		}

		case TURN_IN_SHOW_SUCCESS:
		{
			PlaySoundEntry("Quest.TurnInAcceptedLight");
			m_pTurnInCompletePopup->SetDialogVariable("result", g_pVGuiLocalize->Find("#TF_QuestView_TurnInSuccess"));
			m_pTurnInCompletePopup->SetVisible(true);
			break;
		}

		case TURN_IN_HIDE_SUCCESS:
		{
			m_pTurnInCompletePopup->SetVisible(false);
			break;
		}

		case TURN_IN_SHOW_STARS_EARNED:
		{
			// This is a hack.  There's a few objective panels that are tying to play this sound
			// so it's stacking and sounding bad.  This is way easier than .res file plumbing
			// a setting.
			float flDelay = 0.f;
			auto lambdaPlayChime = [&]()
			{
				PostMessage(this, new KeyValues("PlaySoundEntry", "sound", "MatchMaking.XPChime"), flDelay);
				flDelay += 0.3f;
			};

			if (msgReport.star_0_earned()) lambdaPlayChime();
			if (msgReport.star_1_earned()) lambdaPlayChime();
			if (msgReport.star_2_earned()) lambdaPlayChime();
			break;
		}

		case TURN_IN_SHOW_BLOOD_MONEY_EARNED:
		{
			break;
		}

		case TURN_IN_SHOW_ITEM_PICKUP_SCREEN:
		{
			m_bAwaitingItemConfirm = true;
			InventoryManager()->ShowItemsPickedUp(true, false);
			PlaySoundEntry("plng_contract_complete_give_item_allclass");
			break;
		}

		case TURN_IN_SHOW_FAILURE:
		{
			m_pTurnInCompletePopup->SetDialogVariable("result", g_pVGuiLocalize->Find("#TF_QuestView_TurnInFailure"));
			m_pTurnInCompletePopup->SetVisible(true);
			break;
		}

		case TURN_IN_HIDE_FAILURE:
		{
			m_pTurnInCompletePopup->SetVisible(false);
			break;
		}

		case TURN_IN_SHOW_GLOBAL_BLOOD_MONEY:
		{
			lambdaShowGainsOverPanel("RewardCreditsLabel", "#TF_QuestMap_BloodMoneyGained", msgReport.reward_credits_earned());
			break;
		}

		case TURN_IN_SHOW_GLOBAL_STARS:
		{
			int nNumStarsEarned = 0;
			if (msgReport.star_0_earned()) nNumStarsEarned += 1;
			if (msgReport.star_1_earned()) nNumStarsEarned += 1;
			if (msgReport.star_2_earned()) nNumStarsEarned += 1;
			Assert(nNumStarsEarned > 0);

			lambdaShowGainsOverPanel("AvailableStarsLabel", "#TF_QuestMap_StarsGained", nNumStarsEarned);
			break;
		}

		case TURN_IN_COMPLETE:
		{
			SetControlVisible("MouseBlocker", false);
			break;
		}

		// Nothing to do for these
		case TURN_IN_HIDE_NODE_VIEW:
		case TURN_IN_SHOW_NODE_UNLOCKS:
			break;
		};
	}
}

void CSoloPanel::SetRegion(const CQuestMapRegion* pRegion, bool bZoomIn)
{
	CQuestMapRegionPanel* pCurrentRegionPanel = m_mapRegions[m_mapRegions.Find(m_currentRegion.defindex())];
	CQuestMapRegionPanel* pNewRegionPanel = m_mapRegions[m_mapRegions.Find(pRegion->GetDefIndex())];
	Assert(pCurrentRegionPanel && pNewRegionPanel);
	if (!pCurrentRegionPanel || !pNewRegionPanel)
		return;

	float flLinkX = 0.5, flLinkY = 0.5;

	const CQuestMapRegionPanel* pLinkContainingPanel = bZoomIn ? pCurrentRegionPanel : pNewRegionPanel;
	uint32 nLinkDefindex = bZoomIn ? pRegion->GetDefIndex() : m_currentRegion.defindex();
	const EditablePanel* pRegionLink = pLinkContainingPanel->GetRegionLinkPanel(nLinkDefindex);

	if (pRegionLink)
	{
		flLinkX = pLinkContainingPanel->GetZoomPanel()->GetChildPositionInfo(pRegionLink)->m_flX;
		flLinkY = pLinkContainingPanel->GetZoomPanel()->GetChildPositionInfo(pRegionLink)->m_flY;
	}

	pCurrentRegionPanel->StartZoomAway(flLinkX, flLinkY, bZoomIn);
	pNewRegionPanel->StartZoomTo(flLinkX, flLinkY, bZoomIn);

	// Run our own animation command to manage things we need to do
	g_pClientMode->GetViewportAnimationController()->StartAnimationSequence(this, "RegionZoom");

	m_currentRegion.set_defindex(pRegion->GetDefIndex());

	// Make the tuner needle move
	{
		float flDestination = pRegion->GetRadioFreq();
		float flTime = tf_quest_map_zoom_transition_in_time * 2; // 2x for zoom-away + zoom-to

		g_pClientMode->GetViewportAnimationController()->RunAnimationCommand(this, "tuner_pos", flDestination, 0.f, flTime, vgui::AnimationController::INTERPOLATOR_BIAS, 0.75f, true, false);
	}

	UpdatePassAdPanel();
}

void CSoloPanel::UpdatePassAdPanel()
{
	
}

void CSoloPanel::RegionSelected(KeyValues* pParams)
{
	uint32 nRegionDefindex = pParams->GetInt("defindex");
	const CQuestMapRegion* pRegion = GetProtoScriptObjDefManager()->GetTypedDefinition< CQuestMapRegion >(nRegionDefindex);
	if (!pRegion)
	{
		Assert(false);
		return;
	}

	SetRegion(pRegion, true);
}

void CSoloPanel::RegionBackout()
{
	// Pop the top!
	const CQuestMapRegion* pRegion = GetProtoScriptObjDefManager()->GetTypedDefinition< CQuestMapRegion >(m_currentRegion.defindex());
	if (!pRegion)
		return;

	const CQuestMapRegion* pParentRegion = pRegion->GetParent();
	if (!pParentRegion)
		return;

	SetRegion(pParentRegion, false);
}

void CSoloPanel::DisableMouseBlocker()
{
	SetControlVisible("MouseBlocker", false);
}


void CSoloPanel::FireTurnInStateEvent(KeyValues* pParams)
{
	auto pEvent = gameeventmanager->CreateEvent("quest_turn_in_state");
	if (pEvent)
	{
		pEvent->SetInt("state", pParams->GetInt("state"));
		gameeventmanager->FireEventClientSide(pEvent);
	}
}

void CSoloPanel::OnPlaySoundEntry(KeyValues* pParams)
{
	PlaySoundEntry(pParams->GetString("sound"));
}

void CSoloPanel::MapStateChangeSequence()
{
	auto pRegion = GetRegionPanel(m_currentRegion.defindex());
	if (!pRegion)
		return;

	PostMessage(pRegion->GetVPanel(), new KeyValues("CloseNodeView"), 0.5f);
	PostMessage(pRegion->GetVPanel(), new KeyValues("ShowNodeUnlockChange"), 1.f);
}

void CSoloPanel::OnCursorEntered()
{
	UpdateIntroState();
}

void CSoloPanel::OnCursorExited()
{
	UpdateIntroState();
}

const CQuestMapRegionPanel* CSoloPanel::GetRegionPanel(uint32 nRegionDefIndex) const
{
	auto idx = m_mapRegions.Find(nRegionDefIndex);
	if (idx == m_mapRegions.InvalidIndex())
		return NULL;

	return m_mapRegions[idx];
}

//
// This is not cheap.  Try to do this as infrequently as possible
//
void CSoloPanel::UpdateControls(bool bIgnoreInvalidLayout)
{
	if (!bIgnoreInvalidLayout && IsLayoutInvalid())
		return;

	if (!steamapicontext ||
		!steamapicontext->SteamUser())
	{
		return;
	}

	static ConVarRef mat_dxlevel("mat_dxlevel");
	if (mat_dxlevel.GetInt() < 90)
	{
		m_pIntroPanel->SetControlVisible("StaticBG", false);
		m_pMapAreaPanel->SetControlVisible("StaticOverlay", false);
	}

	//if ( !GTFGCClientSystem()->BConnectedtoGC() )
	//	return;

	// Maybe they activated a pass?
	UpdatePassAdPanel();

	//
	// Create region panels for this map
	//
	const DefinitionMap_t& mapRegions = GetProtoScriptObjDefManager()->GetDefinitionMapForType(DEF_TYPE_QUEST_MAP_REGION);
	FOR_EACH_MAP_FAST(mapRegions, i)
	{
		const CQuestMapRegion* pRegion = (const CQuestMapRegion*)mapRegions[i];

		auto idx = m_mapRegions.Find(pRegion->GetDefIndex());
		if (idx == m_mapRegions.InvalidIndex())
		{
			CQuestMapRegionPanel* pRegionPanel = new CQuestMapRegionPanel(m_pMapAreaPanel, "Region", pRegion->GetID(), m_pQuestNodeViewPanel);
			pRegionPanel->AddActionSignalTarget(this);
			pRegionPanel->MakeReadyForUse();
			m_mapRegions.Insert(pRegion->GetDefIndex(), pRegionPanel);
		}
	}


	// TODO: Setup starting region.  Flag in the region?  Where your active contract is?
	// Now that we have the map def, we can set the starting region
	if (!m_currentRegion.has_defindex())
	{
		const CQuestMapRegion* pStartingRegion = GetProtoScriptObjDefManager()->GetTypedDefinition< CQuestMapRegion >(0);
		if (!pStartingRegion)
			return;

		m_mapRegions[pStartingRegion->GetDefIndex()]->MakeReadyForUse();
		m_mapRegions[pStartingRegion->GetDefIndex()]->StartZoomTo(0.5f, 0.5f, true);
		m_currentRegion.set_defindex(pStartingRegion->GetDefIndex());
	}


	UpdateRegionVisibility();

	// Just got the map loaded.  Transition in
	if (!m_bMapLoaded && IsVisible())
	{
		g_pClientMode->GetViewportAnimationController()->StartAnimationSequence(this, "QuestMap_MapLoaded", false);
	}

	m_bMapLoaded = true;

	InvalidateLayout();
}


void CSoloPanel::UpdateStarsGlobalStatus()
{
	EditablePanel* pGlobalStatus = FindControl< EditablePanel >("GlobalStatus", true);
	if (pGlobalStatus)
	{
		auto lambdaSetTooltip = [&](const char* pszPanelName, const char* pszLocToken)
		{
			Panel* pPanel = pGlobalStatus->FindChildByName(pszPanelName);
			if (!pPanel)
				return;

			pPanel->SetTooltip(GetDashboardTooltip(k_eSmallFont), pszLocToken);
		};

		//auto pRegionDef = GetProtoScriptObjDefManager()->GetTypedDefinition< CQuestMapRegion >(m_currentRegion);
		bool bShowStars = false;
		//auto pStarTypeDef = pRegionDef->GetStarType();
		//if (pStarTypeDef)
		//{
			bShowStars = true;
			pGlobalStatus->SetDialogVariable("stars_available", CFmtStr(": %d", 5));
			pGlobalStatus->SetDialogVariable("stars_total", CFmtStr(": %d/%d", 123, 124));
		//}

		pGlobalStatus->SetDialogVariable("reward_credits", CFmtStr(": %d", 4444));
		pGlobalStatus->SetControlVisible("AvailableStarsImage", bShowStars);
		pGlobalStatus->SetControlVisible("AvailableStarsLabel", bShowStars);
		pGlobalStatus->SetControlVisible("TotalStarsImage", bShowStars);
		pGlobalStatus->SetControlVisible("TotalStarsLabel", bShowStars);
		lambdaSetTooltip("BloodMoneyTooltip", "#TF_QuestMap_BloodMoney");
		lambdaSetTooltip("StarsAvailableTooltip", "#TF_QuestMap_StarsAvailableTooltip");
		lambdaSetTooltip("TotalStarsTooltip", "#TF_QuestMap_StarsTotalTooltip");
	}
}

void CSoloPanel::UpdateRegionVisibility()
{
	// Make the right region show
	FOR_EACH_MAP(m_mapRegions, i)
	{
		uint32 nKey = m_mapRegions.Key(i);
		m_mapRegions[i]->SetVisible(nKey == m_currentRegion.defindex() && m_eScreenDisplay == SCREEN_MAP);
	}

	UpdateStarsGlobalStatus();
}

void CSoloPanel::SOCreated(const CSteamID& steamIDOwner, const GCSDK::CSharedObject* pObject, GCSDK::ESOCacheEvent eEvent)
{
	
}

void CSoloPanel::GoToCurrentQuest()
{
	MakeReadyForUse();

	auto* pActiveQuest = GetQuestMapHelper().GetActiveQuest();
	if (!pActiveQuest)
		return;

	auto* pNode = GetQuestMapHelper().GetQuestMapNodeByID(pActiveQuest->GetSourceNodeID());
	if (!pNode)
		return;

	auto pRegionDef = GetProtoScriptObjDefManager()->GetTypedDefinition< CQuestMapRegion  >(pNode->GetNodeDefinition()->GetRegionDefIndex());
	if (!pRegionDef)
		return;

	auto idx = m_mapRegions.Find(pRegionDef->GetDefIndex());
	if (idx == m_mapRegions.InvalidIndex())
		return;

	auto* pRegionPanel = m_mapRegions[idx];
	if (!pRegionPanel)
		return;

	// Go to the region with the node
	SetRegion(pRegionDef, true);

	// Delay the command to select the node a bit because we the UI needs to do the transitions first, or else
	// the arror from the node view panel will point to the wrong place
	PostMessage(pRegionPanel, new KeyValues("NodeSelected", "node", pNode->GetNodeDefinition()->GetDefIndex()), 0.5f);
}

//
// Debugging functions
//

CON_COMMAND(tfsolo_show_menu, "Show the solo menu")
{
	if (GetSoloPanel()->IsVisible())
	{
		engine->ClientCmd_Unrestricted("gameui_hide");
		GetSoloPanel()->SetVisible(false);
	}
	else
	{
		engine->ClientCmd_Unrestricted("gameui_activate");
		GetSoloPanel()->SetVisible(true);
		GetSoloPanel()->GoToCurrentQuest();
	}
}