//
//  GameScene.hpp
//  ofStereo2016
//
//  Created by Ole Kristensen on 10/03/2016.
//
//

#pragma once

#include "ofMain.h"
#include "ofxStereoscopy.hpp"
#include "VoroNode.hpp"
#include "ofxGui.h"

class GameScene : public ofxStereoscopy::Scene {
    
public:
    
    ofParameter<float> length {"length", 0, 0.0, 1.0};
    ofParameter<float> startDistance {"distance", 100, 0.0, 1000.0};
    ofParameter<ofVec2f> lineWidth {"lineWidth", ofVec2f(10.0,10.0), ofVec2f(-100,-100), ofVec2f(100,100)};
    ofParameter<ofVec3f> lineOffset {"lineOffset", ofVec3f(0, 30.0, 0), ofVec3f(-1000,-1000,-1000), ofVec3f(1000,1000,1000)};
    
    ofParameterGroup params {"game",
        enabled,
        qlab,
        lineWidth,
        length,
        startDistance,
        lineOffset
    };
    
    GameScene() {
        ofxStereoscopy::Scene::params = params;
    }
    
    ofPath path;
    ofVbo vbo;
    ofFloatColor * colors;
    vector<ofVec3f> points;
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
