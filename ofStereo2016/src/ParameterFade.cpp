//
//  ParameterFade.cpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 2/25/16.
//

#include "ParameterFade.hpp"

void ParameterFadeManager::update() {
    
    //cout<<"updating: "<< parameterFades.size() << " fades"<<endl;
    
    for(auto fade : parameterFades) {
        //todo pop from list and delete when done
        //if(fade->hasEnded) delete fade;
        
        fade->update(ofGetElapsedTimef());
    }
    
    vector<AbstractParameterFade *>::iterator it;
    for(it=parameterFades.begin();it!=parameterFades.end();){
        if(!(*it)->isAlive) {
            delete (*it);
            it = parameterFades.erase(it);
        } else {
            ++it;
        }
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
        
    for(auto & fade : parameterFades) {
            if(fade->p == newFade->p) {
                // Components interrupt all
                if (fade->c == newFade->c || newFade->c == "all" || fade->c == "all") {
                    fade->isAlive = false;
                }
            }
    }
    
    parameterFades.push_back(newFade);
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


template<>
void ParameterFade<float>::updateValue(float t) {
    
    float value = getCurrentValue();
    
    if(value != lastValue ) {
        isAlive = false;
    }
    
    value = ofxeasing::map_clamp(t, startTime, endTime, fromValue, toValue, easeFn);
    
    lastValue = value;
    p->cast<float>() = value;
    
    if(value == toValue) isAlive = false;
};

template<>
void ParameterFade<int>::updateValue(float t) {
    
    int value = getCurrentValue();
    
    if(value != lastValue ) {
        isAlive = false;
    }
    
    value = ofxeasing::map_clamp(t, startTime, endTime, fromValue, toValue, easeFn);
    
    lastValue = value;
    p->cast<int>() = value;
    
    if(value == toValue) isAlive = false;
};

template<>
void ParameterFade<double>::updateValue(float t) {
    
    double value = getCurrentValue();
    
    if(value != lastValue ) {
        isAlive = false;
    }
    
    value = ofxeasing::map_clamp(t, startTime, endTime, fromValue, toValue, easeFn);
    
    lastValue = value;
    p->cast<double>() = value;
    
    if(value == toValue) isAlive = false;
};


template<>
void ParameterFade<ofColor>::updateValue(float t) {
    
    ofColor val = getCurrentValue();
    
    if( (val  != lastValue    && c == "all") ||
       (val.r != lastValue.r  && c == "r")   ||
       (val.g != lastValue.g  && c == "g")   ||
       (val.b != lastValue.b  && c == "b")   ||
       (val.a != lastValue.a  && c == "a") )
    {
        isAlive = false;
    }
    
    if(c == "all" || c == "r") {
        val.r = ofxeasing::map_clamp(t, startTime, endTime, fromValue.r, toValue.r, easeFn);
    }
    
    if(c == "all" || c == "g") {
        val.g = ofxeasing::map_clamp(t, startTime, endTime, fromValue.g, toValue.g, easeFn);
    }
    
    if(c == "all" || c == "b") {
        val.b = ofxeasing::map_clamp(t, startTime, endTime, fromValue.b, toValue.b, easeFn);
    }
    
    if(c == "all" || c == "a") {
        val.a = ofxeasing::map_clamp(t, startTime, endTime, fromValue.a, toValue.a, easeFn);
    }
    
    lastValue = val;
    p->cast<ofColor>() = val;
}

template<>
void ParameterFade<ofVec3f>::updateValue(float t) {
    
    ofVec3f val = getCurrentValue();
    
    if( (val   != lastValue    && c == "all") ||
       (val.x != lastValue.x  && c == "x")   ||
       (val.y != lastValue.y  && c == "y")   ||
       (val.z != lastValue.z  && c == "z") )
    {
        isAlive = false;
        //cout<<"parameter fade: for " << p->getName() << ":" << c << " "  << fromValue << " - " << toValue<< " killed by other"<<endl;
    }
    
    if(c == "all" || c == "x") {
        val.x = ofxeasing::map_clamp(t, startTime, endTime, fromValue.x, toValue.x, easeFn);
    }
    if(c == "all" || c == "y") {
        val.y = ofxeasing::map_clamp(t, startTime, endTime, fromValue.y, toValue.y, easeFn);
    }
    if(c == "all" || c == "z") {
        val.z = ofxeasing::map_clamp(t, startTime, endTime, fromValue.z, toValue.z, easeFn);
    }
    
    lastValue = val;
    p->cast<ofVec3f>() = val;
    
    if( (val   == toValue    && c == "all") ||
       (val.x == toValue.x  && c == "x")   ||
       (val.y == toValue.y  && c == "y")   ||
       (val.z == toValue.z  && c == "z") )
    {
        isAlive = false;
        //cout<<"parameter fade: for " << p->getName() << ":" << c << " "  << fromValue << " - " << toValue<< " complete"<<endl;
    }

}

template<>
void ParameterFade<ofVec2f>::updateValue(float t) {
    
    ofVec2f val = getCurrentValue();
    
    if( (val   != lastValue    && c == "all") ||
       (val.x != lastValue.x  && c == "x")   ||
       (val.y != lastValue.y  && c == "y") )
    { isAlive = false; }
    
    if(c == "all" || c == "x") {
        val.x = ofxeasing::map_clamp(t, startTime, endTime, fromValue.x, toValue.x, easeFn);
    }
    if(c == "all" || c == "y") {
        val.y = ofxeasing::map_clamp(t, startTime, endTime, fromValue.y, toValue.y, easeFn);
    }
    
    lastValue = val;
    p->cast<ofVec2f>() = val;
    
    if( (val   == toValue    && c == "all") ||
       (val.x == toValue.x  && c == "x")   ||
       (val.y == toValue.y  && c == "y")
       )
    {
        isAlive = false;
        //cout<<"parameter fade: for " << p->getName() << ":" << c << " "  << fromValue << " - " << toValue<< " complete"<<endl;
    }
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
