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
    float floorSize = WORLD_DIMENSION_MAX*2;

    floor.set(floorSize, floorHeight, floorSize, 2, 2, 2);
    floor.setParent(world->origin);
    floor.setGlobalPosition(0,-floorHeight/2.0, stage_size->z/2.0);
    
    matFloor.noiseDisplacementVelocity = ofVec3f(0.1,0.0,0.25);
    matMountain.noiseDisplacementVelocity = ofVec3f(0.1,0.0,0.25);
    matFloor.setShininess(0.2);
    matFloor.setSpecularColor(ofColor::white);
    
    reconstructMountain();
}

void MountainScene::update(){
    matFloor.updateParameters();
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
    ofFloatColor mountainColor = floorColor->getLerped(ofColor::black, 0.1*(1.0-anaglyphAmount));
    // transition to eyeColor
    mountainColor = mountainColor.getLerped(eyeColor, anaglyphAmount);

    matMountain.begin();
    matMountain.setAmbientColor(mountainColor.getLerped(ofColor::black, 0.9))  ;
    matMountain.setWorldMatrix(floor.getGlobalTransformMatrix());
    floor.drawFaces();
    matMountain.setDiffuseColor(mountainColor);
    
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
    floor.drawWireframe();
}