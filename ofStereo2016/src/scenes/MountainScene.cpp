//
//  MountainScene.cpp
//  ofStereo2016
//
//  Created by Ole Kristensen on 10/03/2016.
//
//

#include "MountainScene.hpp"

void MountainScene::setup(){
    
    ofParameter<ofVec3f> &stage_size = globalParams->getVec3f("stage_size_cm");
        
    ofSetSmoothLighting(true);
    
    globalParams->getVec3f("stage_size_cm").addListener(this, &MountainScene::onStageSize);
    
    mountainRandomSeed.addListener(this, &MountainScene::reconstructMountain<int>);
    mountainCellCount.addListener(this, &MountainScene::reconstructMountain<int>);
    mountainSize.addListener(this, &MountainScene::reconstructMountain<ofVec3f>);
    
    matMountain.noiseDisplacementVelocity = ofVec3f(0.1,0.0,0.25);
    reconstructMountain();
}

void MountainScene::update(){
    matMountain.updateParameters();
    mountain.setGlobalPosition(mountainPosition);
    mountain.setOrientation(mountainRotation);
}

void MountainScene::draw(){

    ofColor eyeColor;
    
    if(world->bIsDrawingLeft)
        eyeColor = anaglyphColorLeft;
    else
        eyeColor = anaglyphColorRight;
    
    ofParameter<ofVec3f> &stage_size = globalParams->getVec3f("stage_size_cm");

    ofSetColor(255);

    ofEnableLighting();

    // compensate for nonlinear transition to eyecolor
    ofFloatColor mColor = mountainColor->getLerped(ofColor::black, 0.1*(1.0-anaglyphAmount));
    // transition to eyeColor
    mColor = mColor.getLerped(eyeColor, anaglyphAmount);

    matMountain.begin();
    matMountain.setAmbientColor(mColor.getLerped(ofColor::black, 0.9))  ;
    matMountain.setWorldMatrix(mountain.getGlobalTransformMatrix());
    matMountain.setDiffuseColor(mColor);
    
    mountain.draw(&matMountain);
    
    
    // split in a way where we get a new node hirearchy, doesnt work with shader stuff otherwise
    /*for( auto m : mountain.getChildrenInSphere(selectPosition.get() - ofVec3f(selectRadius, 0, 0), selectRadius)) {
        
        ofPushMatrix();
        
        ofTranslate(-mountainSplit, 0, 0);
        m->draw();
        
        ofPopMatrix();
    }
    
    
    for( auto m : mountain.getChildrenInSphere(selectPosition.get() + ofVec3f(selectRadius, 0, 0), selectRadius)) {
        
        ofPushMatrix();
        
        ofTranslate(mountainSplit, 0, 0);
        m->draw();
        
        ofPopMatrix();
    }*/
    
    matMountain.end();
}

void MountainScene::reconstructMountain(){
    ofSeedRandom(mountainRandomSeed);
    mountain.setupFromBoundingBox(mountainSize->x, mountainSize->y, mountainSize->z, mountainCellCount, true,true,true);
    mountain.setParent(world->origin);
}

void MountainScene::onStageSize(ofVec3f& stageSize){
    //reconstructMountain();
}

void MountainScene::drawModel(){
    ofSetColor(255,64);
    ofNoFill();
    mountain.draw();
}