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
        pad = gui->add2dPad(p.getName(), ofRectangle(min.x,min.y,max.x,max.y));
        addC(pad);
        
        pad->setPoint(p.get());
    } else {
        xSlider = gui->addSlider(p.getName() + " x", min.x, max.x, p.get().x);
        addC(xSlider);
        
        ySlider = gui->addSlider(p.getName() + " y", min.y, max.y, p.get().y);
        addC(ySlider);
    }
    
    zSlider = gui->addSlider(p.getName() + " z", min.z, max.z, p.get().z);
    addC(zSlider);
}

template<>
void DatGuiParameterBinding<ofVec3f>::paramChangedEvent(ofVec3f & vec) {

    if(widgetType == PadZ) {
        pad->setPoint(vec);
        zSlider->setValue(vec.z);
        
    } else {
        xSlider->setValue(vec.x);
        ySlider->setValue(vec.y);
        zSlider->setValue(vec.z);
    }
    
}

template<>
void DatGuiParameterBinding<ofVec3f>::on2dPadEvent(ofxDatGui2dPadEvent e) {
    p.set(ofVec3f(e.x,e.y,p.get().z));
}

template<>
void DatGuiParameterBinding<ofVec3f>::onSliderEvent(ofxDatGuiSliderEvent e) {
    
    ofVec3f vec(p.get());
    
    /*if(widgetType == PadZ) {
        PadZ
    }*/
    
    if(e.target == xSlider) {
        vec.x = e.value;
    } else if(e.target == ySlider) {
        vec.y = e.value;
    } else if(e.target == zSlider) {
        vec.z = e.value;
    }
    
    p.set(vec);
}

//// Vec2f
//// __________________________________

template<>
void DatGuiParameterBinding<ofVec2f>::setup() {
    
    if(widgetType == Pad) {
        pad = gui->add2dPad(p.getName(), ofRectangle(min.x,min.y,max.x,max.y));
        addC(pad);
        pad->setPoint(p.get());
        
    } else {
        xSlider = gui->addSlider(p.getName() + "x", min.x, max.x, p.get().x);
        addC(xSlider);
        ySlider = gui->addSlider(p.getName() + "y", min.y, max.y, p.get().y);
        addC(ySlider);
    }
}

template<>
void DatGuiParameterBinding<ofVec2f>::paramChangedEvent(ofVec2f & vec) {
    
    if(widgetType == Pad) {
        pad->setPoint(vec);
        
    } else {
        xSlider->setValue(vec.x);
        ySlider->setValue(vec.y);
    }
}


template<>
void DatGuiParameterBinding<ofVec2f>::on2dPadEvent(ofxDatGui2dPadEvent e) {
    p.set(ofVec2f(e.x,e.y));
}


template<>
void DatGuiParameterBinding<ofVec2f>::onSliderEvent(ofxDatGuiSliderEvent e) {
    
    ofVec2f vec(p.get());
    
    if(e.target == xSlider) {
        vec.x = e.value;
    } else if(e.target == ySlider) {
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
    
    colorPicker = gui->addColorPicker(p.getName(), p.get());
    addC(colorPicker);
    
    if(widgetType == ColorWithAlpha) {
        string name = p.getName() + " alpha";
        alphaSlider = gui->addSlider(name, 0, p.get().limit(), p.get().a);
        alphaSlider->setPrecision(0);
        addC(alphaSlider);
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
    
    colorPicker->setColor(color);
    
    if(widgetType == ColorWithAlpha) {
        alphaSlider->setValue(p.get().a);
    }
    
}

template<>
void DatGuiParameterBinding<ofColor>::onSliderEvent(ofxDatGuiSliderEvent e) {
    
    ofColor col(p.get());
    
    col.a = e.value;
    p.set(col);
    
    
}

