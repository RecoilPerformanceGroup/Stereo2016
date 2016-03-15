//
//  PlaneCutScene.cpp
//  ofStereo2016
//
//  Created by Ole Kristensen on 10/03/2016.
//
//

#include "PlaneCutScene.hpp"
#include "ofApp.h"

void PlaneCutScene::setup(){
    
    ofApp * app = (ofApp*)ofGetAppPtr();

    randomSeed.addListener(this, &PlaneCutScene::reconstructMeshes);
    cellCount.addListener(this, &PlaneCutScene::reconstructMeshes);
    
    VoroNode * vFloor = new VoroNode();
    vFloor->setup(app->stage_size_cm.get().x+400, 50.0, app->stage_size_cm.get().z+400, cellCount);
    vFloor->setGlobalPosition(0, -25.0, app->stage_size_cm.get().z/2.0);
    vFloor->setParent(app->world.origin);
    voroNodes.push_back(vFloor);
    VoroNode * vWall = new VoroNode();
    vWall->setup(app->stage_size_cm.get().x+400, app->stage_size_cm.get().y+400, 400, cellCount);
    vWall->setGlobalPosition(0, app->stage_size_cm.get().y/2.0, -200);
    vWall->setParent(app->world.origin);
    voroNodes.push_back(vWall);
    
    mat.setDiffuseColor(ofFloatColor::white);
    
    ofSetSmoothLighting(true);
    
    float falloff = 0.1/(app->stage_size_cm.get().y * 4.0 * app->world.pixels_cm);
    
    // directional lights emit along the +z axis
    
    directionalLight1.setup();
    directionalLight1.setDirectional();
    directionalLight1.setPosition(0, app->stage_size_cm.get().y*0.5, app->stage_size_cm.get().z*0.55);
    directionalLight1.setOrientation(ofVec3f(70,0,0));
    directionalLight1.setDiffuseColor(ofFloatColor::pink*0.85);
    
    directionalLight2.setup();
    directionalLight2.setDirectional();
    directionalLight2.setPosition(0, app->stage_size_cm.get().y*0.5, app->stage_size_cm.get().z*0.45);
    directionalLight2.lookAt(ofVec3f(0, app->stage_size_cm.get().y*0.5, app->stage_size_cm.get().z*0.5));
    directionalLight2.rotate(45, ofVec3f(1,1,0));
    
    directionalLight2.setDiffuseColor(ofFloatColor::blue*0.5);
}

void PlaneCutScene::update(){
    
    ofApp * app = (ofApp*)ofGetAppPtr();
    
    if(rotateLights){
        directionalLight2.rotate(1, ofVec3f(1,1,0));
        directionalLight1.rotate(1, ofVec3f(0,1,0));
    }
    
    voroNodes[1]->setGlobalPosition(wallPos);
    
}



void PlaneCutScene::draw(){
    
    ofApp * app = (ofApp*)ofGetAppPtr();

    ofSetColor(255);
   /* directionalLight1.setScale(2);
    directionalLight1.draw();
    directionalLight2.setScale(2);
    directionalLight2.draw();
    */
    ofEnableLighting();
    directionalLight1.enable();
    directionalLight2.enable();
    mat.begin();
    
   // ofDrawBox(0, app->stage_size_cm.get().y/2, -100, app->stage_size_cm.get().x, app->stage_size_cm.get().y, 200);
    voroNodes[0]->draw(&mat);

    voroNodes[1]->draw(&mat);

/*
 
    for (VoroNode * vn : voroNodes) {
        vn->draw();
    }
*/
    
/*    ofPushMatrix();
        ofTranslate(ofVec3f(app->world.physical_camera_pos_cm.get())*0.85);
        ofDrawSphere(sin(ofGetElapsedTimef())*10.0, -20, 10);
    ofPopMatrix();
*/
    mat.end();

    directionalLight1.disable();
    ofDisableLighting();

}

void PlaneCutScene::reconstructMeshes(int& value){
    
    ofSeedRandom(randomSeed);
    for (auto vn : voroNodes) {
        vn->split(cellCount, true,true, true);
    }

}

void PlaneCutScene::drawGui(){
    ofSetColor(255);//FIXME: Why? should we not always push/pop styles? Or do this in the superclass or caller?
}