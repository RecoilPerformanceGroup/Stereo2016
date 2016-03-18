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
    
    float floorHeight = 10.0;
    float floorSize = 100000.0;

    floor.set(floorSize, floorHeight, floorSize, 10, 10, 10);
    floor.setParent(world->origin);
    floor.setGlobalPosition(0,-floorHeight/2.0, stage_size->z/2.0);
    
    reconstructMountain();
}

void MountainScene::update(){
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

    ofFloatColor mountainColor = floorColor->getLerped(eyeColor, anaglyphAmount);
    
    matFloor.begin();
    matFloor.setDiffuseColor(mountainColor);
    matFloor.setAmbientColor(mountainColor.getLerped(ofColor::black, 0.75));
    floor.drawFaces();
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
    mountain.setParent(world->origin);
}

void MountainScene::onStageSize(ofVec3f& stageSize){
    //reconstructMountain();
}

void MountainScene::drawModel(){
    ofSetColor(255,64);
    ofNoFill();
    mountain.draw();
    floor.drawWireframe();
}