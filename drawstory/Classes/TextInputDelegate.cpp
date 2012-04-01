//
//  TextInputDelegate.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-3-29.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "TextInputDelegate.h"

using namespace cocos2d;


static const float _touchDelta = 5.0f;

TextInputDelegate::TextInputDelegate()
: inputtingNode_(NULL)
{
    
}

TextInputDelegate::~TextInputDelegate() 
{ }


CCTextFieldTTF* TextInputDelegate::addTextField(const cocos2d::CCPoint &pos, 
                                                int tag, 
                                                const char *placehold, 
                                                const char *fontName, 
                                                float fontSize)
{
    CCTextFieldTTF* field = CCTextFieldTTF::textFieldWithPlaceHolder(placehold, 
                                                                     fontName, 
                                                                     fontSize);
    CC_ASSERT(field != NULL);
    field->setPosition(pos);
    field->setDelegate(this);
    addChild(field, 0, tag);
    
    // add to text field arrays
    textFields_.push_back(field);
    
    return field;
}

const char* TextInputDelegate::getTextFieldString(int tag)
{
    CCTextFieldTTF* field = static_cast<CCTextFieldTTF*>(getChildByTag(tag));
    
    const char* str = NULL;
    if(field)
        str = field->getString();
    return str;
}

void TextInputDelegate::setTextFieldString(int tag, const char *str)
{
    CCTextFieldTTF* field = static_cast<CCTextFieldTTF*>(getChildByTag(tag));
    if(field)
        field->setString(str);
}


void TextInputDelegate::keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo &info)
{
    
}

void TextInputDelegate::onEnter()
{
    CCLayer::onEnter();
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, -1, true);
}

void TextInputDelegate::onExit()
{
    CCLayer::onExit();
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
}

bool TextInputDelegate::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    touchBeganPosition_ = convertTouchToNodeSpace(touch);
    if(hitTestWithTextFields(touchBeganPosition_) == NULL)
    {
        onTouchBegan(touch, event);
    }
    return true;
}

void TextInputDelegate::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    onTouchMoved(touch, event);
}

void TextInputDelegate::ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    onTouchCancelled(touch,event);
}

void TextInputDelegate::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    CCPoint endPos = convertTouchToNodeSpace(touch);
    
    if (::abs(endPos.x - touchBeganPosition_.x) < _touchDelta
        && ::abs(endPos.y - touchBeganPosition_.y) < _touchDelta)
    {
        CCNode* nodeClicked = hitTestWithTextFields(endPos);
        if(nodeClicked != inputtingNode_)
        {
            if(inputtingNode_)
                onTextFieldClicked(inputtingNode_, false);
            inputtingNode_ = static_cast<CCTextFieldTTF*>(nodeClicked);   
            if(inputtingNode_)
                onTextFieldClicked(inputtingNode_, true);
        }
    }
    
    onTouchEnded(touch, event);
}


bool TextInputDelegate::onTextFieldInsertText(cocos2d::CCTextFieldTTF *sender, const char *text, int nLen)
{
    // if insert enter, treat as default to detach with ime
    if ('\n' == *text)
    {
        return false;
    }
    

    return (sender->getCharCount() >= getMaximumCharacters(sender));
}



void TextInputDelegate::onTextFieldClicked(CCTextFieldTTF* field, bool clicked)
{
    if(clicked)
        field->attachWithIME();
    else 
        field->detachWithIME();
}


CCNode* TextInputDelegate::hitTestWithTextFields(const cocos2d::CCPoint &localPos)
{
    CCNode* node = NULL;
    for(std::vector<CCTextFieldTTF*>::size_type fieldIdx = 0; fieldIdx < textFields_.size(); ++fieldIdx)
    {
        CCTextFieldTTF* field = textFields_.at(fieldIdx);
        if(CCRect::CCRectContainsPoint(getTextFieldRect(field), localPos))
        {
            node = field;
            break;
        }
    }
    return node;
}


