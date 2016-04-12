//
//  OpeningScene.hpp
//  ofStereo2016
//
//  Created by Stereo on 11/04/16.
//
//

#pragma once

#include "ofMain.h"
#include "ofxStereoscopy.hpp"
#include "VoroNode.hpp"
#include "ofxSvg.h"

class OpeningScene : public ofxStereoscopy::Scene {
    
public:
    
    ofParameter<float> speed {"speed", 0.01, 0.0, 10};
    ofParameter<float> radius {"radius", 20, 0.0, 800};
    ofParameter<int> length {"length", 20, 0.0, 800};
    ofParameter<float> height {"height", 100, 0.0, 300};
    ofParameter<ofVec3f> space {"space", ofVec3f(200, 200, 200),ofVec3f(0,0,0),ofVec3f(1000,1000,1000)};
    ofParameter<ofFloatColor> color1 {"color1", ofFloatColor(1.0,1.0,1.0,1.0),ofFloatColor(0.0,0.0,0.0,0.0),ofFloatColor(1.0,1.0,1.0,1.0)};
    ofParameter<ofFloatColor> color2 {"color2", ofFloatColor(1.0,1.0,1.0,1.0),ofFloatColor(0.0,0.0,0.0,0.0),ofFloatColor(1.0,1.0,1.0,1.0)};
    ofParameter<float> pivotRadius {"pivotRadius", 20, 0.0, 800};
    ofParameter<float> pivotSpeed {"pivotSpeed", 0.02, 0.0, 100.0};
    ofParameter<float> positionTowardsCamera {"positionTowardsCamera", 0, 0.0, 1.0};
    ofParameter<ofVec2f> lineWidth {"lineWidth", ofVec2f(10.0,10.0), ofVec2f(-100,-100), ofVec2f(100,100)};
    ofParameter<bool> reset {"reset", true};
    
    ofParameterGroup linesParams {"lines",
        speed,
        radius,
        length,
        height,
        color1,
        color2,
        space,
        pivotRadius,
        pivotSpeed,
        lineWidth,
        positionTowardsCamera,
        reset
    };
    

    ofParameter<ofVec3f> titlePosition {"position", ofVec3f(0, 250, 400),ofVec3f(-1000,-1000,-1000),ofVec3f(1000,1000,1000)};
    ofParameter<float> titleWidth {"width", 400.0, 0.0, 1000};
    ofParameter<bool> titleTrigger {"titleTrigger", false};
    
    ofParameterGroup titleParams {"title",
        titlePosition,
        titleWidth,
        titleTrigger
    };
    
    ofParameterGroup params {"openingScene",
        enabled,
        qlab,
        linesParams,
        titleParams
    };
    
    OpeningScene() {
        ofxStereoscopy::Scene::params = params;
    }
    
    ofPath path[2];
    ofVbo vbo[2];
    deque<ofVec3f> points[2];
    ofVec3f noisePos;
    ofVec3f pivotNoisePos;
    ofVec3f spherePos;
    void draw();
    void update();
    void setup();
    void onStageSize(ofVec3f& vec);
    void resetLines();
    void drawLines();
    void drawTitle();
    
    ofShader wideLines;
    
    OrganicMaterial mat;
    
    void drawModel();
    
    ofxSVG title;
    
private:
    static string vertexShader;
    static string fragmentShader;
    static string geometryShader;
    
    
};
