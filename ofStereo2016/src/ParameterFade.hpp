//
//  ParameterFade.hpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 2/25/16.
//
//

#pragma once
#include "ofMain.h"
#include "ofxEasing.h"

class AbstractParameterFade {
public:
    
    // easing lookup from http://easings.net/
    
    AbstractParameterFade(ofAbstractParameter * _p,
                          float _dur,
                          string _easing,
                          float _startTime
                          ) :
    p(_p),
    duration{_dur},
    hasEnded{false},
    isAlive(true) {
        
        easeF = getEaseFunction(_easing);
        
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
    
    typedef std::function<float(float, float, float, float)> function;

    function easeF;
    
    function getEaseFunction(string e) {
        
        
        using namespace ofxeasing;
        
        if(e == "linear")         return linear::easeIn;
        
        if(e == "easeInSine")       return sine::easeIn;
        if(e == "easeOutSine")      return sine::easeOut;
        if(e == "easeInOutSine")    return sine::easeInOut;
           
        if(e == "easeInQuad")       return quad::easeIn
        if(e == "easeOutQuad")      return quad::easeOut
        if(e == "easeInOutQuad")    return quad::easeInOut
            
        if(e == "easeInCubic")      return cubic::easeIn;
        if(e == "easeOutCubic")     return cubic::easeOut;
        if(e == "easeInOutCubic")   return cubic::easeInOut;
           
        if(e == "easeInQuart")      return quart::easeIn;
        if(e == "easeOutQuart")     return quart::easeOut;
        if(e == "easeInOutQuart")   return quart::easeInOut;
            
        if(e == "easeInQuint")      return quint::easeIn;
        if(e == "easeOutQuint")     return quint::easeOut;
        if(e == "easeInOutQuint")   return quint::easeInOut;
            
        if(e == "easeInExpo")       return expo::easeIn;
        if(e == "easeOutExpo")      return expo::easeOut;
        if(e == "easeInOutExpo")    return expo::easeInOut;
            
        if(e == "easeInCirc")       return circ::easeIn;
        if(e == "easeOutCirc")      return circ::easeOut;
        if(e == "easeInOutCirc")    return circ::easeInOut;
            
        if(e == "easeInBack")       return back::easeIn;
        if(e == "easeOutBack")      return back::easeOut;
        if(e == "easeInOutBack")    return back::easeInOut;
            
        if(e == "easeInElastic")    return elastic::easeIn;
        if(e == "easeOutElastic")   return elastic::easeOut;
        if(e == "easeInOutElastic") return elastic::easeInOut;
            
        if(e == "easeInBounce")     return bounce::easeIn;
        if(e == "easeOutBounce")    return bounce::easeOut;
        if(e == "easeInOutBounce")  return bounce::easeInOut;
            
    }
    
    virtual void updateValue(float timeBase) {};
};

template<typename ParameterType>
class ParameterFade : public AbstractParameterFade {
public:
    
    ParameterFade(ofAbstractParameter * _p,
                  ParameterType _toValue,
                  float _dur,
                  string _easing="linear",
                  float _startTime=NULL) :
    AbstractParameterFade(_p, _dur, _easing, _startTime),
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

class ParameterFadeManager {
public:
    
    ParameterFadeManager() {
    }
    
    ~ParameterFadeManager() {
    }
    
    void update();
    
    void add(AbstractParameterFade * fade);
    
    //private:
    vector<AbstractParameterFade *> parameterFades;
    
};

