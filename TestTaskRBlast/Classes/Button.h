//
// Created by Vlad on 24.09.2020.
//

#ifndef PROJ_ANDROID_BUTTON_H
#define PROJ_ANDROID_BUTTON_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"

static const float SAFE_ZONE_RADIUS = 100.0f;
static const float EPSILON = 0.5f;

class ButtonState {
public:
    static const int IDLE = 1;
    static const int PUSHED = 2;
    static const int DRAGOUT = 3;
};

class Button : public cocos2d::Node {
public:
    Button(const std::string &idleBackground, const std::string &pushedBackground);
    void initTouchListener();
    inline int getCurrentState() const {
        return currentState;
    }
    virtual std::string getCurrentStateAsString() const;
    void setPosition(const cocos2d::Vec2 &position) override;
    float getPositionX() const override;
    float getPositionY() const override;
    virtual void update(float t) override;
protected:
    virtual void setIdleState();
    virtual void setDragoutState();
    virtual void setPushedState();
protected:
    cocos2d::EventListenerTouchOneByOne *touchEvent;
    cocos2d::Sprite *normalSprite;
    cocos2d::Sprite *pressedSprite;

    int currentState;
};


#endif //PROJ_ANDROID_BUTTON_H
