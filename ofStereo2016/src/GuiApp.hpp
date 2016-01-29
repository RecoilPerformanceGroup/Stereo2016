//
//  GuiApp.hpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 1/29/16.
//
//

#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxXmlSettings.h"
#include "ofxOsc.h"

/**
 
 This class should implement gui elemnts in a seperate gui window
 an setup and osc listener to allow all gui elements to be controlled
 
 osc listener should be able to 
 1. directly control parameter
    parametername value
 
 2. trigger a fade from current value to target in time
    parametername fadeto value milliseconds (optional easing function)
 
 3. trigger a fade from specific value to targetvalue
    parametername fadefromto fromvalue targetvalue milliseconds (optional easing function)
 
 4. fade using specific easing functions optionally parsed as names
    integrate
 
 osc sender should broadcast all parameter changes
 
 considerations 
    -   how to implement normalisation?
            Maybe an option to allow explicitly setting all parameters 
            from a normalized value, automatically mapping to min / max in of
            would also work for values already normalised
            0-1 vs (-)1-1 normalisation
    - how to format composite parameters, vec2f, vec3f, quarternion, grayscale, rgb, rgba color ...
 
    - generic format for custom triggers
    - group parameter presets - fade from preset to preset ?  / save / load
    - groups of ui params that can be viewed and hidden together
    - applying filters
 
 **/


class GuiApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
    ofxOscReceiver oscReceiver;
    ofxOscSender oscSender;
    
    
    
    
};

