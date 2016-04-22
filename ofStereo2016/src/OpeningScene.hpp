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
    
    ofParameter<string> leftTitleString {"string", "left"};
    ofParameter<ofVec3f> leftPosition {"position", ofVec3f(0, 250, 400),ofVec3f(-2000,-2000,-2000),ofVec3f(2000,2000,2000)};
    ofParameter<float> leftTitleHeight {"height", 400.0, 0.0, 1000};
    ofParameter<ofFloatColor> leftTitleColor {"color", ofFloatColor(1.0,1.0,1.0,1.0),ofFloatColor(0.0,0.0,0.0,0.0),ofFloatColor(1.0,1.0,1.0,1.0)};

    ofParameter<string> rightTitleString {"string", "right"};
    ofParameter<ofVec3f> rightPosition {"position", ofVec3f(0, 250, 400),ofVec3f(-2000,-2000,-2000),ofVec3f(2000,2000,2000)};
    ofParameter<float> rightTitleHeight {"height", 400.0, 0.0, 1000};
    ofParameter<ofFloatColor> rightTitleColor {"color", ofFloatColor(1.0,1.0,1.0,1.0),ofFloatColor(0.0,0.0,0.0,0.0),ofFloatColor(1.0,1.0,1.0,1.0)};

    ofParameterGroup paramsLeftTitle {"leftTitle",
        leftTitleString,
        leftPosition,
        leftTitleHeight,
        leftTitleColor
    };

    ofParameterGroup paramsRightTitle {"rightTitle",
        rightTitleString,
        rightPosition,
        rightTitleHeight,
        rightTitleColor
    };
    
    ofParameterGroup params {"openingScene",
        enabled,
        enabledDraw,
        enabledDrawModel,
        qlab,
        paramsLeftTitle,
        paramsRightTitle
    };
    
    OpeningScene() {
        ofxStereoscopy::Scene::params = params;
    }
    
    void draw();
    void update();
    void setup();
    void onStageSize(ofVec3f& vec);
    void drawTitles(bool leftEye);
    
    void drawModel();
    
    ofxSVG title;
    
    ofTrueTypeFont font;
    
    
};
