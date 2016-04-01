//
//  SketchScene.cpp
//  ofStereo2016
//
//  Created by Ole Kristensen on 10/03/2016.
//
//

#include "SketchScene.hpp"


void SketchScene::setup(){

    globalParams->getVec3f("stage_size_cm").addListener(this, &SketchScene::onStageSize);
    
    allocateFbos();
    
}

void SketchScene::update(){

}

void SketchScene::draw(){
    
    auto currentName = world->getCurrentPlane()->params.getName();
    
    int i = 0;
    
    ofFbo * currentFbo;
    
    for (auto name : planeNames) {
        if(name == currentName){
            currentFbo = &(world->isDrawingLeft()? leftFbos[i]:rightFbos[i] );
        }
        i++;
    }
    currentFbo->begin();
    ofClear(0,255);
//    ofSetColor(255,200);
//    currentFbo->draw(ofPoint(0,0,0));
    ofSetColor(255,255);

    ofDrawSphere(ofNoise(ofGetElapsedTimef())*200,
                 (ofNoise(ofGetElapsedTimef()+123)*200)+200,
                 ofNoise(ofGetElapsedTimef()+2343245)*200, 20);
 
    currentFbo->end();
    ofSetColor(255,0,0,255);
    currentFbo->draw(-world->getCurrentPlane()->getWidth()/2.0, -world->getCurrentPlane()->getHeight());
    
}

void SketchScene::onStageSize(ofVec3f& vec){
    allocateFbos();
}

void SketchScene::allocateFbos(){
    
    leftFbos.clear();
    rightFbos.clear();
    planeNames.clear();
    
    for(auto plane : world->planes ){
        leftFbos.push_back(ofFbo( plane.second->fboLeft));
        rightFbos.push_back(ofFbo( plane.second->fboRight));
        planeNames.push_back(plane.first);
    }
    
}