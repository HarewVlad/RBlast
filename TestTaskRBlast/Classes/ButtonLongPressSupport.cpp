//
// Created by Vlad on 24.09.2020.
//

#include "ButtonLongPressSupport.h"

ButtonLongPressSupport::ButtonLongPressSupport(const std::string &idleBackground, const std::string &pushedBackground, const std::function<void (Ref *)> &func) : Button(idleBackground, pushedBackground, func) {
    this->timePushed = 0.0f;
    this->longPushThreshold = 2.0f;
}

void ButtonLongPressSupport::update(float t) {
    if (currentState == ButtonLongPressSupportState::PUSHED) {
        if (timePushed >= longPushThreshold) {
            setLongPushState();

            cocos2d::log("LONGPUSH");
            // Reset timer
            timePushed = 0.0f;
        } else {
            timePushed += t;
        }
    }
}

void ButtonLongPressSupport::setIdleState() {
    pressedSprite->stopAllActions();

    // Copy cool effect to normal sprite
    normalSprite->setRotation(pressedSprite->getRotation());

    normalSprite->setVisible(true);
    pressedSprite->setVisible(false);

    currentState = ButtonLongPressSupportState::IDLE;
}

void ButtonLongPressSupport::setDragoutState() {
    pressedSprite->stopAllActions();

    // Copy cool effect to normal sprite
    normalSprite->setRotation(pressedSprite->getRotation());

    normalSprite->setVisible(true);
    pressedSprite->setVisible(false);

    currentState = ButtonLongPressSupportState::DRAGOUT;
}

void ButtonLongPressSupport::setLongPushState() {
    if (currentState != ButtonLongPressSupportState::LONG_PUSH) {
        // Some cool effects
        auto rotate = cocos2d::RotateBy::create(1, 25);
        pressedSprite->runAction(cocos2d::RepeatForever::create(rotate));

        currentState = ButtonLongPressSupportState::LONG_PUSH;
    }
}

void ButtonLongPressSupport::setPushedState() {
    if (currentState != ButtonLongPressSupportState::PUSHED &&
        currentState != ButtonLongPressSupportState::LONG_PUSH) {
        normalSprite->setVisible(false);
        pressedSprite->setVisible(true);

        currentState = ButtonLongPressSupportState::PUSHED;
    }
}

std::string ButtonLongPressSupport::getCurrentStateAsString() const {
    switch (currentState) {
        case ButtonLongPressSupportState::IDLE:
            return "IDLE";
        case ButtonLongPressSupportState::PUSHED:
            return "PUSHED";
        case ButtonLongPressSupportState::DRAGOUT:
            return "DRAGOUT";
        case ButtonLongPressSupportState::LONG_PUSH:
            return "LONG_PUSH";
        default:
            return "";
    }
}