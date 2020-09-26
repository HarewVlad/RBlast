//
// Created by Vlad on 24.09.2020.
//

#pragma once

#include "Button.h"

class ButtonLongPressSupportState : public ButtonState {
public:
    static const int LONG_PUSH = 4;
};

class ButtonLongPressSupport : public Button {
public:
    ButtonLongPressSupport(const std::string &idleBackground, const std::string &pushedBackground, const std::function<void (Ref *)> &func);
    std::string getCurrentStateAsString() const override;
private:
    void setPushedState() override;
    void setIdleState() override;
    void setDragoutState() override;
    void setLongPushState();
    void update(float t) override;
private:
    float timePushed;
    float longPushThreshold;
};
