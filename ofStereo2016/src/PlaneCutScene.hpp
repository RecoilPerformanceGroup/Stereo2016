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
#include "dispatch/dispatch.h"

class PlaneCutScene : public ofxStereoscopy::Scene {
    
public:

    ofParameter<int> randomSeed {"Random Seed", 0,0,10000 };
    ofParameter<int> cellCount {"Cell count", 15,1,1000 };

    ofParameterGroup params {"",
        enabled,
        randomSeed,
        cellCount
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
    
    ofLight directionalLight1;
    ofLight directionalLight2;
    
    ofLight pointlight;
    ofLight spotlight;
    
    vector<VoroNode *> voroNodes;
    
    ofxPanel panel;
    
    ofMaterial mat;
    ofTexture tex;
    
};


#endif /* PlaneCutScene_hpp */
