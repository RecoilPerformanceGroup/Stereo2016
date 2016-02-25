#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxOsc.h"
#include "ParameterFade.hpp"

// todo use templates or base class with subclasses for different types
// for now I just make it work for floats

class ColorPickerFromParameter {
public:
    
    ColorPickerFromParameter(ofParameter<ofColor> & _p, ofxDatGui * gui, bool alphaSlider=false) : p(_p) {
        
        g = gui->addColorPicker(p.getName(), p.get());
        
        p.addListener(this, &ColorPickerFromParameter::paramChangedEvent);
        g->onColorPickerEvent(this, &ColorPickerFromParameter::onColorPickerEvent);
        
        if(alphaSlider) {
            
            string name = p.getName() + " alpha";
            s = gui->addSlider(name, 0, p.get().limit(), p.get().a);
            s->setPrecision(0);
            s->onSliderEvent(this, &ColorPickerFromParameter::onSliderEvent);
        }
    }
    
    ~ColorPickerFromParameter() {
        delete g;
        if(s) delete s;
    }
    
private:
    
    ofxDatGuiColorPicker * g;
    ofxDatGuiSlider * s;
    ofParameter<ofColor> & p;
    
    void paramChangedEvent(ofColor & color) {
        g->setColor(color);
        if(s) s->setValue(p.get().a);
    }
    
    void onColorPickerEvent(ofxDatGuiColorPickerEvent e) {
        // color picker doesn't have alpha, preserve its value
        ofColor col(e.color);
        col.a = p.get().a;
        p.set(col);
    }
    
    void onSliderEvent(ofxDatGuiSliderEvent e) {
        // color picker doesn't have alpha, preserve its value
        ofColor col(p.get());
        col.a = e.value;
        p.set(col);
    }
};

class ofApp : public ofBaseApp{

public:
    
    ofParameter<float> float01{"float01", 1, 0, 1};
    ofParameter<ofColor> color01{"color01", ofColor::white,ofColor::black,ofColor::white};
    ofParameter<ofVec2f> vec201;
    ofParameter<ofVec3f> vec301;
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

    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onSliderEvent(ofxDatGuiSliderEvent e);
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    void onMatrixEvent(ofxDatGuiMatrixEvent e);
    void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
    void on2dPadEvent(ofxDatGui2dPadEvent e);
    void onTextInputEvent(ofxDatGuiTextInputEvent e);

    
    // osc
    ofxOscReceiver oscReceiver;
    ofxOscSender oscSender;
    
    // program variables
    float circleRadius;
    ofColor col;
    
};
