//
//  ParameterFade.hpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 2/25/16.
//
//

#pragma once
#include "ofMain.h"

class AbstractParameterFade {
public:
    
    AbstractParameterFade(ofAbstractParameter * _p,
                          float _dur,
                          float _startTime=NULL
                          ) :
    p(_p),
    duration{_dur},
    hasEnded{false},
    isAlive(true)  {
        
        startTime = _startTime ? _startTime : ofGetElapsedTimef(); // pass in to enable wait functionality
        endTime = startTime + duration;
    }
    
    ~AbstractParameterFade() {
    }
    
    ofAbstractParameter * p;
    
    float duration;
    float startTime;
    float endTime;
    
    bool hasStarted, hasEnded, isAlive;
    
    void update(float timeBase);
    
    virtual void updateValue(float timeBase) {};    
};

template<typename ParameterType>
class ParameterFade : public AbstractParameterFade {
public:
    
    ParameterFade(ofAbstractParameter * _p,
                  ParameterType _toValue,
                  float _dur,
                  float _startTime=NULL) :
    AbstractParameterFade(_p, _dur, _startTime),
    toValue{_toValue} {
        
        //fromValue =  (_fromValue != NULL) ? _fromValue : p->cast<ParameterType>().get();
        fromValue = p->cast<ParameterType>().get();
        p->cast<ParameterType>().addListener(this, &ParameterFade::paramChanged);
    }
    
    ParameterType toValue;
    ParameterType fromValue;
    ParameterType lastValue;
    ParameterType value;
    
    void paramChanged(ParameterType & _val){
        if(_val != lastValue) {
            isAlive = false;
        }
    }
    
    void updateValue(float timeBase);
    
};
