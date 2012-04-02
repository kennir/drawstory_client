//
//  TextInputDelegate.h
//  drawstory
//
//  Created by 张 靖宇 on 12-3-29.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef drawstory_TextInputDelegate_h
#define drawstory_TextInputDelegate_h

#include "cocos2d.h"
#include <vector>

class TextInputDelegate : public cocos2d::CCLayer, public cocos2d::CCIMEDelegate, public cocos2d::CCTextFieldDelegate
{
protected:
    enum { kDefaultMaximumCharacter  = 1024 };
public:
    TextInputDelegate();
    virtual ~TextInputDelegate();
    
    
    // virtual function you must override it
    virtual void onTextFieldClicked(cocos2d::CCTextFieldTTF* field, bool clicked);
    
    cocos2d::CCTextFieldTTF* addTextField(const cocos2d::CCPoint& pos,
                                          int z,
                                          int tag,
                                          const char* placehold,
                                          const char* fontName,
                                          float fontSize);
    const char* getTextFieldString(int tag);
    void setTextFieldString(int tag,const char* str);
    
    virtual void keyboardWillShow(cocos2d::CCIMEKeyboardNotificationInfo &info);
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
    virtual void ccTouchCancelled(cocos2d::CCTouch* touch,cocos2d::CCEvent* event);
    virtual bool onTextFieldInsertText(cocos2d::CCTextFieldTTF * sender, const char * text, int nLen);
    
    // 
    virtual void onTouchBegan(cocos2d::CCTouch* touch,cocos2d::CCEvent* event) { }
    virtual void onTouchMoved(cocos2d::CCTouch* touch,cocos2d::CCEvent* event) { }
    virtual void onTouchCancelled(cocos2d::CCTouch* touch,cocos2d::CCEvent* event) { }
    virtual void onTouchEnded(cocos2d::CCTouch* touch,cocos2d::CCEvent* event) { }
    
    virtual cocos2d::CCRect getTextFieldRect(cocos2d::CCTextFieldTTF* textField) { return textField->boundingBox(); }
    virtual int getMaximumCharacters(cocos2d::CCTextFieldTTF* textField) const { return kDefaultMaximumCharacter; }
    
protected:
    virtual cocos2d::CCNode* hitTestWithTextFields(const cocos2d::CCPoint& localPos);
protected:
    cocos2d::CCPoint touchBeganPosition_;
    cocos2d::CCTextFieldTTF* inputtingNode_;
    std::vector<cocos2d::CCTextFieldTTF*> textFields_;
};


#endif
