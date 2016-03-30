//
//  PlaneCutScene.cpp
//  ofStereo2016
//
//  Created by Ole Kristensen on 10/03/2016.
//
//

#include "PlaneCutScene.hpp"

void PlaneCutScene::setup(){
    
    ofParameter<ofVec3f> &stage_size = globalParams->getVec3f("stage_size_cm");
    
    vnFloor = new VoroNode();
    vnFloor->setupFromBoundingBox(stage_size->x+400, 50.0, stage_size->z+400, cellCount, false, true, false);
    vnFloor->setGlobalPosition(0, -25.0, stage_size->z/2.0);
    vnFloor->setParent(world->origin);
    voroNodes.push_back(vnFloor);
    vnWall = new VoroNode();
    vnWall->setupFromBoundingBox(stage_size->x+400, stage_size->y+400, 400, cellCount, true, true, true);
    vnWall->setGlobalPosition(0, stage_size->y/2.0, -200);
    vnWall->setParent(world->origin);
    voroNodes.push_back(vnWall);

    matFloor.setDiffuseColor(ofFloatColor::pink);
    matWall.setDiffuseColor(ofFloatColor::yellow);
    
    ofSetSmoothLighting(true);
    
    // directional lights emit along the +z axis
    
    directionalLight1.setup();
    directionalLight1.setDirectional();
    directionalLight1.setPosition(0, stage_size->y*0.5, stage_size->z*0.55);
    directionalLight1.setOrientation(ofVec3f(70,0,0));
    directionalLight1.setDiffuseColor(ofFloatColor::pink*0.85);
    
    directionalLight2.setup();
    directionalLight2.setDirectional();
    directionalLight2.setPosition(0, stage_size->y*0.5, stage_size->z*0.45);
    directionalLight2.lookAt(ofVec3f(0, stage_size->y*0.5, stage_size->z*0.5));
    directionalLight2.rotate(45, ofVec3f(1,1,0));
    
    directionalLight2.setDiffuseColor(ofFloatColor::blue*0.5);
    
    globalParams->getVec3f("stage_size_cm").addListener(this, &PlaneCutScene::onStageSize);
    randomSeed.addListener(this, &PlaneCutScene::onRandomSeed);
    cellCount.addListener(this, &PlaneCutScene::onCellCount);

}

void PlaneCutScene::update(){
    
    if(rotateLights){
        directionalLight2.rotate(1, ofVec3f(1,1,0));
        directionalLight1.rotate(1, ofVec3f(0,1,0));
    }
    
    voroNodes[1]->setGlobalPosition(wallPos);
    
}



void PlaneCutScene::draw(){
    
    ofSetColor(255);
   /* directionalLight1.setScale(2);
    directionalLight1.draw();
    directionalLight2.setScale(2);
    directionalLight2.draw();
    */
    ofEnableLighting();
    directionalLight1.enable();
    directionalLight2.enable();
    
    matFloor.begin();
    vnFloor->draw(&matFloor);
    matFloor.end();
    
    matWall.begin();
    vnWall->draw(&matWall);
    matWall.end();
    
    directionalLight1.disable();
    directionalLight2.disable();
    ofDisableLighting();

}

void PlaneCutScene::reconstructMeshes(){
    ofSeedRandom(randomSeed);
    vnFloor->split(cellCount, false, true, false);
    vnWall->split(cellCount, true, true, true);
}

void PlaneCutScene::onCellCount(int& value){
    reconstructMeshes();
}

void PlaneCutScene::onRandomSeed(int& value){
    reconstructMeshes();
}

void PlaneCutScene::onStageSize(ofVec3f& stageSize){
    for (auto vn : voroNodes) {
        delete vn;
    }
    voroNodes.clear();
    
    vnFloor = new VoroNode();
    vnFloor->setupFromBoundingBox(stageSize.x+400, 50.0, stageSize.z+400, cellCount);
    vnFloor->setGlobalPosition(0, -25.0, stageSize.z/2.0);
    vnFloor->setParent(world->origin);
    voroNodes.push_back(vnFloor);
    vnWall = new VoroNode();
    vnWall->setupFromBoundingBox(stageSize.x+400, stageSize.y+400, 400, cellCount);
    vnWall->setGlobalPosition(0, stageSize.y/2.0, -200);
    vnWall->setParent(world->origin);
    voroNodes.push_back(vnWall);
    //reconstructMeshes();
}

void PlaneCutScene::drawGui(){
    ofSetColor(255);//FIXME: Why? should we not always push/pop styles? Or do this in the superclass or caller?
}