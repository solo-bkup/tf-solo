#include "vgui_controls/EditablePanel.h"
#include "tf_controls.h"

using namespace vgui;

extern const float node_medium_radius;
extern const float node_large_radius;

void DrawAmbientActiveCirlceSolo(float flXPos, float flYPos, const Color& color);

//-----------------------------------------------------------------------------
// Purpose: A node on the solo map
//-----------------------------------------------------------------------------
class CSoloNodePanel : public vgui::EditablePanel
{
public:

	enum EMapState
	{
		NEUTRAL,
		MOUSE_OVER,
		SELECTED,
	};

	DECLARE_CLASS_SIMPLE(CSoloNodePanel, vgui::EditablePanel);
	CSoloNodePanel(uint32 nDefIndex, Panel* pParent, const char* pszPanelName);
	~CSoloNodePanel();

	virtual void ApplySchemeSettings(IScheme* pScheme) OVERRIDE;
	virtual void ApplySettings(KeyValues* inResourceData);
	virtual void OnCommand(const char* pCommand) OVERRIDE;
	virtual void PerformLayout() OVERRIDE;
	virtual void OnThink() OVERRIDE;
	virtual void Paint() OVERRIDE;
	virtual void OnCursorEntered() OVERRIDE;
	virtual void OnCursorExited() OVERRIDE;
	virtual void OnMousePressed(MouseCode code) OVERRIDE;
	virtual void OnMouseDoublePressed(MouseCode code) OVERRIDE;

	MESSAGE_FUNC_PARAMS(UpdateStateVisuals, "UpdateStateVisuals", pKVParams);

	EMapState GetState() const { return m_eMapState; }
	void EnterMapState(EMapState eMapState);
	bool BRequirementsMet() const { return m_bRequirementsMet; }

	void DrawNode(float flXPos,
		float flYPos,
		bool bPurchased,
		const Color& colorActive,
		const Color& colorBonus,
		const Color& colorInactive,
		float flScale) const;
private:
	static uint32 m_nDraggingID;

	EMapState m_eMapState;
	float m_flMapStateEnterTime;

	CExButton* m_pSelectButton;
	Label* m_pNameLabel;
	ImagePanel* m_pStarCostImage;
	bool m_bOverSelected;
	bool m_bRequirementsMet;
	bool m_bBaselineSet = false;

	int m_nStartWide;
	int m_nStartTall;
};

//-----------------------------------------------------------------------------
// Purpose: Tooltip to hold the tooltip panel
//-----------------------------------------------------------------------------
class CSoloNodeTooltip : public vgui::BaseTooltip, public vgui::EditablePanel
{
	DECLARE_CLASS_SIMPLE(CSoloNodeTooltip, vgui::EditablePanel);
public:
	CSoloNodeTooltip(vgui::Panel* pParent);

	virtual void ShowTooltip(Panel* pCurrentPanel) OVERRIDE;
	virtual void HideTooltip() OVERRIDE;
	virtual void PerformLayout() OVERRIDE;

	virtual void PositionWindow(Panel* pTipPanel) OVERRIDE;
	virtual void ApplySchemeSettings(IScheme* pScheme) OVERRIDE;

private:

	CSoloNodePanel* m_pFocusedNode;
};
