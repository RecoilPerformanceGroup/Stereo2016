#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxOsc.h"
#include "ParameterFade.hpp"
#include "ofxDatGuiParameterBindings.hpp"

class ofApp : public ofBaseApp{

public:
    
    ofParameter<float> float01{"float01", 1, 0, 1};
    ofParameter<ofColor> color01{"color01", ofColor::white,ofColor::black,ofColor::white};
    ofParameter<ofVec2f> vec201{"position1", ofVec3f{50,50,50}, ofVec3f{0,0,0}, ofVec3f{100,100,100}};
    ofParameter<ofVec3f> vec301{"position", ofVec3f{50,50,50}, ofVec3f{0,0,0}, ofVec3f{100,100,100}};
    ofParameter<float> float02;
    
    ofParameterGroup subSubParams{
        "subSubParams",
        vec301
    };
    
    ofParameterGroup subParams{
        "subParams",
        subSubParams,
        float01,
        float02,
        color01};
    
    ofParameterGroup mainParams{
        "mainParams",
        subParams,
        vec201
    };
    
		void setup();
        void setupGui();
		void update();
		
        void draw();
        void drawGui(ofEventArgs & args);

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
    
    ParameterFadeManager * fadeManager;
    
    // dat gui
    ofxDatGuiValuePlotter* plotter;
    vector<ofxDatGuiComponent*> components;
    
    ofxDatGui* gui = new ofxDatGui( 100, 100 );

    
    // osc
    ofxOscReceiver oscReceiver;
    ofxOscSender oscSender;
    
    // program variables
    float circleRadius;
    ofColor col;
    
};
