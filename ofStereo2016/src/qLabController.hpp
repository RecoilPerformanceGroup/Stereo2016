//
//  qLabController.hpp
//  ofStereo2016
//
//  Created by Ole Kristensen on 30/03/2016.
//
//

#pragma once
#include <iostream>
#include <ofMain.h>
#include "ofxOsc.h"

class qLabController{

public:
    ofxOscReceiver oscReceiver;
    ofxOscSender oscSender;
    
    qLabController(){};

    void setup(string sendAddress = "localhost", int sendPort = 53000, int receivePort = 53001){
        oscSender.setup(sendAddress, sendPort);
        oscReceiver.setup(receivePort);
    };
    
    void update(){
        while(oscReceiver.hasWaitingMessages()) {
            
            ofxOscMessage msg;
            oscReceiver.getNextMessage(msg);
            cout << "UPDATE" << endl;
            cout << msg.getAddress() << endl;;
            for(int i = 0; i < msg.getNumArgs(); i++){
                cout << "\t" << msg.getArgAsString(i) << endl;
            }
        }
    };
    
    void newOscCueFromString(string oscString){
        ofxOscMessage mNewOsc;
        mNewOsc.setAddress("/new");
        mNewOsc.addStringArg("osc");
        oscSender.sendMessage(mNewOsc, false);
        
        ofxOscMessage mMessageType;
        mMessageType.setAddress("/cue/selected/messageType");
        mMessageType.addIntArg(2);
        oscSender.sendMessage(mMessageType, false);
        
        ofxOscMessage mCustomString;
        mCustomString.setAddress("/cue/selected/customString");
        mCustomString.addStringArg(oscString);
        oscSender.sendMessage(mCustomString, false);
    };
    
    void newOscCueFromParameter(ofAbstractParameter * p, float fadeTime = 0){
        
        string oscString( findOscAddress(p) );
        
        if(fadeTime > 0.0){
            oscString += " fade " + ofToString(fadeTime);
        }
        
        newOscCueFromString(oscString);
        
    };

    
    void newGroupWithOscCuesFromParameterGroup(const ofParameterGroup & g, string qlabRoot = "/"){
        
        ofxOscMessage mNewGroup;
        mNewGroup.setAddress(qlabRoot+"new");
        mNewGroup.addStringArg("group");
        oscSender.sendMessage(mNewGroup, false);
        
        while(!oscReceiver.hasWaitingMessages());
        
        while(oscReceiver.hasWaitingMessages()) {
            
            ofxOscMessage msg;
            oscReceiver.getNextMessage(msg);
            cout << msg.getAddress() << endl;
            
            vector<string> address = ofSplitString(msg.getAddress(),"/",true);

            if(address[0] == "reply"){
                for(int i = 0; i < msg.getNumArgs(); i++){
                    cout << "\t" << msg.getArgAsString(i) << endl;
                }
            }
        }
        
        ofxOscMessage mMode;
        mMode.setAddress(qlabRoot+"cue/selected/mode");
        mMode.addIntArg(3);
        oscSender.sendMessage(mMode, false);
        
        ofxOscMessage mDisplayName;
        mDisplayName.setAddress(qlabRoot+"cue/selected/name");
        mDisplayName.addStringArg(g.getName());
        oscSender.sendMessage(mDisplayName, false);
        
        for(std::size_t i=0;i<g.size();i++){
            if(g.getType(i)==typeid(ofParameterGroup).name()){
//                newGroupWithOscCuesFromParameterGroup(g.getGroup(i), "/cue_id/"+ guidString+"/");
            }
        }
        
    }
    
    string findOscAddress(ofAbstractParameter * p) {
        
        string a("/");
        vector<string> h = p->getGroupHierarchyNames();
        
        for( auto s : h) {
            a += s;
            if (s != h.back()) a += "/";
        }
        
        a += " \"" + p->toString() + "\"";
        
        return a;
    }

};