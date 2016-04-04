//
//  PlateauScene.cpp
//  ofStereo2016
//
//

#include "PlateauScene.hpp"

void PlateauScene::setup(){
    
    ofParameter<ofVec3f> &stage_size = globalParams->getVec3f("stage_size_cm");
    
    ofSetSmoothLighting(true);
    
    globalParams->getVec3f("stage_size_cm").addListener(this, &PlateauScene::onStageSize);
    
    plateauRandomSeed.addListener(this, &PlateauScene::reconstructPlateau<int>);
    plateauCellCount.addListener(this, &PlateauScene::reconstructPlateau<int>);
    plateauSize.addListener(this, &PlateauScene::reconstructPlateau<ofVec3f>);
    
    float floorHeight = 10.0;
    float floorSize = WORLD_DIMENSION_MAX*2;
    
    floor.set(floorSize, floorHeight, floorSize, 2, 2, 2);
    floor.setParent(world->origin);
    floor.setGlobalPosition(0,-floorHeight/2.0, stage_size->z/2.0);
    
    matFloor.noiseDisplacementVelocity = ofVec3f(0.1,0.0,0.25);
    matPlateau.noiseDisplacementVelocity = ofVec3f(0.1,0.0,0.25);
    matFloor.setShininess(0.2);
    matFloor.setSpecularColor(ofColor::white);
    
    reconstructPlateau();
}

void PlateauScene::update(){
    matFloor.updateParameters();
    matPlateau.updateParameters();
    plateau.setGlobalPosition(plateauPosition);
    plateau.setOrientation(plateauRotation);
    
}

void PlateauScene::draw(){
    
    ofColor eyeColor;
    
    if(world->bIsDrawingLeft)
        eyeColor = anaglyphColorLeft;
    else
        eyeColor = anaglyphColorRight;
    
    ofParameter<ofVec3f> &stage_size = globalParams->getVec3f("stage_size_cm");
    
    ofSetColor(255);
    
    ofEnableLighting();
    
    // compensate for nonlinear transition to eyecolor
    ofFloatColor plateauColor = floorColor->getLerped(ofColor::black, 0.1*(1.0-anaglyphAmount));
    // transition to eyeColor
    plateauColor = plateauColor.getLerped(eyeColor, anaglyphAmount);
    
    matPlateau.begin();
    matPlateau.setAmbientColor(plateauColor.getLerped(ofColor::black, 0.9))  ;
    matPlateau.setWorldMatrix(floor.getGlobalTransformMatrix());
    //floor.drawFaces();
    matPlateau.setDiffuseColor(plateauColor);
    
    plateau.draw(&matPlateau);
    
    
    // split in a way where we get a new node hirearchy, doesnt work with shader stuff otherwise
    /*for( auto m : Plateau.getChildrenInSphere(selectPosition.get() - ofVec3f(selectRadius, 0, 0), selectRadius)) {
     
     ofPushMatrix();
     
     ofTranslate(-PlateauSplit, 0, 0);
     m->draw();
     
     ofPopMatrix();
     }
     
     
     for( auto m : Plateau.getChildrenInSphere(selectPosition.get() + ofVec3f(selectRadius, 0, 0), selectRadius)) {
     
     ofPushMatrix();
     
     ofTranslate(PlateauSplit, 0, 0);
     m->draw();
     
     ofPopMatrix();
     }*/
    
    matPlateau.end();
}

void PlateauScene::reconstructPlateau(){
    ofSeedRandom(plateauRandomSeed);
    
    plateau.setupFromBoundingBox(plateauSize->x, plateauSize->y, plateauSize->z, plateauCellCount, true,false,true);
    
    plateau.setParent(world->origin);
}

void PlateauScene::onStageSize(ofVec3f& stageSize){
    //reconstructPlateau();
}

void PlateauScene::drawModel(){
    ofSetColor(255,64);
    ofNoFill();
    plateau.draw();
    floor.drawWireframe();
}