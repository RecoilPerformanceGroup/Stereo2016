//
//  PlaneCutScene.hpp
//  ofStereo2016
//
//  Created by Ole Kristensen on 10/03/2016.
//
//

#ifndef PlaneCutScene_hpp
#define PlaneCutScene_hpp

#include "ofMain.h"
#include "ofxStereoscopy.hpp"
#include "VoroNode.hpp"
#include "ofxGui.h"
#include "OrganicMaterial.hpp"
#include "dispatch/dispatch.h"

class PlaneCutScene : public ofxStereoscopy::Scene {
    
public:

    ofParameter<int> randomSeed {"Random Seed", 0,0,10000 };
    ofParameter<int> cellCount {"Cell count", 15,1,1000 };
    ofParameter<bool> rotateLights {"Rotate Lights", false,false, true };
    ofParameter<bool> drawWall {"drawWall", false,false, true };
    ofParameter<ofVec3f> distortionVec {"Distortion", ofVec3f(0,0,0), ofVec3f(-200,-200,-200),ofVec3f(200,200,200) };
    ofParameter<ofVec3f> wallPos {"Wall Position", ofVec3f(0,0,0), ofVec3f(-1000,-1000,-1000),ofVec3f(1000,1000,1000) };

    ofParameterGroup params {"",
        enabled,
        randomSeed,
        cellCount,
        rotateLights,
        drawWall,
        wallPos
    };
    
    
    PlaneCutScene() {
        
        name = "PlaneCut";
        oscAddress = "PlaneCut";
        
        ofxStereoscopy::Scene::params = params;
        params.setName(oscAddress);
    }
    
    void draw();
    void drawGui();
    void update();
    void setup();
    void reconstructMeshes(int& value);
    
    
    ofLight directionalLight1;
    ofLight directionalLight2;
    
    ofLight pointlight;
    ofLight spotlight;
    
    vector<VoroNode *> voroNodes;
    
    ofxPanel panel;
    
    OrganicMaterial mat;
    ofMaterial matstatic;
    ofTexture tex;
    
};


#endif /* PlaneCutScene_hpp */