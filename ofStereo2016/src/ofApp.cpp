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
 
 **/


//--------------------------------------------------------------
void ofApp::setup(){
        
}


void ofApp::setupGui() {
    
    ofxDatGuiComponent* component;
    
    
    mainParams.setName("mainParams");
    subParams.setName("subParams");
    
    mainParams.add(subParams);
    subParams.add(subSubParams);
    
    subParams.add(float01);
    
    subSubParams.add(float02);
    
    gui->addFRM(1.0f);
    //gui->addSlider("normalizedRange", 0, 1);
    //gui->addSlider("normalizedRange2", -1, 1);
    //gui->addSlider("arbitraryRange", 0, 1024);
    
    gui->addSlider(float01);
    
    gui->addColorPicker("colorpicker");
    
    //vector<string> options = {"ONE", "TWO", "THREE", "FOUR"};
    //gui->addDropdown("dropdown", options);
    
    //binding
    
    //gui->getSlider("normalizedRange")->bind(circleRadius);
    
    
    //paramcol.addListener(this, []{} );
    
    //gui->getColorPicker("colorpicker")->
    
    oscReceiver.setup(9999);
    
}

void ofApp::drawGui(ofEventArgs &args) {
    
    
    gui->draw();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    while(oscReceiver.hasWaitingMessages()) {
        
        ofxOscMessage msg;
        oscReceiver.getNextMessage(msg);
        ofAbstractParameter * p = &mainParams;
        
        cout<<"Received "<<msg.getAddress()<<endl;
        
        vector<string> address = ofSplitString(ofToLower(msg.getAddress()),"/",true);
        for(unsigned int i=0;i<address.size();i++){
            if(p) {
                
                cout<<p->getEscapedName()<<endl;
                if(address[i]==ofToLower(p->getEscapedName())){
                    if(p->type()==typeid(ofParameterGroup).name()){
                        if(address.size()>=i+1){
                            p = &static_cast<ofParameterGroup*>(p)->get(address[i+1]);
                        }
                    }else if(p->type()==typeid(ofParameter<int>).name() && msg.getArgType(0)==OFXOSC_TYPE_INT32){
                        p->cast<int>() = msg.getArgAsInt32(0);
                    }else if(p->type()==typeid(ofParameter<float>).name() && msg.getArgType(0)==OFXOSC_TYPE_FLOAT){
                        p->cast<float>() = msg.getArgAsFloat(0);
                    }else if(p->type()==typeid(ofParameter<double>).name() && msg.getArgType(0)==OFXOSC_TYPE_DOUBLE){
                        p->cast<double>() = msg.getArgAsDouble(0);
                    }else if(p->type()==typeid(ofParameter<bool>).name() && (msg.getArgType(0)==OFXOSC_TYPE_TRUE
                                                                             || msg.getArgType(0)==OFXOSC_TYPE_FALSE
                                                                             || msg.getArgType(0)==OFXOSC_TYPE_INT32
                                                                             || msg.getArgType(0)==OFXOSC_TYPE_INT64
                                                                             || msg.getArgType(0)==OFXOSC_TYPE_FLOAT
                                                                             || msg.getArgType(0)==OFXOSC_TYPE_DOUBLE
                                                                             || msg.getArgType(0)==OFXOSC_TYPE_STRING
                                                                             || msg.getArgType(0)==OFXOSC_TYPE_SYMBOL)){
                        p->cast<bool>() = msg.getArgAsBool(0);
                    }else if(msg.getArgType(0)==OFXOSC_TYPE_STRING){
                        p->fromString(msg.getArgAsString(0));
                    }
                }
            }
        }
        
        
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    
    //ofBackground(gui->getColorPicker("colorpicker")->getColor());
    
   // ofSetColor(gui->getColorPicker("colorpicker")->getColor().getInverted());
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
