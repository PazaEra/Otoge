﻿#include "MusicSelectScene.hpp"
#include "../../../../Util/Calculate/Animation/Easing.hpp"
#include "../../../../System/GlobalMethod.hpp"
#include "../../../../Util/Visual/Color.hpp"
#include "../../../../System/Task/TaskManager.hpp"
#include "../../Title/TitleScene.hpp"
#include "../../../../System/Input/KeyboardManager.hpp"
#include "../../../../System/Config.h"
#include "../../../../Util/Setting/SettingManager.h"
#include "../../../../Util/Calculate/Screen/FontStringCalculator.hpp"
#include "../../../../System/Input/MouseManager.hpp"
#include "../../../../System/Font/FontHandleCreator.hpp"

MusicSelectScene::MusicSelectScene() : Scene("MusicSelectScene")
{
    HeaderPanel_ = std::make_shared<Scene>("Header", ScreenData(0.f, 0.f, 100.f, 10.f), DefaultScaler_);
    HeaderPanel_->SetDrawFunction([=]
        {
            ScreenData fixed = HeaderPanel_->GetDefaultScaler()->Calculate(0.f, 0.f, 100.f, 100.f);
            DrawBox(engine::CastToInt(fixed.posX), engine::CastToInt(fixed.posY), engine::CastToInt(fixed.posX + fixed.width), engine::CastToInt(fixed.posY + fixed.height), color_preset::WHITE, TRUE);
        });
    HeaderPanel_->SetPriority(50.f);
    AddChildTask(std::static_pointer_cast<Task>(HeaderPanel_));

    BackButton_ = std::make_shared<Button>("< Back", ScreenData(0.f, 0.f, 10.f, 100.f), HeaderPanel_->GetDefaultScaler());
    BackButton_->GetTextLabelInstance()->adjustmentFontSize = false;
    BackButton_->GetTextLabelInstance()->ChangeFontSize(static_cast<int>(DefaultScaler_->CalculateHeight(2.5f)));
    BackButton_->GetTextLabelInstance()->ChangeFontThickness(2);
    BackButton_->SetTransparent(100.f);
    HeaderPanel_->AddChildTask(std::static_pointer_cast<Task>(BackButton_));

    ListPanel_ = std::make_shared<ScrollablePanel>("MusicList", ScreenData(50.f, HeaderPanel_->GetScreenHeight(), 50.f, 100.f - HeaderPanel_->GetScreenHeight()), ScreenData(0.f, 0.f, 100.f, 100.f), DefaultScaler_);
    ListPanel_->SetPriority(-10.f);
    AddChildTask(std::static_pointer_cast<Task>(ListPanel_));


    MusicPanels_.push_back(std::make_shared<MusicInfoPanel>("TEST", "hoge", 10.2f, ListPanel_->GetPanelInstance()->GetDefaultScaler()));
    MusicPanels_.push_back(std::make_shared<MusicInfoPanel>("TEST2", "hoge", 10.2f, ListPanel_->GetPanelInstance()->GetDefaultScaler()));
    MusicPanels_.push_back(std::make_shared<MusicInfoPanel>("TEST3", "hoge", 10.2f, ListPanel_->GetPanelInstance()->GetDefaultScaler()));
    MusicPanels_.push_back(std::make_shared<MusicInfoPanel>("TEST4", "hoge", 10.2f, ListPanel_->GetPanelInstance()->GetDefaultScaler()));
    MusicPanels_.push_back(std::make_shared<MusicInfoPanel>("TEST5", "hoge", 10.2f, ListPanel_->GetPanelInstance()->GetDefaultScaler()));
    MusicPanels_.push_back(std::make_shared<MusicInfoPanel>("TEST6", "hoge", 10.2f, ListPanel_->GetPanelInstance()->GetDefaultScaler()));
    MusicPanels_.push_back(std::make_shared<MusicInfoPanel>("TEST7", "hoge", 10.2f, ListPanel_->GetPanelInstance()->GetDefaultScaler()));
    MusicPanels_.push_back(std::make_shared<MusicInfoPanel>("TEST8", "hoge", 10.2f, ListPanel_->GetPanelInstance()->GetDefaultScaler()));
    MusicPanels_.push_back(std::make_shared<MusicInfoPanel>("TEST9", "hoge", 10.2f, ListPanel_->GetPanelInstance()->GetDefaultScaler()));
    MusicPanels_.push_back(std::make_shared<MusicInfoPanel>("TEST10", "hoge", 10.2f, ListPanel_->GetPanelInstance()->GetDefaultScaler()));
    MusicPanels_.push_back(std::make_shared<MusicInfoPanel>("TEST11", "hoge", 10.2f, ListPanel_->GetPanelInstance()->GetDefaultScaler()));


    ListPanel_->GetPanelInstance()->SetScreenHeight((MusicPanels_[0]->GetScreenHeight() + 1.0f) * (MusicPanels_.size() + 1));
    ListPanel_->GetPanelInstance()->ReCalculateScreen();
    ListPanel_->GetPanelInstance()->RefreshDrawBuffer();
    int PanelCount = 0;
    for(auto panel : MusicPanels_)
    {
        panel->SetPositionY((panel->GetScreenHeight() + 1.0f) * PanelCount);
        ListPanel_->GetPanelInstance()->AddChildTask(panel);
        PanelCount++;
    }

    StartFadeIn();
}

MusicSelectScene::~MusicSelectScene()
{
}

void MusicSelectScene::OnStartedFadeIn()
{
    HeaderPanel_->SetPositionY(-HeaderPanel_->GetScreenHeight());
    HeaderPanel_->SetTransparent(0.f);
    SetPositionY(GetScreenHeight());
}

void MusicSelectScene::SceneFadeIn(float deltaTime)
{
    float totalTime = 0.5f;
    Easing::EaseFunction ease = Easing::OutExp;

    SetPositionY(engine::CastToFloat(ease(timerCount, totalTime, 0.f, GetScreenHeight())));
    SetTransparent(engine::CastToFloat(ease(timerCount, totalTime, 100.f, 0.f)));

    float secondStartTime = (totalTime - 0.3f);
    if (timerCount > secondStartTime)
    {
        float secondTotalTime = 0.4f;
        HeaderPanel_->SetPositionY(engine::CastToFloat(ease(timerCount - secondStartTime, secondTotalTime, 0.f, -HeaderPanel_->GetScreenHeight())));
        HeaderPanel_->SetTransparent(engine::CastToFloat(ease(timerCount - secondStartTime, secondTotalTime, 100.f, 0.f)));

        if(timerCount > totalTime + secondTotalTime)
        {
            StopFade();
        }
    }
}

void MusicSelectScene::OnStoppedFadeIn()
{
    HeaderPanel_->SetPositionY(0.f);
    HeaderPanel_->SetTransparent(100.f);
    SetPositionX(0.f);
    SetTransparent(100.f);
}

void MusicSelectScene::SceneFadeOut(float deltaTime)
{
    float totalTime = 0.5f;
    Easing::EaseFunction ease = Easing::OutExp;

    SetPositionY(engine::CastToFloat(ease(timerCount, totalTime, GetScreenHeight(), 0.f)));
    SetTransparent(engine::CastToFloat(ease(timerCount, totalTime, 0.f, 100.f)));

    if (timerCount > totalTime)
    {
        StopFade();
    }
}

void MusicSelectScene::OnStoppedFadeOut()
{
    SetTransparent(0.f);
    Terminate();
}

void MusicSelectScene::SceneUpdate(float deltaTime)
{
    if (BackButton_->IsClickedMouse())
    {
        TaskManager::GetInstance()->AddTaskByTypename<TitleScene>();
        StartFadeOut();
    }
}

void MusicSelectScene::Draw()
{
}


/*** 楽曲情報パネル ***/
int MusicInfoPanel::TitleFontHandle_ = -1;
int MusicInfoPanel::MiddleFontHandle_ = -1;
int MusicInfoPanel::SmallFontHandle_ = -1;
int MusicInfoPanel::GlobalPanelCount_ = 0;

MusicInfoPanel::MusicInfoPanel(const std::string& musicName, const std::string& artistName, float difficulty, std::shared_ptr<FlexibleScaler> parentScaler) :
                        Scene("MusicSelectScene{" + musicName + " , " + artistName + ": " + std::_Floating_to_string("%.2f", difficulty) + "}", 100.f, 14.0f, 4.f, 0.f, parentScaler)
{
    if (TitleFontHandle_ == -1 && MiddleFontHandle_ == -1 && SmallFontHandle_ == -1 && GlobalPanelCount_ == 0)
    {
        TitleFontHandle_ = FontHandleCreator::Create(engine::CastToInt(DefaultScaler_->CalculateHeight(24.f)), engine::CastToInt(DefaultScaler_->CalculateHeight(2.f)), FontHandleCreator::normal);
        MiddleFontHandle_ = FontHandleCreator::Create(engine::CastToInt(DefaultScaler_->CalculateHeight(18.f)), engine::CastToInt(DefaultScaler_->CalculateHeight(1.4f)), FontHandleCreator::normal);
        SmallFontHandle_ = FontHandleCreator::Create(engine::CastToInt(DefaultScaler_->CalculateHeight(10.f)), 1, FontHandleCreator::normal);

        //TitleFontHandle_ = CreateFontToHandle(SettingManager::GetGlobal()->Get<std::string>(game_config::SETTINGS_FONT_NAME).get().c_str(), engine::CastToInt(DefaultScaler_->CalculateHeight(28.f)), engine::CastToInt(DefaultScaler_->CalculateHeight(2.f)), 0);
        //MiddleFontHandle_ = CreateFontToHandle(SettingManager::GetGlobal()->Get<std::string>(game_config::SETTINGS_FONT_NAME).get().c_str(), engine::CastToInt(DefaultScaler_->CalculateHeight(18.f)), engine::CastToInt(DefaultScaler_->CalculateHeight(1.4f)), 0);
        //SmallFontHandle_ = CreateFontToHandle(SettingManager::GetGlobal()->Get<std::string>(game_config::SETTINGS_FONT_NAME).get().c_str(), engine::CastToInt(DefaultScaler_->CalculateHeight(10.f)), 1, 0);
    }

    GlobalPanelCount_++;

    MusicName_ = musicName;
    ArtistName_ = artistName;
    Difficulty_ = difficulty;

    PreLayoutPosX_ = ParentScaler_->CalculatePositionRateX(GetRawPositionX());

    DefaultPosX_ = GetPositionX();
}

MusicInfoPanel::~MusicInfoPanel()
{
    GlobalPanelCount_--;
    if (GlobalPanelCount_ == 0)
    {
        DeleteFontToHandle(TitleFontHandle_);
        DeleteFontToHandle(MiddleFontHandle_);
        DeleteFontToHandle(SmallFontHandle_);
        TitleFontHandle_ = -1;
        MiddleFontHandle_ = -1;
        SmallFontHandle_ = -1;
    }
}

void MusicInfoPanel::SceneUpdate(float deltaTime)
{
    Easing::EaseFunction hoverEase = Easing::OutExp;
    const float totalTime = 0.8f;

    if (IsBeginOnMouse())
    {
        timerCount = 0.f;
        isHoverAnimate = true;
    }
    else if(IsEndOnMouse())
    {
        timerCount = 0.f;
        isHoverAnimate = true;
    }

    if(isHoverAnimate)
    {
        if(IsOnMouse())
        {
            SetPositionX(engine::CastToFloat(hoverEase(timerCount, totalTime, 0.f, DefaultPosX_)));
        }
        else
        {
            SetPositionX(engine::CastToFloat(hoverEase(timerCount, totalTime, DefaultPosX_, 0.f)));
        }

        if(timerCount >= totalTime)
        {
            timerCount = 0.f;
            isHoverAnimate = false;
        }
    }

    if (IsDownMouse())
    {
        timerCount = 0.f;
        AddChildTask(std::static_pointer_cast<Task>(std::make_shared<ButtonPushedAnimate>(
            DefaultScaler_->
            CalculatePositionRateX(
                MouseManager::GetInstance()->GetMouseXf() - GetRawPositionX() - ParentScaler_->GetDiffX()),
            DefaultScaler_->CalculatePositionRateY(
                MouseManager::GetInstance()->GetMouseYf() - GetRawPositionY() - ParentScaler_->GetDiffY()),
            color_preset::DARK_GREY, 20.f, DefaultScaler_)));
    }
    /*
    if(timerCount <= 0.333f / 2.f)
    {
        SetPositionX(Easing::InExp(timerCount, (0.333f / 2.f), PreLayoutPosX_ - 1.f, PreLayoutPosX_));
        //AddPositionY(-5.f * deltaTime);
    }
    if (timerCount > 0.333f / 2.f)
    {
        SetPositionX(Easing::OutExp(timerCount - 0.333f / 2.f, (0.333f / 2.f), PreLayoutPosX_, PreLayoutPosX_ - 1.f));
    }
    if (timerCount >= 0.666f / 2.f)
    {
        timerCount = 0.f;
    }
    */
}

void MusicInfoPanel::Draw()
{
    ScreenData fixed = DefaultScaler_->Calculate(0.f, 0.f, 100.f, 100.f);
    DrawBoxAA(fixed.posX, fixed.posY, fixed.posX + fixed.width, fixed.posY + fixed.height, color_preset::DARK_GREY, TRUE);
    DrawBoxAA(fixed.posX, fixed.posY, fixed.posX + fixed.width, fixed.posY + fixed.height, color_preset::WHITE, FALSE);

    float TitleTextCenterH = FontStringCalculator::GetStringCenterHorizontal(TitleFontHandle_, MusicName_);
    float TitleTextCenterV = FontStringCalculator::GetStringCenterVertical(TitleFontHandle_);
    float ArtistTextCenterH = FontStringCalculator::GetStringCenterHorizontal(MiddleFontHandle_, MusicName_);
    float ArtistTextCenterV = FontStringCalculator::GetStringCenterVertical(MiddleFontHandle_);
    float DifficultyTextCenterH = FontStringCalculator::GetStringCenterHorizontal(SmallFontHandle_, MusicName_);
    float DifficultyTextCenterV = FontStringCalculator::GetStringCenterVertical(SmallFontHandle_);

    ScreenData TitlePos = DefaultScaler_->Calculate(18.f, 14.f, 0.f, 0.f);
    //TitlePos.posX -= TitleTextCenterH;
    TitlePos.posY -= TitleTextCenterV;
    ScreenData ArtistPos = DefaultScaler_->Calculate(DefaultScaler_->CalculatePositionRateX(TitlePos.posX) + 1.f, DefaultScaler_->CalculatePositionRateY(TitlePos.posY) + 30.f, 0.f, 0.f);
    //ArtistPos.posX -= ArtistTextCenterH;
    ArtistPos.posY -= ArtistTextCenterV;

    DrawStringFToHandle(TitlePos.posX, TitlePos.posY, MusicName_.c_str(), color_preset::WHITE, TitleFontHandle_);
    DrawStringFToHandle(ArtistPos.posX, ArtistPos.posY, ArtistName_.c_str(), color_preset::LIGHT_GREY, MiddleFontHandle_);
}
