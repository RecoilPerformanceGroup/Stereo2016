#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    shader.load("shadersGL3/shader");
    
    params.add(
    numCells.set("Cells", 40, 0, 1000),
    debugDraw.set("Debug draw", false),
    autoRotation.set("Automatic rotation", ofVec3f(0,0,0),
                     ofVec3f(-1,-1,-1),
                     ofVec3f(1,1,1)));
    
    params.add(autoRotationSpeed.set("rot speed", 0, -1, 1));
    
    params.add(crystalColor.set("Crystal color", ofColor_<float>(1,1,1,1), ofColor_<float>(0,0,0,0), ofColor_<float>(1,1,1,1)));
    
    params.add(spotlightPosition.set("spotlightPosition", ofVec3f(0,0,0), ofVec3f(-4,-4,-4), ofVec3f(4,4,4)));
    
    params.add(scale.set("scale", 0, 0, 10));
    
    params.add(scaleCells.set("scaleCells", 0,0,0.999));
    
    panel.setup(params);
    panel.loadFromFile("settings.xml");
    
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
    
    cam.setupPerspective();
    
    ofLoadImage(tex, "grain-texture.png");
    
    //grab.setup(1080, 720, true);
    randFbo.allocate(1024, 1024);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    /*ofFbo newFbo = ofFbo();
    
    float ss = ofRandom(128,512);
    newFbo.allocate(ss,ss);
    
    randFbo = newFbo;
    
    randFbo.updateTexture(1);
    
    grab.update();
    if(grab.isFrameNew()) {
        
    }*/
    
    cube->nCells = numCells;
    cube->update();
    
    float s = min(ofGetHeight(), ofGetWidth()) * scale;
    
    cam.setFarClip(s*1000);
    
    spotlight.setPosition(spotlightPosition.get() * s);
    spotlight.lookAt(ofVec3f(0,0,0));
    
    pointlight.setPosition(cam.getPosition());
    pointlight.lookAt(ofVec3f(0,0,0));
    
    mat.setDiffuseColor(crystalColor.get());
    //mat.setEmissiveColor(ofFloatColor(1,1,0.5));
    // diffuse ambient specular emissive
    //mat.setColors(crystalColor.get().getLerped(ofColor(0), 0.4), crystalColor.get().getLerped(ofColor(0), 0.2), crystalColor.get().getLerped(ofColor(255), 0.2), ofColor(0));
    
    
    
    plane.set(0.2, 0.2, 20, 20, OF_PRIMITIVE_TRIANGLES);

    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofEnableDepthTest();
    ofEnableLighting();
    ofEnableNormalizedTexCoords();
    


    ofBackground(0);
    ofSetColor(255);
    
    
    
    
    cam.begin();
    
    ofBackground(0);
    
    pointlight.enable();
    spotlight.enable();
    
    //
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
            
            //mat.begin();
            shader.begin();
            
            // a lot of the time you have to pass in variables into the shader.
            // in this case we need to pass it the elapsed time for the sine wave animation.
            shader.setUniform1f("time", ofGetElapsedTimef()*1.0);
            
            //plane.drawWireframe();
            
            for(int i = 0; i < cube->cellMeshes.size(); i++){
                ofPushMatrix(); {
                    
                    ofSetColor(255,255,255,255);
                    
                    float explode = ofMap(ofSignedNoise(ofGetElapsedTimef()), -1, 1, 0.4, 1);
                    
                    ofTranslate(cube->cellMeshes[i].getCentroid().x, cube->cellMeshes[i].getCentroid().y, cube->cellMeshes[i].getCentroid().z);
                    
                    ofScale(scaleCells,scaleCells,scaleCells);
                    
                    ofTranslate(-cube->cellMeshes[i].getCentroid().x, -cube->cellMeshes[i].getCentroid().y, -cube->cellMeshes[i].getCentroid().z);
                    
                    
                    tex.bind();

                    ofSetColor(0);
                    cube->cellMeshes[i].drawWireframe();
                    
                    ofSetColor(255);
                    cube->cellMeshes[i].drawFaces();
                    
                    //ofDrawSphere(cube->cellMeshes[i].getCentroid(), 0.005);
                    
                    tex.unbind();
                    
                    
                } ofPopMatrix();
                
            }
            
            shader.end();
            //mat.end();
            
        } ofPopMatrix();
        
    
    //grab.getTexture().unbind();
    
    
    spotlight.disable();
    pointlight.disable();
    
    

    
    
    cam.draw();
    
    if(debugDraw) {
        pointlight.draw();
        spotlight.draw();
    }
    
    cam.end();
    

    
    ofDisableNormalizedTexCoords();
    ofDisableDepthTest();
    ofDisableLighting();
    
    //tex.draw(0,0);
    panel.draw();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if(key=='r') {
        cube->generate();
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
