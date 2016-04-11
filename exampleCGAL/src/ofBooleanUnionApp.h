//
//  ofBooleanUnionApp.h
//  Created by Lukasz Karluk on 13/12/13.
//

#pragma once

#include "ofMain.h"
#include "ofxCGALBooleanOp.h"

class ofBooleanUnionApp : public ofBaseApp {
public:
    
    void setup() {
        ofSetFrameRate(30);
        ofSetSphereResolution(4);
        
        bShowResult = false;
        
        float radius = 200;
        mesh1 = ofMesh::box(300,300,300,10,10,10);
        mesh1.mergeDuplicateVertices();

        mesh2 = ofMesh::box(600,100,120,10,4,4);
        mesh2.mergeDuplicateVertices();
        
    }
    
    void update() {
        //
    }
    
    void draw() {
        cam.begin();
        
        if(bShowResult == true) {
            ofSetColor(ofColor::white);
            meshResult.drawWireframe();
        } else {
            ofSetColor(ofColor::red);
            mesh1.drawWireframe();
            mesh2.drawWireframe();
        }
        
        ofSetColor(ofColor::white);
        
        cam.end();
        
        int x = 20;
        int y = 20;
        ofDrawBitmapString("Press '1' for OF_BOOLEAN_OPERATOR_UNION", x, y);
        ofDrawBitmapString("Press '2' for OF_BOOLEAN_OPERATOR_DIFFERENCE", x, y+=20);
        ofDrawBitmapString("Press '3' for OF_BOOLEAN_OPERATOR_INTERSECTION", x, y+=20);
        ofDrawBitmapString("Press '4' for OF_BOOLEAN_OPERATOR_COMPLEMENT", x, y+=20);
        
        x = ofGetWidth() - 100;
        y = 20;
        ofDrawBitmapString(ofToString(ofGetFrameRate()) + " fps", x, y);
    }
    
    void keyPressed(int key) {
        
        bShowResult = true;
        
        if(key == '1') {
            meshResult = ofxCGALBooleanOp::runOp(mesh1, mesh2, OF_BOOLEAN_OPERATOR_UNION);
        } else if(key == '2') {
            meshResult = ofxCGALBooleanOp::runOp(mesh1, mesh2, OF_BOOLEAN_OPERATOR_DIFFERENCE);
        } else if(key == '3') {
            meshResult = ofxCGALBooleanOp::runOp(mesh1, mesh2, OF_BOOLEAN_OPERATOR_INTERSECTION);
        } else if(key == '4') {
            meshResult = ofxCGALBooleanOp::runOp(mesh1, mesh2, OF_BOOLEAN_OPERATOR_COMPLEMENT);
        } else {
            bShowResult = false;
        }
    }
    
protected:
    
    ofEasyCam cam;
    ofMesh mesh1;
    ofMesh mesh2;
    ofMesh meshResult;
    ofBoxPrimitive box;

    bool bShowResult;
};
