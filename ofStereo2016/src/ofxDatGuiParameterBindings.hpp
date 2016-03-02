//
//  ofxDatGuiParameterBindings.hpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 2/25/16.
//
//

#pragma once
#include <memory>
#include "ofMain.h"
#include "ofxDatGui.h"

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

class AbstractDatGuiParameterBinding {
public:
    
    AbstractDatGuiParameterBinding(ofAbstractParameter & p, ofxDatGui * gui) : abstractParameter(p), gui(gui) {
    }
    
    ofAbstractParameter & abstractParameter;
    ofxDatGui * gui;
    vector<ofxDatGuiComponent *> guiCompononents;
    
private:
};

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
        
        setup();
        
        p.addListener(this, &DatGuiParameterBinding::paramChangedEvent);
        
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
    void onSliderEvent(ofxDatGuiSliderEvent e){};
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
    ofxDatGuiColorPicker * colorPicker;
    ofxDatGui2dPad * pad;
    ofxDatGuiSlider * alphaSlider;
    ofxDatGuiSlider * xSlider;
    ofxDatGuiSlider * ySlider;
    ofxDatGuiSlider * zSlider;
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

class GuiBindingManager {
public:
    
    
    
};


