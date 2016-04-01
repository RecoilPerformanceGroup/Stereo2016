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

    ofParameter<float> speed {"speed", 1.0, 0.0, 2.0};

    ofParameterGroup params {"sketch",
        enabled,
        speed
    };
    
    SketchScene() {
        ofxStereoscopy::Scene::params = params;
    }
    
    ofPath path;
    vector<ofVec3f> points;
    ofVec3f noisePos;
    void draw();
    void update();
    void setup();
    void onStageSize(ofVec3f& vec);
    
    ofShader wideLines;

    void drawModel();
    
private:
    static string vertexShader;
    static string fragmentShader;
    static string geometryShader;
    
    
};

#endif /* SketchScene_hpp */
