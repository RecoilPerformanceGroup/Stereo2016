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

    ofParameter<float> speed {"speed", 0.01, 0.0, 2.0};
    ofParameter<ofVec3f> position {"position", ofVec3f(0, 250, 400),ofVec3f(0, 250, 400),ofVec3f(0, 250, 400)};

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
    ofVec3f spherePos;
    void draw();
    void update();
    void setup();
    void onStageSize(ofVec3f& vec);
    
    ofShader wideLines;

    OrganicMaterial mat;
    
    void drawModel();
    
private:
    static string vertexShader;
    static string fragmentShader;
    static string geometryShader;
    
    
};

#endif /* SketchScene_hpp */
