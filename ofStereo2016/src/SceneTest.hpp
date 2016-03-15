//
//  sceneTest.hpp
//  ofStereo2016
//
//  Created by Ole Kristensen on 25/02/2016.
//
//

#pragma once

#include "ofMain.h"
#include "ofxStereoscopy.hpp"

class SceneTest : public ofxStereoscopy::Scene {
    
public:
    void draw();
    void update();
    void setup();
    
    ofLight light;
    
/*    void setGui(ofxUICanvas * gui, float width);
    void guiEvent(ofxUIEventArgs &e);
    void receiveOsc(ofxOscMessage * m, string rest);
  */
};

