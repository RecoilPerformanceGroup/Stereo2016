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
        sketch
    
    };
    
    SketchScene() {
        ofxStereoscopy::Scene::params = params;
    }

    vector <ofFbo> leftFbos;
    vector <ofFbo> rightFbos;
    vector <string> planeNames;
    
    
    void draw();
    void update();
    void setup();
    void onStageSize(ofVec3f& vec);
    void allocateFbos();
    
};


#endif /* SketchScene_hpp */
