//
//  MountainScene.cpp
//  ofStereo2016
//
//  Created by Ole Kristensen on 10/03/2016.
//
//

#include "MountainScene.hpp"

void MountainScene::setup(){
    
    ofParameter<ofVec3f> &stage_size = mainParams.getVec3f("stage_size_cm");
    
    ofSetSmoothLighting(true);
    
    mainParams.getVec3f("stage_size_cm").addListener(this, &MountainScene::onStageSize);
    
    mountainRandomSeed.addListener(this, &MountainScene::reconstructMountain<int>);
    mountainCellCount.addListener(this, &MountainScene::reconstructMountain<int>);
    mountainSize.addListener(this, &MountainScene::reconstructMountain<ofVec3f>);
    
    reconstructMountain();
}

void MountainScene::update(){
    mountain.setGlobalPosition(mountainPosition);
    mountain.setOrientation(mountainRotation);
}

void MountainScene::draw(){
    
    ofParameter<ofVec3f> &stage_size = mainParams.getVec3f("stage_size_cm");

    ofSetColor(255);

    ofEnableLighting();

    ofFloatColor mountainColor = floorColor->getLerped(anaglyphColorLeft, anaglyphAmount);
    
    matFloor.begin();
    matFloor.setDiffuseColor(mountainColor);
    matFloor.setAmbientColor(mountainColor.getLerped(ofColor::black, 0.75));
    float floorHeight = 10.0;
    float floorSize = 100000.0;
    ofDrawBox(ofPoint(0,-floorHeight/2.0, stage_size->z/2.0), floorSize, floorHeight, floorSize);
    matFloor.end();
    
    matMountain.begin();
    matMountain.setAmbientColor(mountainColor.getLerped(ofColor::black, 0.9));
    matMountain.setDiffuseColor(mountainColor);
    mountain.draw(&matMountain);
    matMountain.end();
}

void MountainScene::reconstructMountain(){
    ofSeedRandom(mountainRandomSeed);
    mountain.setup(mountainSize->x, mountainSize->y, mountainSize->z, mountainCellCount, true,true,true);
    mountain.setParent(world.origin);
}

void MountainScene::onStageSize(ofVec3f& stageSize){
    //reconstructMountain();
}

void MountainScene::drawGui(){
    ofSetColor(255);//FIXME: Why? should we not always push/pop styles? Or do this in the superclass or caller?
}