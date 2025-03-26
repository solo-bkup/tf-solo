"Resource/UI/Solo/SoloPanel.res"
{
	"Solo"
	{
		"ControlName"	"CSoloPanel"
		"fieldName"		"QuestMap"
		"xpos"			"0"
		"ypos"			"0"
		"zpos"			"100"
		"wide"			"f0"
		"tall"			"f60"
		"pinCorner"		"0"
		"visible"		"0"
		"enabled"		"1"
		"tabPosition"	"0"
		"PaintBackgroundType"	"0"
		"paintbackground"		"0"
		"skip_autoresize"	"1"

		"RewardItemKV"
		{
			"wide"	"250"
			"tall"	"60"
		}

		"mouseoveritempanel"
		{
			"ControlName"	"CItemModelPanel"
			"fieldName"		"mouseoveritempanel"
			"xpos"			"c-70"
			"ypos"			"270"
			"zpos"			"1000"
			"wide"			"300"
			"tall"			"300"
			"visible"		"0"
			"bgcolor_override"		"0 0 0 0"
			"noitem_textcolor"		"117 107 94 255"
			"PaintBackgroundType"	"2"
			"paintborder"	"1"
			"border"		"MainMenuBGBorder"
		
			"text_center"		"1"
			"model_hide"		"1"
			"resize_to_text"	"1"
			"padding_height"	"15"
		
			"attriblabel"
			{
				"font"			"ItemFontAttribLarge"
				"xpos"			"0"
				"ypos"			"30"
				"zpos"			"2"
				"wide"			"140"
				"tall"			"60"
				"autoResize"	"0"
				"pinCorner"		"0"
				"visible"		"1"
				"enabled"		"1"
				"labelText"		"%attriblist%"
				"textAlignment"	"center"
				"fgcolor"		"117 107 94 255"
				"centerwrap"	"1"
			}
		}

		"TooltipPanel"
		{
			"ControlName"	"EditablePanel"
			"fieldName"		"TooltipPanel"
			"xpos"			"0"
			"ypos"			"0"
			"zpos"			"1001"
			"wide"			"240"
			"tall"			"50"
			"visible"		"0"
			"PaintBackgroundType"	"2"
			"border"		"MainMenuBGBorder"
		
			"TipLabel"
			{
				"ControlName"	"CExLabel"
				"fieldName"		"TipLabel"
				"font"			"HudFontSmallest"
				"labelText"		"%tiptext%"
				"textAlignment"	"center"
				"xpos"			"20"
				"ypos"			"10"
				"zpos"			"2"
				"wide"			"200"
				"tall"			"30"
				"autoResize"	"0"
				"pinCorner"		"0"
				"visible"		"1"
				"enabled"		"1"
				"fgcolor_override"	"235 226 202 255"
				"wrap"			"1"
			}

			"QuestObjective"
			{
				"fieldName"	"QuestObjective"
				"wide"		"200"
				"zpos"		"1002"
			}
		}	

		"Dimmer"
		{
			"ControlName"	"EditablePanel"
			"fieldName"		"Dimmer"
			"xpos"		"0"
			"ypos"		"0"
			"zpos"		"-2"
			"wide"		"f0"
			"tall"		"f60"
			"autoResize"		"0"
			"pinCorner"		"0"
			"visible"		"1"
			"enabled"		"1"
			"tabPosition"		"0"
			"bgcolor_override"	"42 39 37 255"
			//"bgcolor_override"	"20 15 5 230"
		}

		"MainContainer"
		{
			"ControlName"	"EditablePanel"
			"fieldName"		"MainContainer"
			"xpos"			"cs-0.5"
			"ypos"			"20"
			"zpos"			"1"
			"wide"			"700"
			"tall"			"f0"
			"skip_autoresize"	"1"
			"visible"		"1"
			"enabled"		"1"
			"tabPosition"	"0"
			"paintborder"		"0"

			"ScreenBorder"
			{
				"ControlName"	"ImagePanel"
				"fieldName"		"ScreenBorder"
				"xpos"			"cs-0.5-15"
				"ypos"			"cs-0.5+30"
				"zpos"			"1"
				"wide"			"700"
				"tall"			"700"
				"visible"		"0"
				"proportionaltoparent"	"1"
				"mouseinputenabled"		"0"
				"keyboardinputenabled"	"0"

				"image"			"cyoa/cyoa_pda"
				"scaleimage"	"1"
			}

			"MapAreaPanel"
			{
				"ControlName"	"EditablePanel"
				"fieldName"		"MapAreaPanel"
				"xpos"			"cs-0.5"
				"ypos"			"cs-0.5"
				"wide"			"540"
				"tall"			"315"
				"zpos"			"0"
				"proportionaltoparent" "1"
				"mouseinputenabled"	"1"

				"TurnInCompletePopup"
				{
					"ControlName"	"EditablePanel"
					"fieldName"		"TurnInCompletePopup"
					"xpos"			"cs-0.5"
					"ypos"			"cs-0.5"
					"zpos"			"300"
					"wide"			"250"
					"tall"			"150"
					"visible"		"0"
					"proportionaltoparent" "1"
					"mouseinputenabled"		"0"

					"border"		"CYOANodeViewBorder"

					"BorderOverlay"
					{
						"ControlName"	"Panel"
						"fieldName"		"BorderOverlay"
						"xpos"			"0"
						"ypos"			"0"
						"zpos"			"300"
						"wide"			"f0"
						"tall"			"f0"
						"proportionaltoparent" "1"

						"border"		"CYOANodeViewBorder_Active"
					}

					"CheckImage"
					{
						"ControlName"	"ImagePanel"
						"fieldName"		"CheckImage"
						"xpos"			"cs-0.5"
						"ypos"			"25"
						"zpos"			"300"
						"wide"			"o1"
						"tall"			"70"
						"proportionaltoparent" "1"

						"image"			"cyoa/check"
						"scaleimage"	"1"
						"drawcolor_override"	"QuestMap_ActiveOrange"
					}

					"BodyText"
					{
						"ControlName"	"Label"
						"fieldName"		"BodyText"
						"xpos"			"0"
						"ypos"			"90"
						"zpos"			"300"
						"wide"			"f0"
						"tall"			"300"
						"proportionaltoparent" "1"
						"fgcolor_override"	"QuestMap_ActiveOrange"
						"labeltext"		"%result%"
						"TextAlignment"		"north"
						"font"	"QuestMap_Huge"
						"centerwrap"	"1"
					}
				}

				"GlobalStatus"
				{
					"ControlName"	"EditablePanel"
					"fieldName"		"GlobalStatus"
					"xpos"			"0"
					"ypos"			"rs1"
					"zpos"			"3"
					"wide"			"f0"
					"tall"			"30"
					"proportionaltoparent" "1"
					"mouseinputenabled"		"1"

					"border"		"QuickplayBorder"
					"bgcolor_override"	"0 0 0 240"

					"BloodMoneyTooltip"
					{
						"ControlName"	"Panel"
						"fieldName"		"BloodMoneyTooltip"
						"xpos"			"20"
						"ypos"			"0"
						"zpos"			"100"
						"wide"			"60"
						"tall"			"50"
						"paintbackground"	"0"
						"paintborder"		"0"
					}

					"RewardCreditImage"
					{
						"ControlName"	"ImagePanel"
						"fieldName"		"RewardCreditImage"
						"xpos"			"20"
						"ypos"			"-1"
						"zpos"			"0"
						"wide"			"o1"
						"tall"			"26"
						"visible"		"1"
						"PaintBackgroundType"	"0"
						"proportionaltoparent"	"1"
						"mouseinputenabled"		"0"
						"keyboardinputenabled"	"0"

						"image"			"cyoa/cyoa_cash_large"
						"scaleimage"	"1"
					}

					"RewardCreditsLabel"
					{
						"ControlName"	"Label"
						"fieldName"		"RewardCreditsLabel"
						"labeltext"		"%reward_credits%"
						"xpos"			"47"
						"ypos"			"7"
						"wide"			"140"
						"tall"			"10"
						"zpos"			"1"
						"font"			"QuestMap_Small"
						"TextAlignment"		"north-west"
						"proportionaltoparent" "1"
						"mouseinputenabled"		"0"
					}

					"StarsAvailableTooltip"
					{
						"ControlName"	"Panel"
						"fieldName"		"StarsAvailableTooltip"
						"xpos"			"94"
						"ypos"			"0"
						"zpos"			"100"
						"wide"			"40"
						"tall"			"50"
						"paintbackground"	"0"
						"paintborder"		"0"
					}

					"AvailableStarsImage"
					{
						"ControlName"	"ImagePanel"
						"fieldName"		"AvailableStarsImage"
						"xpos"			"94"
						"ypos"			"2"
						"zpos"			"0"
						"wide"			"o1"
						"tall"			"20"
						"visible"		"1"
						"PaintBackgroundType"	"0"
						"proportionaltoparent"	"1"
						"mouseinputenabled"		"0"
						"keyboardinputenabled"	"0"

						"image"			"cyoa/star_on"
						"scaleimage"	"1"
					}

					"AvailableStarsLabel"
					{
						"ControlName"	"Label"
						"fieldName"		"AvailableStarsLabel"
						"labeltext"		"%stars_available%"
						"xpos"			"115"
						"ypos"			"7"
						"wide"			"140"
						"tall"			"10"
						"zpos"			"1"
						"font"			"QuestMap_Small"
						"TextAlignment"		"north-west"
						"proportionaltoparent" "1"
						"mouseinputenabled"		"0"
					}

					"TotalStarsTooltip"
					{
						"ControlName"	"Panel"
						"fieldName"		"TotalStarsTooltip"
						"xpos"			"r70"
						"ypos"			"0"
						"zpos"			"100"
						"wide"			"40"
						"tall"			"50"
						"proportionaltoparent"	"1"
						"paintbackground"	"0"
						"paintborder"		"0"
					}

					"TotalStarsImage"
					{
						"ControlName"	"ImagePanel"
						"fieldName"		"TotalStarsImage"
						"xpos"			"r70"
						"ypos"			"2"
						"zpos"			"0"
						"wide"			"o1"
						"tall"			"20"
						"visible"		"1"
						"PaintBackgroundType"	"0"
						"proportionaltoparent"	"1"
						"mouseinputenabled"		"0"
						"keyboardinputenabled"	"0"

						"image"			"cyoa/star_off"
						"scaleimage"	"1"
					}

					"TotalStarsLabel"
					{
						"ControlName"	"Label"
						"fieldName"		"TotalStarsLabel"
						"labeltext"		"%stars_total%"
						"xpos"			"r50"
						"ypos"			"7"
						"wide"			"140"
						"tall"			"10"
						"zpos"			"1"
						"font"			"QuestMap_Small"
						"TextAlignment"		"north-west"
						"proportionaltoparent" "1"
						"mouseinputenabled"		"0"
					}
				}

				"SelectedNodeInfoPanel"
				{
					"fieldName"		"SelectedNodeInfoPanel"
					"xpos"			"0"
					"ypos"			"0"
					"zpos"			"10"
					"wide"			"200"
					"tall"			"220"
					"visible"		"0"
					"enabled"		"1"
					"proportionaltoparent" "1"
					"mouseinputenabled"		"1"
					"keyboardinputenabled"	"0"
					"border"	"CYOANodeViewBorder"
					"collapsed_height"	"165"
					"expanded_height"	"220"
				}

				"QuestObjective"
				{
					"fieldName"	"QuestObjective"
					"wide"		"200"
					"zpos"		"11"
					"visible"	"0"
					"mouseinputenabled"	"0"

					"border"	"CYOAPopupBorder"
					"bgcolor_override"	"37 37 37 255"

					"ObjectivesLabel"
					{
						"fieldName"	"ObjectivesLabel"
						"ControlName"	"Label"
						"xpos"		"2"
						"ypos"		"2"
						"zpos"		"100"
						"wide"		"f2"
						"tall"		"20"
						"labelText"	"#TF_QuestView_Objectives"
						"visible"	"1"
						"font"		"QuestMap_Medium"
						"fgcolor_override"	"75 75 75 255"
						"bgcolor_override"	"0 0 0 255"
						"textAlignment"	"west"
						"textinsetx"	"5"
					}

					"ItemTrackerPanel"
					{
						"fieldName"		"ItemTrackerPanel"
						"xpos"			"0"	
						"ypos"			"25"
						"wide"			"f0"	
						"tall"			"200"
						"progress_bar_standard_loc_token"	"#QuestPoints_Standard"
						"progress_bar_advanced_loc_token"	"#QuestPoints_Bonus"
						"item_attribute_res_file" "resource/UI/quests/CYOA/QuestObjectivePanel_CYOA.res"
						"mouseinputenabled"	"0"
						"map_view"	"1"
						"show_item_name"	"0"
						"bar_gap"		"5"
						"group_bars_with_objectives"	"1"
						"proportionaltoparent"	"1"

						"ModelImageKV"
						{
							"fieldName"	"ModelImage"
							"wide"		"20"
							"tall"		"20"
							"scaleimage"	"1"
							"zpos"		"10"
						}

						"progressbarKV"
						{
							"xpos"			"0"
							"ypos"			"8"
							"wide"			"f15"
							"tall"			"6"
							"zpos"			"4"
							"visible"		"1"
							"enabled"		"1"
							"proportionaltoparent" "1"

							"bgcolor_override"		"250 234 201 51"

							"PointsLabel"
							{
								"ControlName"	"Label"
								"fieldName"		"PointsLabel"
								"labeltext"		"%points%"
								"xpos"			"0"
								"ypos"			"0"
								"wide"			"f0"
								"tall"			"f0"
								"zpos"			"1"
								"font"			"QuestMap_Small"
								"textinsety"	"-1"
								"TextAlignment"		"center"
								"proportionaltoparent" "1"
							}

							"ProgressBarStandardHighlight" // current completed
							{
								"ControlName"	"EditablePanel"
								"fieldName"		"ProgressBarStandardHighlight"
								"xpos"			"0"
								"ypos"			"0"
								"wide"			"f0"
								"tall"			"f0"
								"bgcolor_override"		"QuestUncommitted"
								"zpos"			"2"
								"visible"		"1"
								"enabled"		"1"
								"proportionaltoparent" "1"

								"PointsLabelInvert"
								{
									"ControlName"	"Label"
									"fieldName"		"PointsLabelInvert"
									"labeltext"		"%points%"
									"xpos"			"0"
									"ypos"			"0"
									"wide"			"f0"
									"tall"			"f0"
									"zpos"			"8"
									"font"			"QuestMap_Small"
									"textinsety"	"-1"
									"TextAlignment"		"center"
									"proportionaltoparent" "1"
									"fgcolor_override"	"Black"
								}
							}

							"ProgressBarStandard" // current completed
							{
								"ControlName"	"EditablePanel"
								"fieldName"		"ProgressBarStandard"
								"xpos"			"0"
								"ypos"			"0"
								"wide"			"f0"
								"tall"			"f0"
								"zpos"			"3"
								"visible"		"1"
								"enabled"		"1"
								"proportionaltoparent" "1"

								"bgcolor_override"		"251 235 202 255"

								"PointsLabelInvert"
								{
									"ControlName"	"Label"
									"fieldName"		"PointsLabelInvert"
									"labeltext"		"%points%"
									"xpos"			"0"
									"ypos"			"0"
									"wide"			"f0"
									"tall"			"f0"
									"zpos"			"8"
									"font"			"QuestMap_Small"
									"textinsety"	"-1"
									"TextAlignment"		"center"
									"proportionaltoparent" "1"
									"fgcolor_override"	"Black"
								}
							}
						}
					}
				}

				"RewardsShop"
				{
					"ControlName"	"EditablePanel"
					"fieldName"		"RewardsShop"
					"xpos"			"cs-0.5"
					"ypos"			"cs-0.5"
					"zpos"			"2"
					"wide"			"p1"
					"tall"			"p1"
					"visible"		"0"
					"proportionaltoparent" "1"

					"BlackBG"
					{
						"ControlName"	"Panel"
						"fieldName"		"BlackBG"
						"xpos"			"0"
						"ypos"			"0"
						"zpos"			"-2"
						"wide"			"f0"
						"tall"			"f0"
						"visible"		"1"
						"PaintBackgroundType"	"0"
						"proportionaltoparent"	"1"
						"mouseinputenabled"		"0"
						"keyboardinputenabled"	"0"

						"bgcolor_override"	"0 0 0 255"
					}

					"Dimmer"
					{
						"ControlName"	"Panel"
						"fieldName"		"Dimmer"
						"xpos"			"0"
						"ypos"			"0"
						"zpos"			"-1"
						"wide"			"f0"
						"tall"			"f0"
						"visible"		"1"
						"PaintBackgroundType"	"0"
						"proportionaltoparent"	"1"
						"mouseinputenabled"		"0"
						"keyboardinputenabled"	"0"
						"alpha"			"100"
					
						"bgcolor_override" "0 0 0 255"
					}

					"TitleBorder"
					{
						"ControlName"	"Panel"
						"fieldName"		"TitleBorder"
						"xpos"			"0"
						"ypos"			"0"
						"zpos"			"-2"
						"wide"			"f0"
						"tall"			"50"
						"visible"		"1"
						"PaintBackgroundType"	"0"
						"proportionaltoparent"	"1"
						"mouseinputenabled"		"0"
						"keyboardinputenabled"	"0"

						"border"		"ReplayDefaultBorder"
					}

					"Title"
					{
						"ControlName"	"Label"
						"fieldName"		"Title"
						"xpos"			"cs-0.5"
						"ypos"			"20"
						"zpos"			"10"
						"wide"			"300"
						"tall"			"14"
						"autoResize"	"0"
						"pinCorner"		"0"
						"visible"		"1"
						"enabled"		"1"
						"tabPosition"	"0"
						"labeltext"		"#TFSOLO_SoloMenu_TeamSelect_Title"
						"font"			"QuestLargeText"
						"textAlignment"	"center"
						"dulltext"		"0"
						"brighttext"	"0"
						"default"		"0"
						"proportionaltoparent" "1"
						"paintbackground"	"0"
					} // Title

					"Description"
					{
						"ControlName"	"Label"
						"fieldName"		"Description"
						"xpos"			"cs-0.5"
						"ypos"			"34"
						"zpos"			"10"
						"wide"			"f0"
						"tall"			"14"
						"autoResize"	"0"
						"pinCorner"		"0"
						"visible"		"1"
						"enabled"		"1"
						"tabPosition"	"0"
						"labeltext"		"#TFSOLO_SoloMenu_TeamSelect_Desc"
						"font"			"QuestMap_Small"
						"textAlignment"	"center"
						"dulltext"		"0"
						"brighttext"	"0"
						"default"		"0"
						"proportionaltoparent" "1"
						"paintbackground"	"0"
					} // Title
					
					"TeamRedButton"
					{
						"ControlName"	"CExImageButton"
						"fieldName"		"TeamRedButton"
						"xpos"			"cs-0.5-150"
						"ypos"			"cs-0.5"
						"zpos"			"10"
						"wide"			"110"
						"tall"			"25"
						"autoResize"	"0"
						"pinCorner"		"3"
						"visible"		"1"
						"enabled"		"1"
						"tabPosition"	"0"
						"labelText"		"RED"
						"font"			"HudFontSmallBold"
						"textAlignment"	"center"
						"textinsetx"	"5"
						"use_proportional_insets" "1"
						"dulltext"		"0"
						"brighttext"	"0"
						"default"		"1"
						"Command"		"selectteamred"
						"proportionaltoparent" "1"
						"actionsignallevel" "4"

						"sound_depressed"	"UI/buttonclick.wav"
						"sound_released"	"UI/buttonclickrelease.wav"
					}
					
					"TeamBlueButton"
					{
						"ControlName"	"CExImageButton"
						"fieldName"		"TeamBlueButton"
						"xpos"			"cs-0.5+150"
						"ypos"			"cs-0.5"
						"zpos"			"10"
						"wide"			"110"
						"tall"			"25"
						"autoResize"	"0"
						"pinCorner"		"3"
						"visible"		"1"
						"enabled"		"1"
						"tabPosition"	"0"
						"labelText"		"BLU"
						"font"			"HudFontSmallBold"
						"textAlignment"	"center"
						"textinsetx"	"5"
						"use_proportional_insets" "1"
						"dulltext"		"0"
						"brighttext"	"0"
						"default"		"1"
						"Command"		"selectteamblue"
						"proportionaltoparent" "1"
						"actionsignallevel" "4"

						"sound_depressed"	"UI/buttonclick.wav"
						"sound_released"	"UI/buttonclickrelease.wav"
					}

					"ItemsScroller"
					{
						"ControlName"	"CExScrollingEditablePanel"
						"fieldName"		"ItemsScroller"
						"xpos"			"cs-0.5"
						"ypos"			"50"
						"wide"			"p1"
						"tall"			"f50"
						"visible"		"1"
						"proportionaltoparent" "1"
						"mouseinputenabled"	"1"
						"bottom_buffer"	"50"
						"scroll_step"	"20"

						"ScrollBar"
						{
							"ControlName"	"ScrollBar"
							"FieldName"		"ScrollBar"
							"xpos"			"rs1-5"
							"ypos"			"0"
							"tall"			"f0"
							"wide"			"5" // This gets slammed from client schme.  GG.
							"zpos"			"1000"
							"nobuttons"		"1"
							"proportionaltoparent"	"1"

							"Slider"
							{
								"fgcolor_override"	"TanDark"
							}
		
							"UpButton"
							{
								"ControlName"	"Button"
								"FieldName"		"UpButton"
								"visible"		"0"
							}
		
							"DownButton"
							{
								"ControlName"	"Button"
								"FieldName"		"DownButton"
								"visible"		"0"
							}
						}

						
					} // ItemsScroller
				} // RewardsShop

				"DisconnetedContainer"
				{
					"ControlName"	"EditablePanel"
					"fieldName"		"DisconnetedContainer"
					"xpos"			"0"
					"ypos"			"0"
					"wide"			"f0"
					"tall"			"f0"
					"zpos"			"50"
					"alpha"			"255"
					"proportionaltoparent" "1"
					"mouseinputenabled"	"0"

					"IntroDesc"
					{
						"ControlName"	"Label"
						"fieldName"		"IntroDesc"
						"labeltext"		"#TF_QuestMap_NoGC"
						"xpos"			"cs-0.5"
						"ypos"			"cs-0.5"
						"wide"			"p0.75"
						"tall"			"100"
						"zpos"			"1"
						"font"			"QuestMap_Large"
						"TextAlignment"		"center"
						"proportionaltoparent" "1"
						"mouseinputenabled"		"0"
						"fgcolor_override"	"TanLight"
						"wrap"	"0"
					}

					"StaticBackground"
					{
						"ControlName"	"ImagePanel"
						"fieldName"		"StaticBackground"
						"xpos"			"0"
						"ypos"			"0"
						"zpos"			"0"
						"wide"			"f0"
						"tall"			"f0"
						"visible"		"1"
						"PaintBackgroundType"	"0"
						"proportionaltoparent"	"1"
						"mouseinputenabled"		"0"
						"keyboardinputenabled"	"0"

						"alpha"			"255"
						"image"			"..\models\passtime\tv\passtime_tv_screen_static"
						"tileImage"	"1"
					}
				}

				"StaticBar1"
				{
					"ControlName"	"ImagePanel"
					"fieldName"		"StaticBar1"
					"xpos"			"0"
					"ypos"			"50"
					"zpos"			"9000"
					"wide"			"f0"
					"tall"			"50"
					"visible"		"1"
					"PaintBackgroundType"	"0"
					"proportionaltoparent"	"1"
					"mouseinputenabled"		"0"
					"keyboardinputenabled"	"0"

					"alpha"		"50"
					"image"			"..\overlays\black_gradient"
					"scaleimage"	"1"
					"rotation"	"3"
				}

				
				"StaticBar2"
				{
					"ControlName"	"EditablePanel"
					"fieldName"		"StaticBar2"
					"xpos"			"0"
					"ypos"			"120"
					"zpos"			"9000"
					"wide"			"f0"
					"tall"			"50"
					"visible"		"1"
					"PaintBackgroundType"	"0"
					"proportionaltoparent"	"1"
					"mouseinputenabled"		"0"
					"keyboardinputenabled"	"0"

					"bgcolor_override"	"255 255 255 3"
				}

				"BlackOverlay"
				{
					"ControlName"	"Panel"
					"fieldName"		"BlackOverlay"
					"xpos"			"0"
					"ypos"			"0"
					"zpos"			"8999"
					"wide"			"f0"
					"tall"			"f0"
					"visible"		"1"
					"PaintBackgroundType"	"0"
					"proportionaltoparent"	"1"
					"mouseinputenabled"		"0"
					"keyboardinputenabled"	"0"
					
					"bgcolor_override" "0 0 0 255"
				}

				"StaticOverlay"
				{
					"ControlName"	"ImagePanel"
					"fieldName"		"StaticOverlay"
					"xpos"			"0"
					"ypos"			"0"
					"zpos"			"9000"
					"wide"			"f0"
					"tall"			"f0"
					"visible"		"1"
					"PaintBackgroundType"	"0"
					"proportionaltoparent"	"1"
					"mouseinputenabled"		"0"
					"keyboardinputenabled"	"0"

					"alpha"			"20"
					"image"			"..\models\passtime\tv\passtime_tv_screen_static"
					"tileImage"	"1"
				}

				

			} // MapAreaPanel

		} // MainContainer
	}
}
