//
//  ParameterFade.hpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 2/25/16.
//
//

#pragma once
#include <map>

#include "ofMain.h"
#include "ofxEasing.h"


class AbstractParameterFade {
public:
    
    AbstractParameterFade(ofAbstractParameter * _p,
                          float _dur,
                          ofxeasing::function _easeFn,
                          float _startTime
                          ) :
    p(_p),
    easeFn(_easeFn),
    duration{_dur},
    hasEnded{false},
    isAlive(true) {
        
        // pass in startTime to enable wait functionality
        startTime = _startTime ? _startTime : ofGetElapsedTimef();
        endTime = startTime + duration;
    }
    
    ~AbstractParameterFade() {
    }
    
    ofAbstractParameter * p;
    
    bool hasStarted, hasEnded, isAlive;
    
    void update(float timeBase);
    
    ofxeasing::function easeFn;
    
    virtual void updateValue(float timeBase) {};
    
    float duration;
    float startTime;
    float endTime;
    
    string c; // all __ r,g,b,a __ x,y,z __ x,y

    
};

template<typename ParameterType>
class ParameterFade : public AbstractParameterFade {
public:
    
    ParameterFade(ofAbstractParameter * _p,
                  ParameterType _toValue,
                  float _dur,
                  ofxeasing::function _easeFn=ofxeasing::linear::easeIn,
                  string _component="all",
                  float _startTime=NULL) :
    AbstractParameterFade(_p, _dur, _easeFn, _startTime),
    toValue{_toValue} {
    
        c = _component;
        
        //fromValue =  (_fromValue != NULL) ? _fromValue : p->cast<ParameterType>().get();
        fromValue = p->cast<ParameterType>().get();
        p->cast<ParameterType>().addListener(this, &ParameterFade::paramChanged);
    }
    
private:
    
    void updateValue(float timeBase);
    
    ParameterType toValue;
    ParameterType fromValue;
    ParameterType lastValue;
    ParameterType value;
    
    void paramChanged(ParameterType & _val){
        
        if(_val != lastValue && c=="all") {
            isAlive = false;
        }
    }
};

class ParameterFadeManager {
public:
    
    static const std::map<string, ofxeasing::function> EaseFunctions;
    bool hasEaseFunction(string easeFunctionName);
    ofxeasing::function getEaseFunction(string easeFunctionName);

    ParameterFadeManager() {
    }
    
    ~ParameterFadeManager() {
    }
    
    void update();
    void add(AbstractParameterFade * fade);
    bool isFadingParameter(const ofAbstractParameter & parameter);
    
private:
    vector<AbstractParameterFade *> parameterFades;
    
};



