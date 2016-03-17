//
//  ParameterFade.cpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 2/25/16.
//

#include "ParameterFade.hpp"

void ParameterFadeManager::update() {
    
    for(auto fade : parameterFades) {
        //todo pop from list and delete when done
        //if(fade->hasEnded) delete fade;
        
        fade->update(ofGetElapsedTimef());
    }
}

bool ParameterFadeManager::isFadingParameter(const ofAbstractParameter & p) {
    for(auto fade : parameterFades) {
        if(fade->p == &p) {
            return true;
        }
    }
    return false;
}

void ParameterFadeManager::add(AbstractParameterFade * newFade) {
    
    bool _bAddNew = true;
    
    for(auto & fade : parameterFades) {
        if(fade->p == newFade->p) {
           
            // Components interrupt all
            if (fade->c == newFade->c || newFade->c == "all" || fade->c == "all") {
                fade = newFade;
                _bAddNew = false;
                
            }
        }
    }
    
    if(_bAddNew) parameterFades.push_back(newFade);
}

bool ParameterFadeManager::hasEaseFunction(string easeFunctionName) {
    return (EaseFunctions.find(easeFunctionName) != EaseFunctions.end());
}

ofxeasing::function ParameterFadeManager::getEaseFunction(string easeFunctionName) {
    
    if(hasEaseFunction(easeFunctionName)) {
       return EaseFunctions.find(easeFunctionName)->second;
    } else {
        ofLogWarning()<<"easeFunction not found for: '"<<easeFunctionName<<endl;
        return NULL;
    }
}

void AbstractParameterFade::update(float t) {
    
    hasStarted = (t >= startTime);
    hasEnded = (t >= endTime);
    
    if (hasStarted && !hasEnded && isAlive) {
        updateValue(t);
    }
}

template<>
void ParameterFade<ofColor>::paramChanged(ofColor & _val){
    
    if( (_val   != lastValue    && c == "all") ||
        (_val.r != lastValue.r  && c == "r")   ||
        (_val.g != lastValue.g  && c == "g")   ||
        (_val.b != lastValue.b  && c == "b")   ||
        (_val.a != lastValue.a  && c == "a") )
        { isAlive = false; }
    
}

template<>
void ParameterFade<ofVec3f>::paramChanged(ofVec3f & _val){
    
    if( (_val   != lastValue    && c == "all") ||
        (_val.x != lastValue.x  && c == "x")   ||
        (_val.y != lastValue.y  && c == "y")   ||
        (_val.z != lastValue.z  && c == "z") )
        { isAlive = false; }
    
}

template<>
void ParameterFade<ofVec2f>::paramChanged(ofVec2f & _val){
    
    if( (_val   != lastValue    && c == "all") ||
        (_val.x != lastValue.x  && c == "x")   ||
        (_val.y != lastValue.y  && c == "y") )
        { isAlive = false; }
    
}

template<>
void ParameterFade<ofColor>::updateValue(float t) {
    
    value = p->cast<ofColor>().get();

    if(c == "all" || c == "r") {
        value.r = ofxeasing::map(t, startTime, endTime, fromValue.r, toValue.r, ofxeasing::linear::easeIn);
    }
    
    if(c == "all" || c == "g") {
        value.g = ofxeasing::map(t, startTime, endTime, fromValue.g, toValue.g, easeFn);
    }
    
    if(c == "all" || c == "b") {
        value.b = ofxeasing::map(t, startTime, endTime, fromValue.b, toValue.b, easeFn);
    }
    
    if(c == "all" || c == "a") {
        value.a = ofxeasing::map(t, startTime, endTime, fromValue.a, toValue.a, easeFn);
    }
    
    lastValue = value;
    p->cast<ofColor>() = value;
}

template<>
void ParameterFade<ofVec3f>::updateValue(float t) {
    
    value = p->cast<ofVec3f>().get();
    
    if(c == "all" || c == "x") {
        value.x = ofxeasing::map(t, startTime, endTime, fromValue.x, toValue.x, ofxeasing::linear::easeIn);
    }
    if(c == "all" || c == "y") {
        value.y = ofxeasing::map(t, startTime, endTime, fromValue.y, toValue.y, easeFn);
    }
    if(c == "all" || c == "z") {
        value.z = ofxeasing::map(t, startTime, endTime, fromValue.z, toValue.z, easeFn);
    }
    
    lastValue = value;
    p->cast<ofVec3f>() = value;
}

template<>
void ParameterFade<ofVec2f>::updateValue(float t) {
    
    value = p->cast<ofVec2f>().get();
    
    if(c == "all" || c == "x") {
        value.x = ofxeasing::map(t, startTime, endTime, fromValue.x, toValue.x, ofxeasing::linear::easeIn);
    }
    if(c == "all" || c == "y") {
        value.y = ofxeasing::map(t, startTime, endTime, fromValue.y, toValue.y, easeFn);
    }
    
    lastValue = value;
    p->cast<ofVec2f>() = value;
}


template<>
void ParameterFade<int>::updateValue(float t) {
    value = ofxeasing::map(t, startTime, endTime, fromValue, toValue, easeFn);
    lastValue = value;
    p->cast<int>() = value;
}

template<>
void ParameterFade<float>::updateValue(float t) {
    value = ofxeasing::map(t, startTime, endTime, fromValue, toValue, easeFn);
    lastValue = value;
    p->cast<float>() = value;
}

template<>
void ParameterFade<double>::updateValue(float t) {
    value = ofxeasing::map(t, startTime, endTime, fromValue, toValue, easeFn);
    lastValue = value;
    p->cast<double>() = value;
}

using namespace ofxeasing;

// easing lookup from http://easings.net/
const std::map<string, ofxeasing::function> ParameterFadeManager::EaseFunctions {
    {"linear",           linear::easeIn},
    
    {"easeInSine",       sine::easeIn},
    {"easeOutSine",      sine::easeIn},
    {"easeInOutSine",    sine::easeIn},
    
    {"easeInQuad",       quad::easeIn},
    {"easeOutQuad",      quad::easeOut},
    {"easeInOutQuad",    quad::easeInOut},
    
    {"easeInCubic",      cubic::easeIn},
    {"easeOutCubic",     cubic::easeOut},
    {"easeInOutCubic",   cubic::easeInOut},
    
    {"easeInQuart",      quart::easeIn},
    {"easeOutQuart",     quart::easeOut},
    {"easeInOutQuart",   quart::easeInOut},
    
    {"easeInQuint",      quint::easeIn},
    {"easeOutQuint",     quint::easeOut},
    {"easeInOutQuint",   quint::easeInOut},
    
    {"easeInExpo",       exp::easeIn},
    {"easeOutExpo",      exp::easeOut},
    {"easeInOutExpo",    exp::easeInOut},
    
    {"easeInCirc",       circ::easeIn},
    {"easeOutCirc",      circ::easeOut},
    {"easeInOutCirc",    circ::easeInOut},
    
    {"easeInBack",       back::easeIn},
    {"easeOutBack",      back::easeOut},
    {"easeInOutBack",    back::easeInOut},
    
    {"easeInElastic",    elastic::easeIn},
    {"easeOutElastic",   elastic::easeOut},
    {"easeInOutElastic", elastic::easeInOut},
    
    {"easeInBounce",     bounce::easeIn},
    {"easeOutBounce",    bounce::easeOut},
    {"easeInOutBounce",  bounce::easeInOut},
};
