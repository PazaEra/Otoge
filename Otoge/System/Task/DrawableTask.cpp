﻿#include "DrawableTask.hpp"
#include "../Config.h"
#include "../../Util/Setting/SettingManager.h"
#include "../Input/MouseManager.hpp"
#include "../../Util/Calculate/Screen/FontStringCalculator.hpp"
#include "../../Util/Visual/Color.hpp"
#include "TaskManager.hpp"
#include "../../Util/Window/DxSettings.hpp"
#include "../GlobalMethod.hpp"
int DrawableTask::TemporaryDrawBuffer_ = -1;
int DrawableTask::BufferWidth_ = -1;
int DrawableTask::BufferHeight_ = -1;
bool DrawableTask::IsDrawPoint_ = false;
float DrawableTask::DrawPointSize_ = 1.5f;

DrawableTask::DrawableTask(const std::string& sceneName, float x, float y, float z,
                           std::shared_ptr<FlexibleScaler> parentScaler) : Task(sceneName)
{
    if(TemporaryDrawBuffer_ == -1)
    {
        if(BufferWidth_ == -1) BufferWidth_ = DxSettings::windowWidth;
        if(BufferHeight_ == -1) BufferHeight_ = DxSettings::windowWidth;
        IsDrawPoint_ = SettingManager::GetGlobal()->Get<bool>(game_config::SETTINGS_DEBUG_DRAW_DTASK_POINT).get();
        SettingManager::GetGlobal()->SetDefault<float>("system.debug.drawable.drawPointSize", DrawPointSize_);
        SettingManager::GetGlobal()->Save();
        DrawPointSize_ = SettingManager::GetGlobal()->Get<float>("system.debug.drawable.drawPointSize").get();
        TemporaryDrawBuffer_ = MakeScreen(BufferWidth_, BufferHeight_, TRUE);
        Logger_->Info("一時描画バッファを生成しました。");
    }
    isAutoUpdateChildren = false;
    this->position.x = x;
    this->position.y = y;
    this->position.z = z;
    ParentScaler_ = parentScaler;
    if(ParentScaler_ == nullptr)
    {
        ParentScaler_ = FlexibleScaler::GetWindowBasedInstance();
    }
}

DrawableTask::~DrawableTask()
{
}

void DrawableTask::Update(float deltaTime)
{
    PreUpdate(deltaTime);
    if(IsVisible_ && engine::CastToInt(Transparency_) > 0)
    {
        int l_CurrentBuffer = GetDrawScreen();
        int l_CurrentBlendMode = DX_BLENDMODE_NOBLEND, l_CurrentBlendParam = 255;
        GetDrawBlendMode(&l_CurrentBlendMode, &l_CurrentBlendParam);
        SetDrawScreen(TemporaryDrawBuffer_);
        SetDrawBlendMode(l_CurrentBlendMode, l_CurrentBlendParam);
        ClearDrawScreen();

        /* 描画始め */
        Draw();
        TaskManager::UpdateTasks(children, childrenQueues, TickSpeed_, deltaTime);

        // デバッグ
        if(IsDrawPoint_)
        {
            float x = ParentScaler_->CalculatePositionX(position.x);
            float y = ParentScaler_->CalculatePositionY(position.y);
            float rx = ParentScaler_->CalculateWidth(DrawPointSize_);
            float ry = ParentScaler_->CalculateHeight(DrawPointSize_);
            DrawOvalAA(x, y, rx, ry, 50, color_preset::LIGHT_GREEN, TRUE);
        }

        /* 描画終わり */
        SetDrawScreen(l_CurrentBuffer);
        SetDrawBlendMode(l_CurrentBlendMode, l_CurrentBlendParam);
        if(engine::CastToInt(Transparency_) < 100)
            SetDrawBlendMode(DX_BLENDMODE_PMA_ALPHA, engine::CastToInt((Transparency_ / 100.f) * 255.f));
        else
            SetDrawBlendMode(DX_BLENDMODE_PMA_ALPHA, 255);

        DrawExtendGraph(0, 0, BufferWidth_, BufferHeight_, TemporaryDrawBuffer_, TRUE);
        SetDrawBlendMode(l_CurrentBlendMode, l_CurrentBlendParam);
    }
}

void DrawableTask::Enable3D()
{
    IsEnabled3D_ = true;
}

void DrawableTask::Disable3D()
{
    IsEnabled3D_ = false;
}

void DrawableTask::SetVisible(bool visible)
{
    IsVisible_ = visible;
}

void DrawableTask::SetTransparent(float transparent)
{
    Transparency_ = transparent;
}

float DrawableTask::GetTransparent()
{
    return Transparency_;
}

bool DrawableTask::IsVisible() const
{
    return IsVisible_;
}

bool DrawableTask::IsEnable3D() const
{
    return IsEnabled3D_;
}
