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
#include "ofxJSON.h"

class qLabController{

public:
    ofxOscReceiver oscReceiver;
    ofxOscSender oscSender;
    ofxJSONElement json;
    
    float TIMEOUT = 4;
    
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
    
    string newOscCueFromString(string oscString){
        ofxOscMessage mNewOsc;
        mNewOsc.setAddress("/new");
        mNewOsc.addStringArg("osc");
        oscSender.sendMessage(mNewOsc, false);
        
        float start = ofGetElapsedTimef();
        while(!oscReceiver.hasWaitingMessages() && (ofGetElapsedTimef()-start < TIMEOUT));
        
        while(oscReceiver.hasWaitingMessages()) {
            
            ofxOscMessage msg;
            oscReceiver.getNextMessage(msg);
            
            vector<string> address = ofSplitString(msg.getAddress(),"/",true);
            
            if(address[0] == "reply" && address[address.size()-1] == "new"){
                if(json.parse(msg.getArgAsString(0))){
                    string newCueID = json["data"].asString();
                    
                    ofxOscMessage mMessageType;
                    mMessageType.setAddress("/cue/selected/messageType");
                    mMessageType.addIntArg(2);
                    oscSender.sendMessage(mMessageType, false);
                    
                    ofxOscMessage mCustomString;
                    mCustomString.setAddress("/cue/selected/customString");
                    mCustomString.addStringArg(oscString);
                    oscSender.sendMessage(mCustomString, false);

                    return newCueID;
                }
            }
        }
    };
    
    string newOscCueFromParameter(const ofAbstractParameter& p, float fadeTime = 0){
        
        string oscString( findOscAddress(p) );
        
        if(fadeTime > 0.0){
            oscString += " fade " + ofToString(fadeTime);
        }
        
        return newOscCueFromString(oscString);
        
    };

    
    string newGroupWithOscCuesFromParameterGroup(const ofParameterGroup & g){
        
        string selectionString = "";

        ////////////
        // create a dummy cue
        
        string dummyID;
        ofxOscMessage mNewDummy;
        mNewDummy.setAddress("/new");
        mNewDummy.addStringArg("memo");
        oscSender.sendMessage(mNewDummy, false);
        
        
        float start = ofGetElapsedTimef();
        while(!oscReceiver.hasWaitingMessages() && (ofGetElapsedTimef()-start < TIMEOUT));
        
        while(oscReceiver.hasWaitingMessages()) {
            
            ofxOscMessage msg;
            oscReceiver.getNextMessage(msg);
            
            vector<string> address = ofSplitString(msg.getAddress(),"/",true);
            
            if(address[0] == "reply" && address[address.size()-1] == "new"){
                if(json.parse(msg.getArgAsString(0))){
                    selectionString += json["data"].asString() + ",";
                    dummyID = json["data"].asString();
                }
            }
        }
        
        ////////////
        // create cues for children recursively

        for(std::size_t i=0;i<g.size();i++){
            if(g.getType(i)==typeid(ofParameterGroup).name()){
                selectionString += newGroupWithOscCuesFromParameterGroup(g.getGroup(i)) + ",";
            } else {
                if(g.get(i).getName() != "add to qlab")
                    selectionString += newOscCueFromParameter(g.get(i)) + ",";
            }

        }

        ////////////
        // select children before creating group

        // remove trailing comma:
        selectionString = selectionString.substr(0, selectionString.size()-1);
        
        ofxOscMessage mMessageType;
        mMessageType.setAddress("/select_id/" + selectionString);
        oscSender.sendMessage(mMessageType, false);

        ////////////
        // create group for selected cues

        ofxOscMessage mNewGroup;
        mNewGroup.setAddress("/new");
        mNewGroup.addStringArg("group");
        oscSender.sendMessage(mNewGroup, false);

        start = ofGetElapsedTimef();
        while(!oscReceiver.hasWaitingMessages() && (ofGetElapsedTimef()-start < TIMEOUT));
        
        while(oscReceiver.hasWaitingMessages()) {
            
            ofxOscMessage msg;
            oscReceiver.getNextMessage(msg);
            
            vector<string> address = ofSplitString(msg.getAddress(),"/",true);

            if(address[0] == "reply" && address[address.size()-1] == "new"){
                if(json.parse(msg.getArgAsString(0))){
                    string newCueID = json["data"].asString();
                    
                    ofxOscMessage mMode;
                    mMode.setAddress("/cue/selected/mode");
                    mMode.addIntArg(3);
                    oscSender.sendMessage(mMode, false);
                    
                    ofxOscMessage mDisplayName;
                    mDisplayName.setAddress("/cue/selected/name");
                    mDisplayName.addStringArg(g.getName());
                    //mDisplayName.addStringArg(g.getName() + " " + newCueID);
                    oscSender.sendMessage(mDisplayName, false);
                    
                    ofxOscMessage mColorName;
                    mColorName.setAddress("/cue/selected/colorName");
                    mColorName.addStringArg("grey");
                    oscSender.sendMessage(mColorName, false);
                    
                    ofxOscMessage mDeleteDummy;
                    mDeleteDummy.setAddress("/delete_id/" + dummyID);
                    oscSender.sendMessage(mDeleteDummy, false);

                    // cout << g.getName() << " " << newCueID << ": "  << selectionString << endl;
                    
                    return newCueID;
                    
                } else {
                    ofLogNotice("qLabController::newGroup") << "Failed to parse JSON.";
                }
            }
        }
    }
    
    string findOscAddress(const ofAbstractParameter& p) {
        
        string a("/");
        vector<string> h = p.getGroupHierarchyNames();
        
        for( auto s : h) {
            a += s;
            if (s != h.back()) a += "/";
        }
        if(p.type() == typeid(ofParameter<bool>).name()){
            a += " " + p.toString();
        } else {
            a += " \"" + p.toString() + "\"";
        }
        return a;
    }
    
};