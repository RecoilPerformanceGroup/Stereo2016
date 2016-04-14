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
    ofParameter<ofVec2f> lineWidth {"lineWidth", ofVec2f(10.0,10.0), ofVec2f(-100,-100), ofVec2f(100,100)};
    
    ofParameterGroup params {"game",
        enabled,
        qlab,
        lineWidth,
        length
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
