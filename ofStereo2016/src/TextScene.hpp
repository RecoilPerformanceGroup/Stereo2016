//
//  TextScene.hpp
//  ofStereo2016
//
//  Created by Stereo on 11/04/16.
//
//

#pragma once

#include "ofMain.h"
#include "ofxStereoscopy.hpp"
#include "ofxSvg.h"

class TextScene : public ofxStereoscopy::Scene {
    
public:
    
    ofParameter<ofVec3f> textPosition {"position", ofVec3f(0, 250, 400),ofVec3f(-2000,-2000,-2000),ofVec3f(2000,2000,2000)};
    ofParameter<float> textSize {"size", 30, 0.0, 1000};
    ofParameter<ofFloatColor> textColor {"color", ofFloatColor(1.0,1.0,1.0,1.0),ofFloatColor(0.0,0.0,0.0,0.0),ofFloatColor(1.0,1.0,1.0,1.0)};
    ofParameter<string> textString {"string", ""};
    
    ofParameterGroup textParams {"text",
        textPosition,
        textSize,
        textString,
        textColor
    };
    
    ofParameter<float> logoWidth {"width", 0.0, 0.0, 1000};
    ofParameter<ofVec3f> logoPosition {"position", ofVec3f(0, 250, 400),ofVec3f(-2000,-2000,-2000),ofVec3f(2000,2000,2000)};
    ofParameter<ofFloatColor> logoColor {"color", ofFloatColor(1.0,1.0,1.0,1.0),ofFloatColor(0.0,0.0,0.0,0.0),ofFloatColor(1.0,1.0,1.0,1.0)};
    ofParameter<bool> logoDraw {"draw", false};
    
    
    //ofParameter<bool> recoilLogoDraw {"recoilLogoDraw", false};
    
    ofParameterGroup logoParams {"logo",
        logoWidth,
        logoPosition,
        logoColor,
        logoDraw,
        //recoilLogoDraw
    };

    ofParameterGroup params {"TextScene",
        enabled,
        enabledDraw,
        enabledDrawModel,
        qlab,
        textParams,
        logoParams
    };
    
    TextScene() {
        ofxStereoscopy::Scene::params = params;
    }
    
    ofTrueTypeFont font;
    
    ofxSVG logo;
    
    
    
    void draw();
    void update();
    void setup();
    void onStageSize(ofVec3f& vec);
    void drawText();
    void drawModel();
    void drawLogo();
    
};
