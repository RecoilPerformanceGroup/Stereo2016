//
//  ofxDatGuiParameterBindings.hpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 2/25/16.
//
//

#pragma once
#include "ofMain.h"
#include "ofxDatGui.h"



// todo use templates or base class with subclasses for different types
// for now I just make it work for floats

//class DatGuiFromParameter

//class PadAndSliderFromVec3fParameter

// templatetype vec2f vec3f
// class SlidersFromVector
// creates 2 or 3 synced sliders

template<typename ParameterType>
class SlidersFromVec {
public:
    
    SlidersFromVec(ofParameter<ParameterType> & _p, ofxDatGui * _gui, bool use2DPad=false) : p(_p), gui(_gui) {
        
    }
    
    // template for
    void setup();
    
    ofParameter<ParameterType> & p;
    ofxDatGui * gui;
};


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

