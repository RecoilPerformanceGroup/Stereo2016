#include "ofApp.h"


/**

 Structure
 
 A parameter is an ofParameter
 
 All ofParameters can be controlled by OSC usign their name 
 
 Optionally an ofParameter may also have a gui element
 The gui element is instatiated seperately 
 
 Add shorthand methods to bind gui elements to other things than sliders
 
 Use addSlider ofParameter constructor for sliders
 
 ofParameter can fit to different gui elelemts
 
 maybe a slider is saturation of color but ofParamter might still be an ofColor ... how to do this ?
 
 PArams are saved using ofxXmlSettings ? json ?
 
 
 osc is case insensitive
 
 
 // if a fade is running can a message override? YEs I think.
 // New message cancel fade
 
 
 **/


//--------------------------------------------------------------
void ofApp::setup(){
}


void ofApp::setupGui() {
    
    ofxDatGuiComponent* component;
    
    
    gui->addFRM(1.0f);
    //gui->addSlider("normalizedRange", 0, 1);
    //gui->addSlider("normalizedRange2", -1, 1);
    //gui->addSlider("arbitraryRange", 0, 1024);
    
    
    /*color01.addListener{[](float & amp){
        // do something with the amplitude
    })*/
    
    
    //vector<string> options = {"ONE", "TWO", "THREE", "FOUR"};
    //gui->addDropdown("dropdown", options);
    
    //binding
    
    //gui->getSlider("normalizedRange")->bind(circleRadius);
    gui->addSlider(float01);

    new ColorPickerFromParameter(color01, gui, true);
    
    /*[](float & amp){
        // do something with the amplitude
    })*/
    
    /*color01.addListener(this, [colorPicker](ofColor & col){
        cout<<"color ofparam change"<<endl;
        colorPicker->setColor(col);
    });*/
    
    
    //colorPicker->onColorPickerEvent(<#T *owner#>, <#void (ListenerClass::*listenerMethod)(args)#>)
    /*colorPicker->addListener(this, [this]{
        cout<<"colorpicker change"<<endl;
    });*/

    
    //gui->getColorPicker("colorpicker")->
    
    oscReceiver.setup(9999);
    
}

void ofApp::drawGui(ofEventArgs &args) {
    gui->draw();
}


void AbstractParameterFade::update(float timeBase) {
    
    hasStarted = (timeBase >= startTime);
    hasEnded = (timeBase >= endTime);
    
    if (hasStarted && !hasEnded && isAlive) {
        updateValue(timeBase);
    }
}

template<>
void ParameterFade<ofColor>::updateValue(float timeBase) {
    
        //} else if(p->type()==typeid(ofParameter<ofColor>).name()) {
    value.r = ofxeasing::map_clamp(timeBase, startTime, endTime, fromValue.r, toValue.r, ofxeasing::linear::easeIn);
    value.g = ofxeasing::map_clamp(timeBase, startTime, endTime, fromValue.g, toValue.g, ofxeasing::linear::easeIn);
    value.b = ofxeasing::map_clamp(timeBase, startTime, endTime, fromValue.b, toValue.b, ofxeasing::linear::easeIn);
    value.a = ofxeasing::map_clamp(timeBase, startTime, endTime, fromValue.a, toValue.a, ofxeasing::linear::easeIn);
        
    lastValue = value;
    p->cast<ofColor>() = value;
}

template<>
void ParameterFade<int>::updateValue(float timeBase) {
    //if(p->type() == typeid(ofParameter<float>).name() || p->type() == typeid(ofParameter<int>).name())
    value = ofxeasing::map_clamp(timeBase, startTime, endTime, fromValue, toValue, ofxeasing::linear::easeIn);
    lastValue = value;
    p->cast<int>() = value;
}

template<>
void ParameterFade<float>::updateValue(float timeBase) {
    //if(p->type() == typeid(ofParameter<float>).name() || p->type() == typeid(ofParameter<int>).name()) {
    value = ofxeasing::map_clamp(timeBase, startTime, endTime, fromValue, toValue, ofxeasing::linear::easeIn);
    lastValue = value;
    p->cast<float>() = value;
}

template<>
void ParameterFade<double>::updateValue(float timeBase) {
    //if(p->type() == typeid(ofParameter<float>).name() || p->type() == typeid(ofParameter<int>).name()) {
    value = ofxeasing::map_clamp(timeBase, startTime, endTime, fromValue, toValue, ofxeasing::linear::easeIn);
    lastValue = value;
    p->cast<double>() = value;
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    for(auto fade : parameterFades) {
        
        // todo pop from list and delete when done
        //if(fade->hasEnded) delete fade;
        
        fade->update(ofGetElapsedTimef());
    }
    
    
    while(oscReceiver.hasWaitingMessages()) {
        
        ofxOscMessage msg;
        oscReceiver.getNextMessage(msg);
        ofAbstractParameter * p = &mainParams;
        
        cout<<"Received "<<msg.getAddress()<<endl;
        
        vector<string> address = ofSplitString(ofToLower(msg.getAddress()),"/",true);
        
        // fade
        
        
        // get fade method that takes the number of expected arguments before the keyword
        // 1 for sliders
        
        bool fadeValue = false;
        float fadeTime = 2.0; // optional fade time
        // add optional fade ease function - default linear
        // optional wait param
        
        // loop through all arguments
        // if string f - next argument is time if its a float
        // if string e - next argument matches a lookup for easing functions if its an int
        // if string from - next argument is the value to start the fade from
        
        
        for(int i=0; i<msg.getNumArgs(); i++) {
            
            if(msg.getArgType(i) == OFXOSC_TYPE_STRING) {
                if(msg.getArgAsString(i) == "fade") {
                    fadeValue = true;
                    
                    if(msg.getNumArgs() > i+1) {
                        if(msg.getArgType(i+1) == OFXOSC_TYPE_FLOAT) {
                            fadeTime = msg.getArgAsFloat(i+1);
                        } else if( msg.getArgType(i+1) == OFXOSC_TYPE_INT32) {
                            fadeTime = msg.getArgAsInt(i+1);
                        }
                    }
                }
            }
            
        }
        
        for(unsigned int i=0;i<address.size();i++){
            if(p) {
                
                cout<<p->getEscapedName()<<endl;
                if(address[i]==ofToLower(p->getEscapedName())){
                    if(p->type()==typeid(ofParameterGroup).name()){
                        if(address.size()>=i+1){
                            p = &static_cast<ofParameterGroup*>(p)->get(address[i+1]);
                        }
                    }else if(p->type()==typeid(ofParameter<int>).name() && msg.getArgType(0)==OFXOSC_TYPE_INT32){
                        
                        if(fadeValue) {
                            parameterFades.push_back(new ParameterFade<int>(p, msg.getArgAsInt32(0), fadeTime));
                        } else {
                            p->cast<int>() = msg.getArgAsInt32(0);
                        }
                        
                    }else if(p->type()==typeid(ofParameter<float>).name() && msg.getArgType(0)==OFXOSC_TYPE_FLOAT){
                        
                        if(fadeValue) {
                            parameterFades.push_back(new ParameterFade<float>(p, msg.getArgAsFloat(0), fadeTime));
                        } else {
                            p->cast<float>() = msg.getArgAsFloat(0);
                        }
                        
                    }else if(p->type()==typeid(ofParameter<double>).name() && msg.getArgType(0)==OFXOSC_TYPE_DOUBLE){
                        
                        if(fadeValue) {
                            parameterFades.push_back(new ParameterFade<double>(p, msg.getArgAsDouble(0), fadeTime));
                        } else {
                            p->cast<double>() = msg.getArgAsDouble(0);
                        }
                        
                    }else if(p->type()==typeid(ofParameter<bool>).name() && (msg.getArgType(0)==OFXOSC_TYPE_TRUE
                                                                             || msg.getArgType(0)==OFXOSC_TYPE_FALSE
                                                                             || msg.getArgType(0)==OFXOSC_TYPE_INT32
                                                                             || msg.getArgType(0)==OFXOSC_TYPE_INT64
                                                                             || msg.getArgType(0)==OFXOSC_TYPE_FLOAT
                                                                             || msg.getArgType(0)==OFXOSC_TYPE_DOUBLE
                                                                             || msg.getArgType(0)==OFXOSC_TYPE_STRING
                                                                             || msg.getArgType(0)==OFXOSC_TYPE_SYMBOL)){
                        
                        // Bool doesn't fade
                        p->cast<bool>() = msg.getArgAsBool(0);
                        
                    }else if(msg.getArgType(0)==OFXOSC_TYPE_STRING){
                        
                       
                        
                        
                        if(fadeValue) {
                            
                            if(p->type()==typeid(ofParameter<ofColor>).name()) {
                                
                                ofParameter<ofColor> target;
                                target.fromString(msg.getArgAsString(0));
                                parameterFades.push_back(new ParameterFade<ofColor>(p, target.get(), fadeTime));
                            }
                            
                            
                            
                        } else {
                            p->fromString(msg.getArgAsString(0));
                        }
                        
                        // todo: osc on individual components of complex params
                        // eg. x, y,z r, g, b, a, ...
                        
                        
                        
                    }
                }
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

    
    //ofBackground(gui->getColorPicker("colorpicker")->getColor());
    
    ofSetColor(color01);
    ofDrawCircle(100,100, float01.get()*800);
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
