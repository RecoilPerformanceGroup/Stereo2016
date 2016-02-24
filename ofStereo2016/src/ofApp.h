#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxOsc.h"
#include "ofxEasing.h"


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


class AbstractParameterFade {
public:
    
    AbstractParameterFade(ofAbstractParameter * _p,
                  float _dur,
                  float _startTime=NULL
                  ) :
                    p(_p),
                    duration{_dur},
                    hasEnded{false},
                    isAlive(true)  {
        
        startTime = _startTime ? _startTime : ofGetElapsedTimef(); // pass in to enable wait functionality
        endTime = startTime + duration;
        
        //fromValue =  _fromValue ? _fromValue : p->cast<float>().get();
        //ofxeasing::map(value, minIn, maxIn, minOut, maxOut, ofxeasing::linear::easeIn);
        
        /*if( p->type() == typeid(ofParameter<float>).name()) {
            p->cast<float>().addListener(this, &AbstractParameterFade::paramChanged);
            
        } else if( p->type() == typeid(ofParameter<int>).name()) {
            p->cast<int>().addListener(this, &AbstractParameterFade::intParamChanged);
        }*/
    }
    
    ~AbstractParameterFade() {
    }
    
    /*void intParamChanged(int & _val){
        if(_val != lastVal) {
            isAlive = false;
        }
    }
    
    void paramChanged(float & _val){
        if(_val != lastVal) {
            isAlive = false;
        }
    }*/
    
    //float toValue;
    //float fromValue;
    
    ofAbstractParameter * p;
    
    float duration;
    float startTime;
    float endTime;

    bool hasStarted, hasEnded, isAlive;
    
    //float lastVal;
    // ease function
    
    void update(float timeBase);
    
    virtual void updateValue() {};
    
};

template<typename ParameterType>
class ParameterFade : public AbstractParameterFade {
public:
    
    
    ParameterFade(ofAbstractParameter * _p,
                      ParameterType _toValue,
                      float _dur,
                      float _startTime=NULL,
                      ParameterType _fromValue=NULL) :
    AbstractParameterFade(_p, _dur, _startTime),
    toValue{_toValue} {
        
        
        fromValue =  _fromValue ? _fromValue : p->cast<ParameterType>().get();
        p->cast<ParameterType>().addListener(this, &ParameterFade::paramChanged);
        
        
    }
    
    ParameterType toValue;
    ParameterType fromValue;
    ParameterType lastValue;
    
    ParameterType value;
    
    void paramChanged(ParameterType & _val){
        if(_val != lastValue) {
            isAlive = false;
        }
    }
    
    void updateValue() {
        
        if(p->type() == typeid(ofParameter<float>).name() || p->type() == typeid(ofParameter<int>).name()) {
            value = ofxeasing::map_clamp(ofGetElapsedTimef(), startTime, endTime, fromValue, toValue, ofxeasing::linear::easeIn);
        }
        // Todo add easing for vec2f, 3f, color 
        
        lastValue = value;
        p->cast<ParameterType>() = value;
    }
    
    
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
    
    
    vector<AbstractParameterFade *> parameterFades;
    
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

    
    // osc
    ofxOscReceiver oscReceiver;
    ofxOscSender oscSender;
    
    // program variables
    float circleRadius;
    ofColor col;
    
    
    
    
};
