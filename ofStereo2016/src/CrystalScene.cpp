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
    
    cube = new VoroCube();
    cube->setup(0.2,0.2,0.2,numCells);
    
    //ofLoadImage(tex, "grain-texture.png");
    
    //grab.setup(1080, 720, true);
    
    
    
}

void CrystalScene::draw() {
    
    ofEnableDepthTest();
    ofEnableLighting();
    ofEnableNormalizedTexCoords();
    
    ofSetColor(255);
    
    pointlight.enable();
    spotlight.enable();
    
    
    ofPushMatrix(); {
        
        
        ofTranslate(0, 0);
        
        float s = min(ofGetHeight(), ofGetWidth()) * scale;
        ofScale(s,s,s);
        
        //ofDrawBox(5, 5, 5);
        
        rotation += ofGetLastFrameTime() * 60.0 * ofVec3f(
                                                          autoRotation.get().x*
                                                          autoRotationSpeed.get(),
                                                          autoRotation.get().y*
                                                          autoRotationSpeed.get(),
                                                          autoRotation.get().z
                                                          *autoRotationSpeed.get());
        
        ofRotateX(rotation.x);
        ofRotateY(rotation.y);
        ofRotateZ(rotation.z);
        
        mat.begin();
        //shader.begin();
        
        // a lot of the time you have to pass in variables into the shader.
        // in this case we need to pass it the elapsed time for the sine wave animation.
        //shader.setUniform1f("time", ofGetElapsedTimef()*1.0);
        
        //plane.drawWireframe();
        
        for(int i = 0; i < cube->cellMeshes.size(); i++){
            ofPushMatrix(); {
                
                ofSetColor(255,255,255,255);
                
                float explode = ofMap(ofSignedNoise(ofGetElapsedTimef()), -1, 1, 0.4, 1);
                
                ofTranslate(cube->cellMeshes[i].getCentroid().x, cube->cellMeshes[i].getCentroid().y, cube->cellMeshes[i].getCentroid().z);
                
                ofScale(scaleCells,scaleCells,scaleCells);
                
                ofTranslate(-cube->cellMeshes[i].getCentroid().x, -cube->cellMeshes[i].getCentroid().y, -cube->cellMeshes[i].getCentroid().z);
                
                
                //tex.bind();
                
                ofSetColor(0);
                cube->cellMeshes[i].drawWireframe();
                
                ofSetColor(255);
                cube->cellMeshes[i].drawFaces();
                
                //ofDrawSphere(cube->cellMeshes[i].getCentroid(), 0.005);
                
                //tex.unbind();
                
                
            } ofPopMatrix();
            
        }
        
        //shader.end();
        mat.end();
        
    } ofPopMatrix();
    
    
    //grab.getTexture().unbind();
    
    
    spotlight.disable();
    pointlight.disable();
    
    ofDisableNormalizedTexCoords();
    ofDisableDepthTest();
    ofDisableLighting();
    
    //tex.draw(0,0);
}

void CrystalScene::update() {
    cube->nCells = numCells.get();
    cube->update();
    
    float s = min(ofGetHeight(), ofGetWidth()) * scale;
    
    spotlight.setPosition(spotlightPosition.get() * s);
    spotlight.lookAt(ofVec3f(0,0,0));
    
    pointlight.lookAt(ofVec3f(0,0,0));
    
    mat.setDiffuseColor(crystalColor.get());
    
    plane.set(0.2, 0.2, 20, 20, OF_PRIMITIVE_TRIANGLES);
}

void CrystalScene::drawGui() {
    ofSetColor(255);
    
}


