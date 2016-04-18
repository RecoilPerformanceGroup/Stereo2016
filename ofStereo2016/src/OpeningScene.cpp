//
//  OpeningScene.cpp
//  ofStereo2016
//
//  Created by Stereo on 11/04/16.
//
//

#include "OpeningScene.hpp"

void OpeningScene::setup(){
    
    font.load("Novecentowide-Bold.otf", 30, true, true, true, 0.1);
    font.setLetterSpacing(0.9);
    
 
}

void OpeningScene::update(){
    
}

void OpeningScene::draw(){
    
    ofPushStyle();
    ofDisableDepthTest();
    
    drawTitles(world->isDrawingLeft());
    
    ofPopStyle();
    
}


void OpeningScene::drawTitles(bool leftEye){
    
    if(leftEye) {
        ofPushStyle();
        ofPushMatrix();
        ofSetColor(leftTitleColor.get());
        float scale = leftTitleHeight/font.getSize();
        ofRectangle bb = font.getStringBoundingBox(leftTitleString, 0,0);
        ofTranslate(world->zInCam(leftPosition.get()));
        ofScale(scale, scale);
        ofTranslate(-bb.width/2.0, -font.getSize()/2.0);
        font.drawStringAsShapes(leftTitleString, 0, 0);
        ofPopMatrix();
        ofPopStyle();
    } else {
        ofPushStyle();
        ofPushMatrix();
        ofSetColor(rightTitleColor.get());
        float scale = rightTitleHeight/font.getSize();
        ofRectangle bb = font.getStringBoundingBox(rightTitleString, 0,0);
        ofTranslate(world->zInCam(rightPosition.get()));
        ofScale(scale, scale);
        ofTranslate(-bb.width/2.0, -font.getSize()/2.0);
        font.drawStringAsShapes(rightTitleString, 0, 0);
        ofPopMatrix();
        ofPopStyle();
    }
    
}

void OpeningScene::drawModel(){
    ofEnableDepthTest();
    drawTitles(true);
    drawTitles(false);
    ofDisableDepthTest();
}