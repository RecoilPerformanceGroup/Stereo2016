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
    ofParameter<int> cellCount {"Cell count", 2,1,1000 };
    ofParameter<bool> rotateLights {"Rotate Lights", false,false, true };
    ofParameter<bool> drawWall {"drawWall", false,false, true };
    ofParameter<ofVec3f> distortionVec {"Distortion", ofVec3f(0,0,0), ofVec3f(-200,-200,-200),ofVec3f(200,200,200) };
    ofParameter<ofVec3f> wallPos {"Wall Position", ofVec3f(0,0,0), ofVec3f(-1000,-1000,-1000),ofVec3f(1000,1000,1000) };

    ofParameterGroup params {"PlaneCut",
        enabled,
        randomSeed,
        cellCount,
        rotateLights,
        drawWall,
        wallPos
    };
    
    PlaneCutScene() {        
        ofxStereoscopy::Scene::params = params;
    }
    
    void draw();
    void drawGui();
    void update();
    void setup();
    void onRandomSeed(int& value);
    void onCellCount(int& value);
    void onStageSize(ofVec3f& vec);
    void reconstructMeshes();
    
    ofLight directionalLight1;
    ofLight directionalLight2;
    
    ofLight pointlight;
    ofLight spotlight;
    
    vector<VoroNode *> voroNodes;
    
    VoroNode * vnFloor;
    VoroNode * vnWall;
    
    ofxPanel panel;
    
    OrganicMaterial matFloor;
    OrganicMaterial matWall;

    ofTexture tex;
    
};


#endif /* PlaneCutScene_hpp */
