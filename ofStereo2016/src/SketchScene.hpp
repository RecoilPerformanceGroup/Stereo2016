//
//  SketchScene.hpp
//  ofStereo2016
//
//  Created by Ole Kristensen on 10/03/2016.
//
//

#ifndef SketchScene_hpp
#define SketchScene_hpp

#include "ofMain.h"
#include "ofxStereoscopy.hpp"
#include "VoroNode.hpp"
#include "ofxGui.h"

class SketchScene : public ofxStereoscopy::Scene {

public:

    ofParameter<int> sketch {"Sketch", 0, 0, 10};

    ofParameterGroup params {"sketch",
        enabled,
        sketch};
    
    
    SketchScene() {
        ofxStereoscopy::Scene::params = params;

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


#endif /* SketchScene_hpp */
