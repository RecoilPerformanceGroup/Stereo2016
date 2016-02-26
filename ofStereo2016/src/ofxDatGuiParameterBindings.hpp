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

class AbstractDatGuiParameterBinding {
public:
    
    AbstractDatGuiParameterBinding(ofAbstractParameter & p, ofxDatGui * gui) : abstractParameter(p), gui(gui) {
        
    }
    
    ofAbstractParameter & abstractParameter;
    ofxDatGui * gui;
    vector<ofxDatGuiComponent *> guiCompononents;
    
private:
    
};



// constructors should be bale to make the follow ing types:


// colorpicker with alpha
// colopicker without alpha

// nice to have: HSV color picker

// vec2 slider with two sliders
// vec2 slider with 2dpad
// vec3slider with 3 sliders
// vec3slider with 3 2dpad and 1 slider


// bool - toggle
// bool - pad

enum WidgetType {
    Color,
    ColorWithAlpha,
    ColorHSV,
    ColorRGB,
    ColorRGBA,
    PadZ,
    Pad,
    XY,
    XYZ,
};

template<typename ParameterType>
class DatGuiParameterBinding : public AbstractDatGuiParameterBinding {
public:
    
    DatGuiParameterBinding(ofParameter<ParameterType> & p, ofxDatGui * gui, WidgetType widgetType) : AbstractDatGuiParameterBinding(p, gui), p(p), widgetType(widgetType) {
        
        min = p.getMin();
        max = p.getMax();
        
        p.addListener(this, &DatGuiParameterBinding::paramChangedEvent);
        
        setup();
        
        for(auto g : guiCompononents) {
            if(g->getType() == ofxDatGuiType::SLIDER) {
                g->onSliderEvent(this, &DatGuiParameterBinding::onSliderEvent);
                
            } else if (g->getType() == ofxDatGuiType::COLOR_PICKER) {
                g->onColorPickerEvent(this, &DatGuiParameterBinding::onColorPickerEvent);
                
            } else if (g->getType() == ofxDatGuiType::PAD2D) {
                g->on2dPadEvent(this, &DatGuiParameterBinding::on2dPadEvent);
            }
        }
        
    }
    
    ParameterType max, min;
    ofParameter<ParameterType> p;
    
    void setup();
    
    //void construct();
    
    void paramChangedEvent(ParameterType & _p) {};
    
    void onButtonEvent(ofxDatGuiButtonEvent e){};
    void onSliderEvent(ofxDatGuiSliderEvent e){
        
    };
    void onDropdownEvent(ofxDatGuiDropdownEvent e){};
    void onMatrixEvent(ofxDatGuiMatrixEvent e){};
    void onColorPickerEvent(ofxDatGuiColorPickerEvent e){};
    void on2dPadEvent(ofxDatGui2dPadEvent e){};
    void onTextInputEvent(ofxDatGuiTextInputEvent e){};
    
    WidgetType widgetType;
    
    void addC(ofxDatGuiComponent * component) {
        guiCompononents.push_back(component);
    };
    
// color specific
    ofxDatGuiColorPicker * getColorPicker() {
        return gui->getColorPicker(guiCompononents[0]->getName());
    };
    
    ofxDatGui2dPad * getPad() {
        return gui->get2dPad(guiCompononents[0]->getName());
    };
    
    ofxDatGuiSlider * getSliderByIndex(int i) {
        if(i>guiCompononents.size()) return nullptr;
        
        return gui->getSlider(guiCompononents[i]->getName());
    }
    
    ofxDatGuiSlider * getAlphaSlider() {
        return getSliderByIndex(1);
    };
    
    ofxDatGuiSlider * getXSlider() {
        return getSliderByIndex(0);
    }
    
    ofxDatGuiSlider * getYSlider() {
        return getSliderByIndex(1);
    }
    
    ofxDatGuiSlider * getZSlider() {
        if(widgetType == PadZ) return getSliderByIndex(1);
        return getSliderByIndex(2);
    }
    
};


class ColorPickerWithAlpha : public DatGuiParameterBinding<ofColor> {
public:
    ColorPickerWithAlpha(ofParameter<ofColor> & p, ofxDatGui * gui) :
    DatGuiParameterBinding(p, gui, ColorWithAlpha) {
    }
};

class ColorPicker : public DatGuiParameterBinding<ofColor> {
public:
    ColorPicker(ofParameter<ofColor> & p, ofxDatGui * gui) :
    DatGuiParameterBinding(p, gui, Color) {
    };
};

class SlidersVec3f : public DatGuiParameterBinding<ofVec3f> {
public:
    SlidersVec3f(ofParameter<ofVec3f> & p, ofxDatGui * gui) :
    DatGuiParameterBinding(p, gui, XYZ){
    };
};

class SlidersVec2f : public DatGuiParameterBinding<ofVec2f> {
public:
    SlidersVec2f(ofParameter<ofVec2f> & p, ofxDatGui * gui) :
    DatGuiParameterBinding(p, gui, XY){
    };
};


class PadAndZ : public DatGuiParameterBinding<ofVec3f> {
public:
    PadAndZ(ofParameter<ofVec3f> & p, ofxDatGui * gui) :
    DatGuiParameterBinding(p, gui, PadZ){
    };
};

class Pad2D : public DatGuiParameterBinding<ofVec2f> {
public:
    Pad2D(ofParameter<ofVec2f> & p, ofxDatGui * gui) :
    DatGuiParameterBinding(p, gui, Pad){
    };
};



/*class Vec2Sliders : public DatGuiParameterBinding<ofColor> {
public:
    ColorPicker(ofParameter<ofColor> & p, ofxDatGui * gui) :
    DatGuiParameterBinding(p, gui, Color) {
    }
};*/


/*
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
};*/

