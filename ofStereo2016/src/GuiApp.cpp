//
//  GuiApp.cpp
//  ofStereo2016
//
//  Created by Johan Bichel Lindegaard on 1/29/16.
//
//

#include "GuiApp.hpp"

//--------------------------------------------------------------
void GuiApp::setup(){
    
}

//--------------------------------------------------------------
void GuiApp::update(){
    
}

//--------------------------------------------------------------
void GuiApp::draw(){
    
    panel.draw();
}

void GuiApp::registerParams(ofParameterGroup & params) {
    
    paramGroups.push_back(params);
    panel.add(params);
    
}


//--------------------------------------------------------------
void GuiApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void GuiApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void GuiApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void GuiApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void GuiApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void GuiApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void GuiApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void GuiApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void GuiApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void GuiApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void GuiApp::dragEvent(ofDragInfo dragInfo){ 
    
}
