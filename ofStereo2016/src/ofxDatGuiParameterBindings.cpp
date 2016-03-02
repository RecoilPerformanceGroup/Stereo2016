//
//  ofxDatGuiParameterBindings.cpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 2/25/16.
//
//

#include "ofxDatGuiParameterBindings.hpp"

//// Vec3f
//// __________________________________

template<>
void DatGuiParameterBinding<ofVec3f>::setup() {
    
    if(widgetType == PadZ) {
        addC(gui->add2dPad(p.getName(), ofRectangle(min.x,min.y,max.x,max.y)));
        getPad()->setPoint(p.get());
    } else {
        addC(gui->addSlider(p.getName() + " x", min.x, max.x, p.get().x));
        addC(gui->addSlider(p.getName() + " y", min.y, max.y, p.get().y));
    }
    
    addC(gui->addSlider(p.getName() + " z", min.z, max.z, p.get().z));
}

template<>
void DatGuiParameterBinding<ofVec3f>::paramChangedEvent(ofVec3f & vec) {

    if(widgetType == PadZ) {
        getPad()->setPoint(vec);
        getZSlider()->setValue(vec.z);
        
    } else {
        getXSlider()->setValue(vec.x);
        getYSlider()->setValue(vec.y);
        getZSlider()->setValue(vec.z);
    }
    
}

template<>
void DatGuiParameterBinding<ofVec3f>::on2dPadEvent(ofxDatGui2dPadEvent e) {
    if(e.target == getPad()) {
        p.set(ofVec3f(e.x,e.y,p.get().z));
    }
}

template<>
void DatGuiParameterBinding<ofVec3f>::onSliderEvent(ofxDatGuiSliderEvent e) {
    
    ofVec3f vec(p.get());
    
    if(e.target == getXSlider()) {
        vec.x = e.value;
    } else if(e.target == getYSlider()) {
        vec.y = e.value;
    } else if(e.target == getZSlider()) {
        vec.z = e.value;
    }
    
    p.set(vec);
}

//// Vec2f
//// __________________________________

template<>
void DatGuiParameterBinding<ofVec2f>::setup() {
    
    if(widgetType == Pad) {
        
        addC(gui->add2dPad(p.getName(), ofRectangle(min.x,min.y,max.x,max.y)));
        getPad()->setPoint(p.get());
        
    } else {
        
        addC(gui->addSlider(p.getName() + "x", min.x, max.x, p.get().x));
        addC(gui->addSlider(p.getName() + "y", min.y, max.y, p.get().y));
    }
}

template<>
void DatGuiParameterBinding<ofVec2f>::paramChangedEvent(ofVec2f & vec) {
    
    
    if(widgetType == Pad) {
        getPad()->setPoint(vec);
        
    } else {
        getXSlider()->setValue(vec.x);
        getYSlider()->setValue(vec.y);
    }
}


template<>
void DatGuiParameterBinding<ofVec2f>::on2dPadEvent(ofxDatGui2dPadEvent e) {
    if(e.target == getPad()) {
        p.set(ofVec2f(e.x,e.y));
    }
}


template<>
void DatGuiParameterBinding<ofVec2f>::onSliderEvent(ofxDatGuiSliderEvent e) {
    
    ofVec2f vec(p.get());
    
    if(e.target == getXSlider()) {
        vec.x = e.value;
    } else if(e.target == getYSlider()) {
        vec.y = e.value;
    }
    
    p.set(vec);
}

//// COLOR
//// __________________________________
template<>
void DatGuiParameterBinding<ofColor>::setup() {
    
    //todo:  rgb type  + rgba type
    //guiCompononents.push_back(gui->addSlider(p.getName() + "x", min.x, max.x));
    //guiCompononents.push_back(gui->addSlider(p.getName() + "y", min.y, max.y));
    //guiCompononents.push_back(gui->addSlider(p.getName() + "z", min.z, max.z));
    
    if(widgetType == ColorWithAlpha) {
        
        addC(gui->addColorPicker(p.getName(), p.get()));
        
        string name = p.getName() + " alpha";
        ofxDatGuiSlider * s = gui->addSlider(name, 0, p.get().limit(), p.get().a);
        s->setPrecision(0);
        addC(s);
        
    } else if (widgetType == Color) {
        addC(gui->addColorPicker(p.getName(), p.get()));
    }
}

template<>
void DatGuiParameterBinding<ofColor>::onColorPickerEvent(ofxDatGuiColorPickerEvent e) {
    
    ofColor col(e.color);
    col.a = p.get().a;
    p.set(col);
}

template<>
void DatGuiParameterBinding<ofColor>::paramChangedEvent(ofColor & color) {
    
    getColorPicker()->setColor(color);
    
    if(widgetType == ColorWithAlpha) {
        getAlphaSlider()->setValue(p.get().a);
    }
    
}

template<>
void DatGuiParameterBinding<ofColor>::onSliderEvent(ofxDatGuiSliderEvent e) {
    
    ofColor col(p.get());
    
    if(widgetType == ColorWithAlpha) {
        
        if(e.target == getAlphaSlider()) {
            col.a = e.value;
            p.set(col);
        }
    }
}

