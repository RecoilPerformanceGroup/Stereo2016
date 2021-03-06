//
//  TextScene.cpp
//  ofStereo2016
//
//  Created by Stereo on 11/04/16.
//
//

#include "TextScene.hpp"

void TextScene::setup(){
    
    logo.load("STEREO-logo.svg");
    
    font.load("Novecentowide-Bold.otf", 30, true, true, true, 0.1);
    font.setLetterSpacing(0.9);

    globalParams->getVec3f("stage_size_cm").addListener(this, &TextScene::onStageSize);
    
}

void TextScene::update(){

    for(int i = 0; i < logo.getNumPath(); i++){
        logo.getPathAt(i).setFillColor(logoColor.get());
    }
    
}

void TextScene::draw(){
    
    ofSetColor(255,255);
    drawText();
    drawLogo();

}

void TextScene::drawLogo() {
    if(logoDraw){
    ofPushMatrix();
    float logoHeight = (logo.getHeight()/logo.getWidth())*logoWidth;
    ofTranslate(world->zInCam(logoPosition));
    ofTranslate(-logoWidth/2.0, logoHeight/2.0);
    ofScale(logoWidth/logo.getWidth(),logoWidth/logo.getWidth());
    logo.draw();
    ofPopMatrix();
    }
}

void TextScene::onStageSize(ofVec3f& vec){
    
}

void TextScene::drawText() {
    ofPushStyle();
    ofPushMatrix();
    ofSetColor(textColor.get());
    float scale = textSize/font.getSize();
    ofRectangle bb = font.getStringBoundingBox(textString, 0,0);
    ofTranslate(world->zInCam(textPosition.get()));
    ofScale(scale, scale);
    ofTranslate(-bb.width/2.0, -font.getSize()/2.0);
    font.drawStringAsShapes(textString, 0, 0);
    ofPopMatrix();
    ofPopStyle();
}

void TextScene::drawModel(){
    ofEnableDepthTest();
    drawText();
    drawLogo();
    ofDisableDepthTest();
}