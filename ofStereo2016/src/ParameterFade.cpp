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

void ParameterFadeManager::add(AbstractParameterFade * fade) {
    parameterFades.push_back(fade);
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
void ParameterFade<ofColor>::updateValue(float t) {
    
    value.r = ofxeasing::map(t, startTime, endTime, fromValue.r, toValue.r, ofxeasing::linear::easeIn);
    value.g = ofxeasing::map(t, startTime, endTime, fromValue.g, toValue.g, easeFn);
    value.b = ofxeasing::map(t, startTime, endTime, fromValue.b, toValue.b, easeFn);
    value.a = ofxeasing::map(t, startTime, endTime, fromValue.a, toValue.a, easeFn);
    
    lastValue = value;
    p->cast<ofColor>() = value;
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
