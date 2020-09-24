//
// Created by Vlad on 24.09.2020.
//

#include "Button.h"

Button::Button(const std::string &idleBackground, const std::string &pushedBackground) {
    normalSprite = cocos2d::Sprite::create(idleBackground);
    normalSprite->setScale(5.0f);
    pressedSprite = cocos2d::Sprite::create(pushedBackground);
    pressedSprite->setScale(5.0f);
    pressedSprite->setVisible(false);

    touchEvent = cocos2d::EventListenerTouchOneByOne::create();
    touchEvent->onTouchBegan = [this](cocos2d::Touch *touch, cocos2d::Event *event) -> bool {
        return true;
    };
    touchEvent->onTouchMoved = [this](cocos2d::Touch *touch, cocos2d::Event *event) {

    };
    touchEvent->onTouchEnded = [this](cocos2d::Touch *touch, cocos2d::Event *event) {

    };
    touchEvent->onTouchCancelled = [this](cocos2d::Touch *touch, cocos2d::Event *event) {

    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchEvent, normalSprite);

    this->currentState = ButtonState::IDLE;

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

void Button::setPosition(const cocos2d::Vec2 &position) {
    normalSprite->setPosition(position);
    pressedSprite->setPosition(position);
}

void Button::update(float t) {

}

void Button::initTouchListener() {
    touchEvent->onTouchBegan = [this](cocos2d::Touch *touch, cocos2d::Event *event) -> bool {
        auto bounds = event->getCurrentTarget()->getBoundingBox();
        if (bounds.containsPoint(touch->getLocation())) {
            setPushedState();

            cocos2d::log("PUSHED");
        }

        return true;
    };
    touchEvent->onTouchMoved = [this](cocos2d::Touch *touch, cocos2d::Event *event) {
        if (currentState != ButtonState::IDLE && touch->getLocation().distance(normalSprite->getPosition()) < SAFE_ZONE_RADIUS) {
            if (touch->getPreviousLocation().distance(touch->getLocation()) > EPSILON) {
                auto bounds = event->getCurrentTarget()->getBoundingBox();
                if (bounds.containsPoint(touch->getLocation())) {
                    setPushedState();

                    cocos2d::log("DRAGOUT TOUCH BUTTON -> PUSHED STATE");
                } else {
                    setDragoutState();

                    cocos2d::log("DRAGOUT");
                }
            }
        } else {
            setIdleState();

            cocos2d::log("LEAVE SAFE ZONE -> IDLE STATE");
        }
    };

    touchEvent->onTouchEnded = [this](cocos2d::Touch *touch, cocos2d::Event *event) {
        setIdleState();
    };

    touchEvent->onTouchCancelled = [this](cocos2d::Touch *touch, cocos2d::Event *event) {
        setIdleState();
    };
}

float Button::getPositionX() const {
    return normalSprite->getPositionX();
}

float Button::getPositionY() const {
    return normalSprite->getPositionY();
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