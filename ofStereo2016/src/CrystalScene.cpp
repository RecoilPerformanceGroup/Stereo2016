//
//  CrystalScene.cpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 3/3/16.
//
//

#include "CrystalScene.hpp"


void CrystalScene::setup() {

    light.setPointLight();
    light.setAmbientColor(ofColor(10));
    light.setPosition(-2, -2, -2);
    
    //shader.load("shadersGL3/shader");
    //pointlight..setDirectional();
    //pointlight..setDiffuseColor(ofColor(100,55,100));
    //pointlight..setAmbientColor(ofColor(10));
    //pointlight..setSpecularColor(ofColor::blue);
    //pointlight..setAttenuation(0.81);
    
    spotlight.setSpotlight();
    spotlight.setDiffuseColor(ofColor(60,60,60));
    spotlight.setSpecularColor(200);
    
    //spotlight.setAttenuation(0.61);
    
    cube = new VoroUnit();
    cube->setup(200,100,100,numCells);
    
    ofLoadImage(tex, "grain-texture.png");
    //grab.setup(1080, 720, true);
}

void CrystalScene::draw() {
    
    ofEnableDepthTest();
    ofEnableLighting();
    ofEnableNormalizedTexCoords();
    
    ofSetColor(255);
    
    pointlight.enable();
    spotlight.enable();
    spotlight.lookAt(origin);
    
    ofPushMatrix(); {
        
        //ofTranslate(origin);
        
        //float s = min(ofGetHeight(), ofGetWidth()) * scale;
        //ofScale(s,s,s);
        
        //ofDrawBox(5, 5, 5);
        
        //ofRotateX(rotation.x);
        //ofRotateY(rotation.y);
        //ofRotateZ(rotation.z);
        
        //mat.begin();
        //shader.begin();
        
        // a lot of the time you have to pass in variables into the shader.
        // in this case we need to pass it the elapsed time for the sine wave animation.
        //shader.setUniform1f("time", ofGetElapsedTimef()*1.0);
        
        //plane.drawWireframe();
        
        /*for(auto c : cube->getChildren()) {
            
            // TODO:  node based drawing
            
        }*/
        
        /*for(int i = 0; i < cube->getChildren().size(); i++){
            ofPushMatrix(); {
                
                ofSetColor(255,255,255,255);
                
                float explode = ofMap(ofSignedNoise(ofGetElapsedTimef()), -1, 1, 0.4, 1);
                
                ofTranslate(cube->getChildren()[i]->mesh.getCentroid().x, cube->getChildren()[i]->mesh.getCentroid().y, cube->getChildren()[i]->mesh.getCentroid().z);
                
                ofScale(scaleCells,scaleCells,scaleCells);
                
                ofTranslate(-cube->getChildren()[i]->mesh.getCentroid().x, -cube->getChildren()[i]->mesh.getCentroid().y, -cube->getChildren()[i]->mesh.getCentroid().z);
                
                //tex.bind();
                
                ofSetColor(0);
                cube->getChildren()[i]->mesh.drawWireframe();
                
                ofSetColor(255);
                cube->getChildren()[i]->mesh.drawFaces();
                
                //ofDrawSphere(cube->cellMeshes[i].getCentroid(), 0.005);
                
                //tex.unbind();
                
            } ofPopMatrix();
        }*/
        
        //shader.end();
        
        mat.begin();
        tex.bind();
        cube->draw();
        tex.unbind();
        mat.end();
        
    } ofPopMatrix();
    
    spotlight.disable();
    pointlight.disable();
    
    ofDisableNormalizedTexCoords();
    ofDisableDepthTest();
    ofDisableLighting();
}

void CrystalScene::update() {
    if(rebuild) {
        cube->generate();
        
        cube->getChildren().front()->split();
        
        rebuild = false;
    }
    
    cube->nCells = numCells.get();
    cube->setPosition(origin);
    cube->update();
    
    rotation += ofGetLastFrameTime() * 60.0 * ofVec3f(
                                                      autoRotation.get().x*
                                                      autoRotationSpeed.get(),
                                                      autoRotation.get().y*
                                                      autoRotationSpeed.get(),
                                                      autoRotation.get().z
                                                      *autoRotationSpeed.get());
    
    cube->setOrientation(rotation);
    //cube->setScale(-scaleCells);
    cube->setScale(scale.get());
    
    for(auto c : cube->getChildren()) {
        
        c->setScale(scaleCells);
        
        for(auto cc : c->getChildren()) {
            
            cc->setScale(scaleCells);
        }
        
        //c->setGlobalPosition(oldPos);
        
        //ofTranslate(-c->getPosition());
    }
    
    spotlight.setPosition(spotlightPosition.get());
    spotlight.lookAt(ofVec3f(0,0,0));
    
    pointlight.lookAt(ofVec3f(0,0,0));
    mat.setDiffuseColor(crystalColor.get());
}

void CrystalScene::drawGui() {
    ofSetColor(255);
}


