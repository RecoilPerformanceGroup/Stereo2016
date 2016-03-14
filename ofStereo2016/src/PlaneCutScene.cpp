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
    
    VoroNode * vFloor = new VoroNode();
    vFloor->setup(app->stage_size_cm.get().x-100, 50.0, app->stage_size_cm.get().z-100, cellCount);
    vFloor->setGlobalPosition(0, -25.0, app->stage_size_cm.get().z/2.0);
//    voroNodes.push_back(vFloor);
    VoroNode * vWall = new VoroNode();
    vWall->setup(app->stage_size_cm.get().x, app->stage_size_cm.get().y, 100, cellCount);
    vWall->setGlobalPosition(0, app->stage_size_cm.get().y/2.0, -50);
    voroNodes.push_back(vWall);
    
    mat.setDiffuseColor(ofFloatColor::white);
    
    ofSetSmoothLighting(true);
    
    float falloff = 0.1/(app->stage_size_cm.get().y * 4.0 * app->world.pixels_cm);
    
    // directional lights emit along the +z axis
    
    directionalLight1.setup();
    directionalLight1.setDirectional();
    directionalLight1.setPosition(0, app->stage_size_cm.get().y*0.5, app->stage_size_cm.get().z*0.5);
    directionalLight1.setOrientation(ofVec3f(70,0,0));
    directionalLight1.setDiffuseColor(ofFloatColor::wheat*0.85);
    
    directionalLight2.setup();
    directionalLight2.setDirectional();
    directionalLight2.setPosition(0, app->stage_size_cm.get().y*0.5, app->stage_size_cm.get().z*0.5);
    directionalLight2.setOrientation(ofVec3f(45,-45,0));
    directionalLight2.setDiffuseColor(ofFloatColor::blue);
}

void PlaneCutScene::update(){
    
    ofApp * app = (ofApp*)ofGetAppPtr();
    
    ofSeedRandom(randomSeed);
    for (auto vn : voroNodes) {
            vn->split(cellCount, true, true, true);
            for (auto * vnChild : vn->getChildren()) {
                vnChild->modMesh = vnChild->mesh;
            }
    }
}

void PlaneCutScene::draw(){
    
    ofApp * app = (ofApp*)ofGetAppPtr();

    ofSetColor(255);
    directionalLight1.draw();
    directionalLight2.draw();
    ofEnableLighting();
    directionalLight1.enable();
    directionalLight2.enable();
    mat.begin();
    
//    ofDrawBox(0, -10, app->stage_size_cm.get().z/2.0, app->stage_size_cm.get().x, 10, app->stage_size_cm.get().z);
    
    for (VoroNode * vn : voroNodes) {
        vn->draw();
    }

/*    ofPushMatrix();
        ofTranslate(ofVec3f(app->world.physical_camera_pos_cm.get())*0.85);
        ofDrawSphere(sin(ofGetElapsedTimef())*10.0, -20, 10);
    ofPopMatrix();
*/
    mat.end();
    directionalLight1.disable();
    ofDisableLighting();

}

void PlaneCutScene::drawGui(){
    ofSetColor(255);//FIXME: Why? should we not always push/pop styles? Or do this in the superclass or caller?
}