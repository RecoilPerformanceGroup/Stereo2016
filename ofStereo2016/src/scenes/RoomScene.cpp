//
//  RoomScene.cpp
//  ofStereo2016
//
//  Created by Ole Kristensen on 25/02/2016.
//
//

#include "RoomScene.hpp"


void RoomScene::setup() {

    ofSetSmoothLighting(true);

    ofParameter<ofVec3f> &stage_size = globalParams->getVec3f("stage_size_cm");
    
    globalParams->getVec3f("stage_size_cm").addListener(this, &RoomScene::onStageSize);

    resolution = 33;
    
    floorHeight = 10.0;
    
    floor.set(stage_size->x, floorHeight, stage_size->z, stage_size->x/resolution, 2, stage_size->z/resolution);
    floor.setParent(world->origin);
    floor.setGlobalPosition(0,-floorHeight/2.0, stage_size->z/2.0);

    floorLight.enable();
    floorLight.setParent(world->origin);
    floorLight.setDirectional();
    floorLight.setGlobalPosition(0, stage_size->y, stage_size->z/2.0);
    floorLight.lookAt(-(floor.getGlobalPosition()-(2.0*floorLight.getGlobalPosition())), ofVec3f(0.0,1.0,0.0));
    
    wallThickness = 10.0;
    
    wall.set(stage_size->x, stage_size->y, wallThickness, stage_size->x/resolution, stage_size->y/resolution, 2);
    wall.setParent(world->origin);
    wall.setGlobalPosition(0,stage_size->y/2.0, -wallThickness/2.0);

    wallLight.enable();
    wallLight.setParent(world->origin);
    wallLight.setDirectional();
    wallLight.setGlobalPosition(0, stage_size->y/2.0, stage_size->z);
    wallLight.lookAt(-(wall.getGlobalPosition()-(2.0*wallLight.getGlobalPosition())), ofVec3f(0.0,1.0,0.0));
    
}

void RoomScene::update() {
    wallLight.setDiffuseColor(wallLightColor.get());
    floorLight.setDiffuseColor(floorLightColor.get());
    matFloor.updateParameters();
    matWall.updateParameters();
}

void RoomScene::draw() {

    floorLight.enable();
    matFloor.begin();
    matFloor.setAmbientColor(floorColor.get().getLerped(ofColor::black, 0.9))  ;
    matFloor.setWorldMatrix(floor.getGlobalTransformMatrix());
    matFloor.setDiffuseColor(floorColor.get());
    floor.draw();
    matFloor.end();
    floorLight.disable();
    
    wallLight.enable();
    matWall.begin();
    matWall.setAmbientColor(wallColor.get().getLerped(ofColor::black, 0.9))  ;
    matWall.setWorldMatrix(wall.getGlobalTransformMatrix());
    matWall.setDiffuseColor(wallColor.get());
    wall.draw();
    matWall.end();
    wallLight.disable(
    );
    
}

void RoomScene::drawModel(){
    ofSetColor(255,64);
    floor.drawWireframe();
    wall.drawWireframe();
}

void RoomScene::onStageSize(ofVec3f& stage_size){
    floor.set(stage_size.x, floorHeight, stage_size.z, stage_size.x/resolution, 2, stage_size.z/resolution);
    floor.setGlobalPosition(0,-floorHeight/2.0, stage_size.z/2.0);
    floorLight.setGlobalPosition(0, stage_size.y, stage_size.z/2.0);
    floorLight.lookAt(-(floor.getGlobalPosition()-(2.0*floorLight.getGlobalPosition())), ofVec3f(0.0,1.0,0.0));

    wall.set(stage_size.x, stage_size.y, wallThickness, stage_size.x/resolution, stage_size.y/resolution, 2);
    wall.setGlobalPosition(0,stage_size.y/2.0, -wallThickness/2.0);
    wallLight.setGlobalPosition(0, stage_size.y/2.0, stage_size.z);
    wallLight.lookAt(-(wall.getGlobalPosition()-(2.0*wallLight.getGlobalPosition())), ofVec3f(0.0,1.0,0.0));
   
    ofPushStyle();
    ofEnableDepthTest();
    ofEnableLighting();
    matFloor.begin();
    ofDrawSphere(floor.getGlobalPosition(), 20);
    matFloor.end();
    ofPopStyle();
    ofDisableDepthTest();
    ofSetColor(floorLightColor.get(), 63);
    floorLight.draw();
    floorLight.transformGL();
    ofDrawArrow(ofVec3f(0,0,0),ofVec3f(0,0,(floorLight.getGlobalPosition()-floor.getGlobalPosition()).length()-5), 5);
    floorLight.restoreTransformGL();

    
    
}

