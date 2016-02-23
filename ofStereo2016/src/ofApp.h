#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxOsc.h"
#include "ofxEasing.h"


// todo use templates or base class with subclasses for different types
// for now I just make it work for floats


class ParameterFade {
public:
    
    
    ParameterFade(ofAbstractParameter * _p, float _toValue, float _dur, float _startTime=NULL, float _fromValue=NULL) : p(_p), duration{_dur}, toValue{_toValue}, hasEnded{false}  {
        
        startTime = _startTime ? _startTime : ofGetElapsedTimef(); // pass in to enable a wait functionality
        endTime = startTime + duration;
        
        fromValue =  _fromValue ? _fromValue : p->cast<float>().get();
        //ofxeasing::map(value, minIn, maxIn, minOut, maxOut, ofxeasing::linear::easeIn);
    }
    
    
    
    ~ParameterFade() {
    }
    
    float toValue;
    float fromValue;
    
    ofAbstractParameter * p;
    
    
    float duration;
    float startTime;
    float endTime;
    

    bool hasStarted, hasEnded;
    
    // ease function
    
    void update(float timeBase);
    // optionally frame based in stead of time base ?
    
};







class ofApp : public ofBaseApp{

	public:
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
    
    
    vector<ParameterFade *> parameterFades;
    
    void addFadeFloat(float from, float to, float duration);
    
    
    // dat gui
    
    ofxDatGuiValuePlotter* plotter;
    vector<ofxDatGuiComponent*> components;
    
    ofxDatGui* gui = new ofxDatGui( 100, 100 );

    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onSliderEvent(ofxDatGuiSliderEvent e);
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    void onMatrixEvent(ofxDatGuiMatrixEvent e);
    void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
    void on2dPadEvent(ofxDatGui2dPadEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);
    
    
    ofParameterGroup mainParams;
    
    ofParameterGroup subParams;
    ofParameter<float> float01{"float01", 1, 0, 1};
    
    ofParameter<ofColor> color01;
    
    ofParameter<ofVec2f> vec201;
    ofParameter<ofVec3f> vec301;
    
    ofParameterGroup subSubParams;
    
    ofParameter<float> float02;
    
    // osc
    ofxOscReceiver oscReceiver;
    ofxOscSender oscSender;
    
    // program variables
    float circleRadius;
    ofColor col;
    
    
    
    
};
