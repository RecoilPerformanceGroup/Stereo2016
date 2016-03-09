//
//  CrystalScene.hpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 3/3/16.
//
//

#pragma once

#include "ofMain.h"
#include "ofxStereoscopy.hpp"
#include "VoroNode.hpp"
#include "ofxGui.h"

#include "dispatch/dispatch.h"


class CrystalScene : public ofxStereoscopy::Scene {
    
public:
    
    ofParameter<int> numCells {"Cells", 40, 0, 1000};
    ofParameter<ofVec3f> autoRotation {"Automatic rotation", ofVec3f(0,0,0),
        ofVec3f(-1,-1,-1),
        ofVec3f(1,1,1)};
    
    ofParameter<ofVec3f> origin {"origin", ofVec3f(0,0,0),
        ofVec3f(-1000,-1000,-1000),
        ofVec3f(1000,1000,1000)};

    ofParameter<float> autoRotationSpeed {"rot speed", 0, -1, 1};
    ofParameter<ofColor_<float>> crystalColor {"Crystal color", ofColor_<float>(1,1,1,1), ofColor_<float>(0,0,0,0), ofColor_<float>(1,1,1,1)};
    ofParameter<ofVec3f> spotlightPosition {"spotlightPosition", ofVec3f(0,0,0), ofVec3f(-2000,-2000,-2000), ofVec3f(2000,2000,2000)};
    ofParameter<float> scale {"scale", 0, 0, 10};
    ofParameter<float> scaleCells {"scaleCells", 0,0,0.999};
    ofParameter<bool> debugDraw {"Debug draw", false};
    ofParameter<bool> rebuild {"rebuild", false};
    
    ofParameterGroup params {"",
        enabled,
        rebuild,
        origin,
        numCells,
        autoRotation,
        autoRotationSpeed,
        crystalColor,
        spotlightPosition,
        scale,
        scaleCells,
        debugDraw};
    
    
    CrystalScene() {
        
        name = "Crystal Test";
        oscAddress = "crystaltest";
        
        ofxStereoscopy::Scene::params = params;
        params.setName(oscAddress);
    }
    
    void draw();
    void drawGui();
    void update();
    void setup();
    
    ofLight light;
    
    vector<ofPoint> cellCentroids;
    vector<float>   cellRadius;
    vector<ofMesh>  cellMeshes;
    
    ofLight pointlight;
    ofLight spotlight;
    
    VoroNode * cube;
    
    ofShader shader;
    ofPlanePrimitive plane;
        
    ofVec3f rotation;
    
    ofxPanel panel;
    
    ofMaterial mat;
    ofTexture tex;
};



