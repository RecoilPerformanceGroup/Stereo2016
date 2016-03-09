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
        
        
        for(auto c : cube->getChildren()) {
            
            
            c->modMesh = c->mesh;
            
            for(int i=0; i<  c->modMesh.getNumVertices(); i++) {
                
  
                
                ofVec3f v = c->modMesh.getVertex(i);
                
                ofDrawSphere(v * c->getGlobalTransformMatrix(), 5);

                
                c->modMesh.setVertex(i, v);
            }
        }
        
        
    } ofPopMatrix();
    
    spotlight.disable();
    pointlight.disable();
    
    ofDisableNormalizedTexCoords();
    ofDisableDepthTest();
    ofDisableLighting();
}

void CrystalScene::update() {
    
    
    // Vertex displace
    
    // sine wave
    
    // twist
    
    // pull
    
    
    
    for(auto c : cube->getChildren()) {
        
        
        c->modMesh = c->mesh;
        
        for(int i=0; i<  c->modMesh.getNumVertices(); i++) {
            
            ofVec3f o = c->modMesh.getVertex(i);
            
            ofVec3f oG = c->modMesh.getVertex(i) + c->getPosition();
            
            ofVec3f v = c->modMesh.getVertex(i);
            
            
            //
            // sine curve on local Y
            //
            //float displacementHeight = 10.0;
            //float displacementY = sin(ofGetElapsedTimef() + (v.x / 10.0)) * displacementHeight;
            
            //v.y += displacementY;
            
            
            //
            // sine curve on global Y
            //
            
            /*float displacementHeight = 20.0;
            float displacementY = sin(ofGetElapsedTimef() + (oG.x / 10.0)) * displacementHeight;
            
            v.y += displacementY;*/
            
            
            //
            // noise x,y,z local
            /*v.x += ofNoise(o.z * 0.05, o.y * 0.05, ofGetElapsedTimef() * 0.5) * 10;
            v.y += ofNoise(o.x * 0.05, o.z * 0.05, ofGetElapsedTimef() * 0.5) * 10;
            v.z += ofNoise(o.y * 0.05, o.x * 0.05, ofGetElapsedTimef() * 0.5) * 10;*/
            
            
            //
            // noise x,y,z global
            /*v.x += ofNoise(oG.z * 0.05, oG.y * 0.05, ofGetElapsedTimef() * 0.5) * 10;
            v.y += ofNoise(oG.x * 0.05, oG.z * 0.05, ofGetElapsedTimef() * 0.5) * 10;
            v.z += ofNoise(oG.y * 0.05, oG.x * 0.05, ofGetElapsedTimef() * 0.5) * 10;*/
            
            
            // twist
            
            float twistAmt = sin(ofGetElapsedTimef() + (v.z / 10.0)) * 20;
            
            // get a point given a transform matrix
            
            v * c->getGlobalTransformMatrix();
            
            
            c->modMesh.setVertex(i, v);
        }
        
        //for(auto cc : c->getChildren()) {
            
            //cc->setScale(scaleCells);
            
            
        //}
        
       
    }
    

    
    
    
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


