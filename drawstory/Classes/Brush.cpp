//
//  Brush.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-4-2.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "Brush.h"
#include "DrawSolidCircle.h"

using namespace cocos2d;



enum {
    kTextureLength = kBrushMax + 2,
};

Brush::Brush() : texture_(NULL), width_(kBrushWidth8), color_(ccRED) {
    
}

Brush::~Brush() {
    CCLOG("Brush::~Brush()");
    CC_SAFE_RELEASE(texture_);
}


bool Brush::init() {
    bool result = false;
    do {
        // Initialize render textures
        // Width from 2.0f to 32.0f, so the max size of render texture is {32x32}
        texture_ = CCRenderTexture::renderTextureWithWidthAndHeight(kTextureLength, kTextureLength);
        CC_BREAK_IF(!texture_);
        texture_->retain();
        updateTexture();
        
        
        result = true;
    } while (0);
    return result;
}


void Brush::updateTexture() {
    texture_->beginWithClear(0, 0, 0, 0);
    glLineWidth(1.0f);
    glColor4f(color_.r, color_.g, color_.b, 255.0f);
    drawSolidCircle(CCPointMake(kTextureLength / 2, kTextureLength / 2), width_ / 2, 0, 50);
    texture_->end();
}


void Brush::visit() {
    texture_->getSprite()->visit();
}

void Brush::setPosition(const cocos2d::CCPoint &pos) {
    texture_->getSprite()->setPosition(pos);
}

void Brush::setWidth(BrushWidth width) {
    if(width_ != width) {
        width_ = width;
        updateTexture();
    }
}

void Brush::setColor(const cocos2d::ccColor3B &color) {
    if(color_.r != color.r || color_.g != color.g || color_.b != color.b) {
        color_ = color;
        updateTexture();
    }
}


