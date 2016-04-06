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

    ofParameter<float> speed {"speed", 0.01, 0.0, 10};
    ofParameter<float> radius {"radius", 20, 0.0, 800};
    ofParameter<ofVec3f> origin {"origin", ofVec3f(0, 250, 400),ofVec3f(-1000, -1000, -1000),ofVec3f(1000,1000,1000)};
    ofParameter<ofVec3f> space {"space", ofVec3f(200, 200, 200),ofVec3f(0,0,0),ofVec3f(1000,1000,1000)};
    ofParameter<float> pivotRadius {"pivotRadius", 20, 0.0, 800};
    ofParameter<float> pivotSpeed {"pivotSpeed", 0.02, 0.0, 100.0};
    ofParameter<float> positionTowardsCamera {"positionTowardsCamera", 0, 0.0, 1.0};
    ofParameter<bool> reset {"reset", true};
    //ofParameter<float> lineWidth {"lineWidth", 10.0, 0.0, 100.0};
    ofParameter<ofVec2f> lineWidth {"lineWidth", ofVec2f(10.0,10.0), ofVec2f(-100,-100), ofVec2f(100,100)};

    ofParameterGroup params {"sketch",
        enabled,
        qlab,
        lineWidth,
        speed,
        radius,
        origin,
        space,
        pivotRadius,
        pivotSpeed,
        positionTowardsCamera,
        reset
    };
    
    SketchScene() {
        ofxStereoscopy::Scene::params = params;
    }
    
    ofPath path;
    ofVbo vbo;
    ofFloatColor * colors;
    vector<ofVec3f> points;
    ofVec3f noisePos;
    ofVec3f pivotNoisePos;
    ofVec3f spherePos;
    void draw();
    void update();
    void setup();
    void onStageSize(ofVec3f& vec);
    void resetLine();
    void drawLine();
    
    ofShader wideLines;

    OrganicMaterial mat;
    
    void drawModel();
    
private:
    static string vertexShader;
    static string fragmentShader;
    static string geometryShader;
    
    
};

#endif /* SketchScene_hpp */
