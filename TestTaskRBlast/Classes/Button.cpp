//
// Created by Vlad on 24.09.2020.
//

#include "Button.h"

Button::Button(const std::string &idleBackground, const std::string &pushedBackground, const std::function<void (Ref *)> &func) {
    normalSprite = cocos2d::Sprite::create(idleBackground);
    normalSprite->setScale(5.0f);
    pressedSprite = cocos2d::Sprite::create(pushedBackground);
    pressedSprite->setScale(5.0f);
    pressedSprite->setVisible(false);

    touchEvent = cocos2d::EventListenerTouchOneByOne::create();
    touchEvent->onTouchBegan = CC_CALLBACK_2(Button::onTouchBegan, this);
    touchEvent->onTouchEnded = CC_CALLBACK_2(Button::onTouchEnded, this);
    touchEvent->onTouchMoved = CC_CALLBACK_2(Button::onTouchMoved, this);
    touchEvent->onTouchCancelled = CC_CALLBACK_2(Button::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchEvent, this);

    this->currentState = ButtonState::IDLE;
    this->mainFunc = func;

    this->scheduleUpdate();

    this->addChild(normalSprite);
    this->addChild(pressedSprite);
}

void Button::setIdleState() {
    if (currentState != ButtonState::IDLE) {
        normalSprite->setVisible(true);
        pressedSprite->setVisible(false);

        currentState = ButtonState::IDLE;
    }
}

void Button::setDragoutState() {
    if (currentState != ButtonState::DRAGOUT) {
        normalSprite->setVisible(true);
        pressedSprite->setVisible(false);

        currentState = ButtonState::DRAGOUT;
    }
}

void Button::setPushedState() {
    if (currentState != ButtonState::PUSHED) {
        normalSprite->setVisible(false);
        pressedSprite->setVisible(true);

        currentState = ButtonState::PUSHED;
    }
}

std::string Button::getCurrentStateAsString() const {
    switch (currentState) {
        case ButtonState::IDLE:
            return "IDLE";
        case ButtonState::PUSHED:
            return "PUSHED";
        case ButtonState::DRAGOUT:
            return "DRAGOUT";
        default:
            return "";
    }
}

bool Button::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
    if (getButtonBoundingBox().containsPoint(touch->getLocation())) {
        setPushedState();

        cocos2d::log("PUSHED");

        mainFunc(this);
    }

    return true;
}

void Button::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {
    setIdleState();
}

void Button::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) {
    if (currentState != ButtonState::IDLE) {
        if (touch->getPreviousLocation().distance(touch->getLocation()) > epsilon) {
            if (getButtonBoundingBox().containsPoint(touch->getLocation())) {
                setPushedState();

                cocos2d::log("INSIDE BOUNDING BOX");
            } else if (getExpandZone().containsPoint(touch->getLocation())) {
                setPushedState();

                cocos2d::log("INSIDE EXPAND ZONE");
            } else if (getSafeZone().containsPoint(touch->getLocation())) {
                setDragoutState();

                cocos2d::log("INSIDE SAFE ZONE");
            } else {
                setIdleState();

                cocos2d::log("OUTSIZE SAFE ZONE");
            }
        }
    }
}

void Button::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event) {
    setIdleState();
}

cocos2d::Rect Button::getButtonBoundingBox() const {
    return cocos2d::Rect(
            this->getPositionX() + normalSprite->getPositionX() - normalSprite->getBoundingBox().size.width * 0.5f,
            this->getPositionY() + normalSprite->getPositionY() - normalSprite->getBoundingBox().size.height * 0.5f,
            normalSprite->getBoundingBox().size.width,
            normalSprite->getBoundingBox().size.height);
}

cocos2d::Rect Button::getExpandZone() const {
    cocos2d::Rect contentSize = getButtonBoundingBox();
    return cocos2d::Rect(contentSize.getMinX() - expandZoneOffset,
                         contentSize.getMinY() - expandZoneOffset,
                         contentSize.size.width + expandZoneOffset * 2.0f,
                         contentSize.size.height + expandZoneOffset * 2.0f);
}

cocos2d::Rect Button::getSafeZone() const {
    cocos2d::Rect contentSize = getButtonBoundingBox();
    return cocos2d::Rect(contentSize.getMinX() - safeZoneOffset,
            contentSize.getMinY() - safeZoneOffset,
            contentSize.size.width + safeZoneOffset * 2.0f,
            contentSize.size.height + safeZoneOffset * 2.0f);
}