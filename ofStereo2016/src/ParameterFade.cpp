//
//  ParameterFade.cpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 2/25/16.
//
//

#include "ParameterFade.hpp"
#include "ofxEasing.h"

void ParameterFadeManager::update() {
    
    for(auto fade : parameterFades) {
        
        // todo pop from list and delete when done
        //if(fade->hasEnded) delete fade;
        
        fade->update(ofGetElapsedTimef());
    }
}

void ParameterFadeManager::add(AbstractParameterFade * fade) {
    parameterFades.push_back(fade);
}


void AbstractParameterFade::update(float t) {
    
    hasStarted = (t >= startTime);
    hasEnded = (t >= endTime);
    
    if (hasStarted && !hasEnded && isAlive) {
        updateValue(t);
    }
}

template<>
void ParameterFade<ofColor>::updateValue(float t) {
    
    value.r = ofxeasing::map_clamp(t, startTime, endTime, fromValue.r, toValue.r, ofxeasing::linear::easeIn);
    value.g = ofxeasing::map_clamp(t, startTime, endTime, fromValue.g, toValue.g, ofxeasing::linear::easeIn);
    value.b = ofxeasing::map_clamp(t, startTime, endTime, fromValue.b, toValue.b, ofxeasing::linear::easeIn);
    value.a = ofxeasing::map_clamp(t, startTime, endTime, fromValue.a, toValue.a, ofxeasing::linear::easeIn);
    
    lastValue = value;
    p->cast<ofColor>() = value;
    
}

template<>
void ParameterFade<int>::updateValue(float t) {
    value = ofxeasing::map_clamp(t, startTime, endTime, fromValue, toValue, ofxeasing::linear::easeIn);
    lastValue = value;
    p->cast<int>() = value;
}

template<>
void ParameterFade<float>::updateValue(float t) {
    value = ofxeasing::map_clamp(t, startTime, endTime, fromValue, toValue, ofxeasing::linear::easeIn);
    lastValue = value;
    p->cast<float>() = value;
}

template<>
void ParameterFade<double>::updateValue(float t) {
    value = ofxeasing::map_clamp(t, startTime, endTime, fromValue, toValue, ofxeasing::linear::easeIn);
    lastValue = value;
    p->cast<double>() = value;
}
