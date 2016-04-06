#include "ofApp.h"

#pragma mark GLOBALS

string jsonString = "";

bool onUpdate = false;
bool eInitRequest = false;

ofxJSONElement paramUpdate;

#pragma mark APP

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetLogLevel(OF_LOG_NOTICE);

    ofSetSmoothLighting(true);

    oscSender.setup("localhost", 53000);
    qlab.setup();
    
    globalParams.add(world.params);
    
    for( auto s : scenes) {
        s->setupScene(&globalParams, &world);
    }
    
    ofAddListener(globalParams.parameterChangedE(), this, &ofApp::paramsChanged);
    
    //globalParams.getParameter().addListener(this,&ofApp::paramChanged);
    
    fadeManager = make_shared<ParameterFadeManager>();
    
    world.calibrator.updateOutputAspect((ofGetWidth()*0.5)/ofGetHeight());

    ofEnableAlphaBlending();
    ofSetCircleResolution(66);
    
    ofxDatGuiFolder * projectorCalibrationFolder = gui->getFolder("Projector Calibration");
    
    world.addPlane(make_shared<ofxStereoscopy::Plane>(
                                                      "WALL",
                                                      800.0,
                                                      500.0,
                                                      ofVec3f(0,250,0),
                                                      ofQuaternion(0, ofVec3f(1,0,0)),
                                                      &world
                                                      ));
    
    (projectorCalibrationFolder->addToggle("WALL LEFT", false))->onButtonEvent(this,&ofApp::onButtonEvent);
    (projectorCalibrationFolder->addToggle("WALL RIGHT", false))->onButtonEvent(this,&ofApp::onButtonEvent);
    
    world.addPlane(make_shared<ofxStereoscopy::Plane>(
                                                      "FLOOR",
                                                      800.0,
                                                      800.0,
                                                      ofVec3f(0,0,400),
                                                      ofQuaternion(-90, ofVec3f(1,0,0)),
                                                      &world
                                                      ));
    
    (projectorCalibrationFolder->addToggle("FLOOR LEFT", false))->onButtonEvent(this,&ofApp::onButtonEvent);
    (projectorCalibrationFolder->addToggle("FLOOR RIGHT", false))->onButtonEvent(this,&ofApp::onButtonEvent);
    
    for (auto p : world.planes) {
        indirectParams.push_back(vector<string> { p.first, "size"});
    }
    
    world.calibrator.linkPointParameters(
                                         &world.getPlane("FLOOR")->quadLeft.outputPointTopLeft,
                                         &world.getPlane("WALL")->quadLeft.outputPointBottomLeft
                                         );
    world.calibrator.linkPointParameters(
                                         &world.getPlane("FLOOR")->quadLeft.outputPointTopRight,
                                         &world.getPlane("WALL")->quadLeft.outputPointBottomRight
                                         );
    world.calibrator.linkPointParameters(
                                         &world.getPlane("FLOOR")->quadRight.outputPointTopLeft,
                                         &world.getPlane("WALL")->quadRight.outputPointBottomLeft
                                         );
    world.calibrator.linkPointParameters(
                                         &world.getPlane("FLOOR")->quadRight.outputPointTopRight,
                                         &world.getPlane("WALL")->quadRight.outputPointBottomRight
                                         );
    

    stage_size_cm.addListener(this, &ofApp::stageResized);
    
    loadAllParameters();
    
#ifdef WEBPARAMS
    //setup sync for GUI
    paramSync.setupFromParamGroup(globalParams);
    
    //listen to parameter changes from ofxSynchedParams
    ofAddListener(paramSync.paramChangedE,this,&ofApp::parameterChanged);
    
    //setup web socket server
    ofxLibwebsockets::ServerOptions options = ofxLibwebsockets::defaultServerOptions();
    options.port = 9092;
    options.bUseSSL = false; // you'll have to manually accept this self-signed cert if 'true'!
    options.documentRoot = ofToDataPath("web"); // that is the default anyway but so you know how to change it :)
    server.setup( options );
    server.addListener(this);
#endif /* WEBPARAMS */
}

#ifdef WEBPARAMS
//--------------------------------------------------------------
void ofApp::parameterChanged( std::string & paramAsJsonString ){
    //	ofLogVerbose("ofDatGuiApp::parameterChanged");
    if(!onUpdate)
        server.send( paramAsJsonString );
}
#endif /* WEBPARAMS */

void ofApp::receiveOscParameter(ofxOscMessage & msg, ofAbstractParameter * _p) {
    
    ofAbstractParameter * p = _p;
    
    cout<<"Received "<<msg.getAddress()<<endl;
    
    vector<string> address = ofSplitString(msg.getAddress(),"/",true);
    // fade
    
    // get fade method that takes the number of expected arguments before the keyword
    // 1 for sliders
    
    bool fadeValue = false;
    float fadeTime = 2.0; // optional fade time
    ofxeasing::function easeFn = ofxeasing::linear::easeIn;
    // add optional fade ease function - default linear
    // optional wait param
    
    // loop through all arguments
    // if string f - next argument is time if its a float
    // if string e - next argument matches a lookup for easing functions if its an int
    // if string from - next argument is the value to start the fade from
    
    for(int i=0; i<msg.getNumArgs(); i++) {
        cout<<msg.getArgAsString(i)<<endl;

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
            } else if (fadeManager->hasEaseFunction(msg.getArgAsString(i))){
                //cout<<msg.getArgAsString(i)<<endl;
                easeFn = fadeManager->getEaseFunction(msg.getArgAsString(i));
            }
        }
    }
    
    for(unsigned int i=0;i<address.size();i++){
        
        if(p) {
            if(address[i]==p->getEscapedName()){
                
                if(p->type()==typeid(ofParameterGroup).name()){
                    if(address.size()>=i+1){
                        p = &static_cast<ofParameterGroup*>(p)->get(address[i+1]);
                        
                    }
                }else if(p->type()==typeid(ofParameter<int>).name() && msg.getArgType(0)==OFXOSC_TYPE_INT32){
                    
                    if(fadeValue) {
                        fadeManager->add(new ParameterFade<int>(p, msg.getArgAsInt32(0), fadeTime, easeFn));
                    } else {
                        p->cast<int>() = msg.getArgAsInt32(0);
                    }
                    
                }else if(p->type()==typeid(ofParameter<float>).name() && msg.getArgType(0)==OFXOSC_TYPE_FLOAT){
                    if(fadeValue) {
                        fadeManager->add(new ParameterFade<float>(p, msg.getArgAsFloat(0), fadeTime, easeFn));
                    } else {
                        p->cast<float>() = msg.getArgAsFloat(0);
                    }
                    
                }else if(p->type()==typeid(ofParameter<double>).name() && msg.getArgType(0)==OFXOSC_TYPE_DOUBLE){
                    if(fadeValue) {
                        fadeManager->add(new ParameterFade<double>(p, msg.getArgAsDouble(0), fadeTime, easeFn));
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
                    
                } else if(p->type()==typeid(ofParameter<ofColor>).name() &&
                          msg.getArgType(0)!=OFXOSC_TYPE_STRING ) {
                    
                    ofColor col = p->cast<ofColor>();
                    float val = msg.getArgAsFloat(0);
                    
                    // size check
                    string suf = address.back();
                    
                    if(suf == "r") {
                        col.r = val;
                    } else if(suf == "g") {
                        col.g = val;
                    } else if(suf == "b") {
                        col.b = val;
                    } else if(suf == "a") {
                        col.a = val;
                    }
                    
                    if(fadeValue) {
                        fadeManager->add(new ParameterFade<ofColor>(p, col, fadeTime, easeFn, suf));
                    } else {
                        p->cast<ofColor>().set(col);
                    }
                    break;
                    
                } else if(p->type()==typeid(ofParameter<ofFloatColor>).name() &&
                            msg.getArgType(0)!=OFXOSC_TYPE_STRING ) {
                        
                        ofFloatColor col = p->cast<ofFloatColor>();
                        float val = msg.getArgAsFloat(0);
                        
                        // size check
                        string suf = address.back();
                        
                        if(suf == "r") {
                            col.r = val;
                        } else if(suf == "g") {
                            col.g = val;
                        } else if(suf == "b") {
                            col.b = val;
                        } else if(suf == "a") {
                            col.a = val;
                        }
                        
                        if(fadeValue) {
                            fadeManager->add(new ParameterFade<ofFloatColor>(p, col, fadeTime, easeFn, suf));
                        } else {
                            p->cast<ofFloatColor>().set(col);
                        }
                        break;

                    
                } else if(p->type()==typeid(ofParameter<ofVec3f>).name() &&
                          msg.getArgType(0)!=OFXOSC_TYPE_STRING ) {
                    
                    ofVec3f vec = p->cast<ofVec3f>();
                    float val = msg.getArgAsFloat(0);
                    
                    // size check
                    string suf = address.back();

                    // TODO: first time with a suffix all others are set to null, when loaded from file. when all set via osc, suf retains others. maybe loading from file fails to set a certain state?
                    
                    if(suf == "x") {
                        vec.x = val;
                    } else if(suf == "y") {
                        vec.y = val;
                    } else if(suf == "z") {
                        vec.z = val;
                    }
                    
                    if(fadeValue) {
                        fadeManager->add(new ParameterFade<ofVec3f>(p, vec, fadeTime, easeFn, suf));
                    } else {
                        p->cast<ofVec3f>().set(vec);
                    }
                    break;
                    
                } else if(p->type()==typeid(ofParameter<ofVec2f>).name() &&
                          msg.getArgType(0)!=OFXOSC_TYPE_STRING ) {
                    
                    ofVec2f vec = p->cast<ofVec2f>();
                    float val = msg.getArgAsFloat(0);
                    
                    // size check
                    string suf = address.back();
                    
                    if(suf == "x") {
                        vec.x = val;
                    } else if(suf == "y") {
                        vec.y = val;
                    }
                    
                    if(fadeValue) {
                        fadeManager->add(new ParameterFade<ofVec2f>(p, vec, fadeTime, easeFn, suf));
                    } else {
                        p->cast<ofVec2f>().set(vec);
                    }
                    break;
                    
                    
                    
                } else if(msg.getArgType(0)==OFXOSC_TYPE_STRING){
                    
                    if(fadeValue) {
                        
                        if(p->type()==typeid(ofParameter<ofColor>).name()) {
                            
                            ofParameter<ofColor> target;
                            target.fromString(msg.getArgAsString(0));
                            fadeManager->add(new ParameterFade<ofColor>(p, target.get(), fadeTime, easeFn));
                        
                        } else if(p->type()==typeid(ofParameter<ofFloatColor>).name()) {
                                
                                ofParameter<ofFloatColor> target;
                                target.fromString(msg.getArgAsString(0));
                                fadeManager->add(new ParameterFade<ofFloatColor>(p, target.get(), fadeTime, easeFn));
                            
                        } else if(p->type()==typeid(ofParameter<ofVec3f>).name()) {
                            
                            ofParameter<ofVec3f> target;
                            target.fromString(msg.getArgAsString(0));
                            fadeManager->add(new ParameterFade<ofVec3f>(p, target.get(), fadeTime, easeFn));
                            
                        } else if(p->type()==typeid(ofParameter<ofVec2f>).name()) {
                            
                            ofParameter<ofVec2f> target;
                            target.fromString(msg.getArgAsString(0));
                            fadeManager->add(new ParameterFade<ofVec2f>(p, target.get(), fadeTime, easeFn));
                        }
                        
                        // Qlab will pass arguments as strings, if there's a following fade string, therefore we catch floats, ints and doubles here as well 
                        
                        else if(p->type()==typeid(ofParameter<float>).name()) {
                            
                            ofParameter<float> target;
                            target.fromString(msg.getArgAsString(0));
                            fadeManager->add(new ParameterFade<float>(p, target.get(), fadeTime, easeFn));
                            
                        } else if(p->type()==typeid(ofParameter<double>).name()) {
                            
                            ofParameter<double> target;
                            target.fromString(msg.getArgAsString(0));
                            fadeManager->add(new ParameterFade<double>(p, target.get(), fadeTime, easeFn));
                        
                        } else if(p->type()==typeid(ofParameter<int>).name()) {
                            
                            ofParameter<int> target;
                            target.fromString(msg.getArgAsString(0));
                            fadeManager->add(new ParameterFade<int>(p, target.get(), fadeTime, easeFn));
                        }
                        
                        
                    } else {
                        p->fromString(msg.getArgAsString(0));
                    }

                }
            }
        }
    }
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    qlab.update();
    
    for( auto s : scenes) {
        s->updateScene();
    }
    
    fadeManager->update();
    
    while(oscReceiver.hasWaitingMessages()) {
        
        ofxOscMessage msg;
        oscReceiver.getNextMessage(msg);
        
        receiveOscParameter(msg, &globalParams);
        receiveOscParameter(msg, &sceneParams);
        
        // custom osc hooks here
    }
    
#ifdef WEBPARAMS
    
    //webUi
    if(eInitRequest){
        eInitRequest = false;
        jsonString = paramSync.parseParamsToJson();
        ofLogVerbose("ofDatGuiApp::update") << "parsed json string:" << jsonString;
        server.send(jsonString);
    }
    
    if(onUpdate){
        paramSync.updateParamFromJson(paramUpdate);
        onUpdate = false;
    }
#endif /* WEBPARAMS */

    if(gui->getDropdown("Model View")->getSelected()->getLabel() == "CAMERA MODEL VIEW"){
        worldModelCam.setPosition(world.physical_camera_pos_cm);
        worldModelCam.lookAt(ofVec3f(0,0,0));
        worldModelCam.setNearClip(20);
    }
    
    updateStage();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
    ofEnableDepthTest();
    
    world.swap.set(swap_left_right);
    
    if(calibrate_projector){
        world.renderProjectorCalibrations();
    } else if (calibrate_camera){
        world.renderCameraCalibrations(stage_size_cm.get());
    } else {
        // render scenes
        ofEnableLighting();
        
        ofFloatColor background_color_alpha_as_brightness(background_color.get());
        background_color_alpha_as_brightness.r *= background_color_alpha_as_brightness.a;
        background_color_alpha_as_brightness.g *= background_color_alpha_as_brightness.a;
        background_color_alpha_as_brightness.b *= background_color_alpha_as_brightness.a;
        background_color_alpha_as_brightness.a = 1.0;
        
        for(std::pair<string, shared_ptr<ofxStereoscopy::Plane>> p : world.planes){
            
            p.second->beginLeft();
            ofClear(background_color_alpha_as_brightness);
            
            for(auto s : scenes) {
                s->drawScene();
            }
            
            p.second->endLeft();
            
            p.second->beginRight();
            ofClear(background_color_alpha_as_brightness);
            
            for(auto s : scenes) {
                s->drawScene();
            }
            
            p.second->endRight();
        }
        
        ofDisableLighting();
    }
    
    
    ofDisableDepthTest();
    
    ofPushMatrix();
    
    if(show_model_on_second_screen){
        ofBackground(12,255);
        
        ofCamera cam(worldModelCam);

        cam.setPosition(world.physical_camera_pos_cm.get() + ofVec3f(-world.physical_eye_seperation_cm*0.5, 0 , 0));
        
        cam.lookAt(world.physical_camera_pos_cm.get().getLimited(stage_size_cm->z));
        cam.setNearClip(20);

        ofPushMatrix();
        ofPushView();
        ofViewport(ofRectangle(0.0, 0.0, ofGetWidth()/2, ofGetHeight()));

        cam.setAspectRatio(ofGetViewportWidth()*2.0/ofGetViewportHeight());
        cam.setForceAspectRatio(true);

        cam.begin();
        //ofTranslate(0,0,-stage_size_cm.get().z);
        //ofScale(2.0, 2.0, 2.0);
        world.drawModel(false, false, false, false);
        cam.end();
        ofPopView();
        ofPopMatrix();

        cam.setPosition(world.physical_camera_pos_cm.get() + ofVec3f(world.physical_eye_seperation_cm*0.5, 0 , 0));
        cam.lookAt(world.physical_camera_pos_cm.get().getLimited(stage_size_cm->z));
        cam.setNearClip(20);
        
        ofPushMatrix();
        ofPushView();
        ofViewport(ofRectangle(ofGetWidth()/2, 0.0, ofGetWidth()/2, ofGetHeight()));

        cam.setAspectRatio(ofGetViewportWidth()*2.0/ofGetViewportHeight());
        cam.setForceAspectRatio(true);

        cam.begin();
        //ofTranslate(0,0,-stage_size_cm.get().z);
        //ofScale(2.0, 2.0, 2.0);
        world.drawModel(false, true, false, false);
        cam.end();
        ofPopView();
        ofPopMatrix();
        cam.setForceAspectRatio(false);
        
    } else {
        
        ofScale(ofGetWidth()*0.5, ofGetHeight(), ofGetHeight());
        
        for(std::pair<string, shared_ptr<ofxStereoscopy::Plane>> p : world.planes){
            ofSetColor(255,255);
            p.second->drawLeft();
        }
        
        ofTranslate(1.0, 0);
        if((calibrate_projector && world.calibrator.rightEye) || !calibrate_projector){
            for(std::pair<string, shared_ptr<ofxStereoscopy::Plane>> p : world.planes){
                ofSetColor(255,255);
                p.second->drawRight();
            }
        }
        
    }
    ofPopMatrix();
}

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
    world.calibrator.updateOutputAspect(w*0.5/h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

#ifdef WEBPARAMS

//--------------------------------------------------------------
void ofApp::launchBrowser(){
        string url = "http";
        if ( server.usingSSL() ){
            url += "s";
        }
        url += "://localhost:" + ofToString( server.getPort() );
        ofLaunchBrowser(url);
}

//--------------------------------------------------------------
void ofApp::onMessage( ofxLibwebsockets::Event& args ){
    // trace out string messages or JSON messages!
    if ( !args.json.isNull() ){
        ofLogNotice("ofDatGuiApp::onMessage") << "json message: " << args.json.toStyledString() << " from " << args.conn.getClientName();
        
        if(args.json["type"]=="initRequest"){
            eInitRequest = true;
        }else if(!onUpdate){
            paramUpdate = args.json;
            onUpdate = true;
        }
    }
}

#endif /* WEBPARAMS */

//--------------------------------------------------------------
#pragma mark GUI

void ofApp::setupGui(shared_ptr<ofAppBaseWindow> gW,shared_ptr<ofAppBaseWindow> mW) {
    
    guiWindow = gW;
    mainWindow = mW;
    if (guiWindow->getWidth() == ofGetScreenWidth()) {
        // hack to go fullscreen on two external monitors
        mW->setWindowPosition(guiWindow->getWidth(), 0);
    }
    
    worldModelCam.setFov(75);

    // Dat Gui
    
    gui = new ofxDatGui( ofxDatGuiAnchor::TOP_LEFT );
    //gui->setTheme(new ofxDatGuiThemeWireframe());
    
    gui->addFRM(1.0f);
    gui->addBreak();
    
    // Stage size
    
    guiBindings.push_back(make_shared<SlidersVec3f>(stage_size_cm, gui));
    ofxDatGuiSlider * resolutionSlider = gui->addSlider(world.pixels_cm);
    resolutionSlider->onSliderEvent(this,&ofApp::onSliderEvent);
    gui->addBreak();
    
    // Model
    
    vector<string> views = {"Perspective Model View", "Camera Model View"};
    ofxDatGuiDropdown * viewDropdown = gui->addDropdown("Model View", views);
    viewDropdown->select(0);
    viewDropdown->onDropdownEvent(this, &ofApp::onDropdownEvent);
    gui->addBreak();
    
    // Camera
    
    /*
    ofxDatGuiFolder * cameraCalibrationFolder = gui->addFolder("Camera Calibration");
    cameraCalibrationFolder->onFolderEvent(this, &ofApp::onFolderEvent);
    
    cameraCalibrationFolder->addSlider("POSITION X", world.physical_camera_pos_cm.getMin().x, world.physical_camera_pos_cm.getMax().x);
    cameraCalibrationFolder->addSlider("POSITION Y", world.physical_camera_pos_cm.getMin().y, world.physical_camera_pos_cm.getMax().y);
    cameraCalibrationFolder->addSlider("POSITION Z", world.physical_camera_pos_cm.getMin().z, world.physical_camera_pos_cm.getMax().z);
     */
    
    gui->addToggle(calibrate_camera);
    guiBindings.push_back(make_shared<SlidersVec3f>(world.physical_camera_pos_cm, gui));
    gui->addSlider(world.physical_eye_seperation_cm);
    gui->addBreak();

    // Projectors
    
    ofxDatGuiFolder * projectorCalibrationFolder = gui->addFolder("Projector Calibration");
    projectorCalibrationFolder->onFolderEvent(this, &ofApp::onFolderEvent);
    gui->addToggle(show_model_on_second_screen);
    
    gui->addToggle(swap_left_right);
    
    gui->addBreak();

    // General

    guiBindings.push_back(make_shared<ColorPickerWithAlpha>(background_color, gui));
    gui->addBreak();

    // Settings
    
    ofxDatGuiButton * saveButton = gui->addButton("save settings");
    saveButton->onButtonEvent(this,&ofApp::onButtonEvent);
    ofxDatGuiButton * loadButton = gui->addButton("load settings");
    loadButton->onButtonEvent(this,&ofApp::onButtonEvent);
    
#ifdef WEBPARAMS
    ofxDatGuiButton * launchBrowserButton = gui->addButton("web interface");
    
    launchBrowserButton->onButtonEvent(this,&ofApp::onButtonEvent);
#endif /* WEBPARAMS */

    // adding the optional header allows you to drag the gui around //
    gui->addHeader(":: World Calibration ::");
    gui->addFooter()->setLabelWhenCollapsed("Expand: World Calibration");

    ofSetFrameRate(60);
    
    oscReceiver.setup(9999);
    
    int sI = 1;
    for( auto & s : scenes) {
        
        ofxPanel * p = new ofxPanel();
        p->setup(s->params);
        p->setPosition(gW->getWidth()-(p->getWidth()*sI), gW->getHeight()-60);
        
        scenePanels.push_back(p);
        sI++;
    }
    
    
}

void ofApp::drawGui(ofEventArgs &args) {
    
    if(ofGetFrameNum() == 2){
        worldModelCam.setPosition(ofVec3f(700, 600, 1500));
        worldModelCam.lookAt(ofVec3f(0,0,0));
        worldModelCam.setNearClip(20);
        worldModelCam.setFarClip(WORLD_DIMENSION_MAX);
        worldModelCam.disableMouseInput();
    }
    
    ofBackgroundGradient(ofColor(20), ofColor(60));
    
    // we draw stereography world here
    
    ofEnableDepthTest();

    if(calibrate_projector){
        
        ofxDatGuiFolder * projectorCalibrationFolder = gui->getFolder("Projector Calibration");
        
        world.calibrator.clearPlane();

        for(std::pair<string, shared_ptr<ofxStereoscopy::Plane>> p : world.planes){
            
            if (((ofxDatGuiToggle *)projectorCalibrationFolder->getComponent(ofxDatGuiType::TOGGLE, p.first + " LEFT"))->ofxDatGuiToggle::getEnabled()) {
                world.calibrator.setPlane(p.second, false);
            } else if (((ofxDatGuiToggle *)projectorCalibrationFolder->getComponent(ofxDatGuiType::TOGGLE, p.first + " RIGHT"))->ofxDatGuiToggle::getEnabled()) {
                world.calibrator.setPlane(p.second, true);
            }
        }
        
        calibrationCamera.setPosition(0.5, 0.5/world.calibrator.outputAspect, -1);
        calibrationCamera.lookAt(ofVec3f(0.5, 0.5/world.calibrator.outputAspect, 0), ofVec3f(0,-1,0));
        calibrationCamera.setNearClip(0.1);
        calibrationCamera.begin();
        world.calibrator.draw();
        calibrationCamera.end();
    } else {
        worldModelCam.begin();
        ofEnableDepthTest();
        world.drawModel(!(gui->getDropdown("Model View")->getSelected()->getLabel() == "CAMERA MODEL VIEW"));
        ofEnableDepthTest();
        for(auto s : scenes) {
            if(s->enabled) {
                s->drawModel();
            }   
        }
        worldModelCam.end();
    }
    ofDisableDepthTest();

    gui->draw();
    
    for( auto s : scenes) {
        //s->drawGui();
    }
    
    for( auto p : scenePanels) {
        ofPushMatrix();ofPushStyle();ofPushView();
        p->draw();
        ofPopMatrix();ofPopStyle();ofPopView();
    }
}

ofVec3f ofApp::calibrationCameraToScreen(ofVec3f v){
    v.y/=world.calibrator.outputAspect;
    return calibrationCamera.worldToScreen(v);
}

ofVec3f ofApp::screenToCalibrator(ofVec2f v){
    ofVec3f zeroZvec(0.0,0.0,0.0);
    ofVec3f zeroZvecOnScreen = calibrationCameraToScreen(zeroZvec);
    return screenToCalibrationCamera(ofVec3f(v.x, v.y, zeroZvecOnScreen.z));
}

ofVec3f ofApp::screenToCalibrationCamera(ofVec3f v){
    v = calibrationCamera.screenToWorld(v);
    v.y*=world.calibrator.outputAspect;
    return v;
}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e){
    
    if(e.target->getLabel() == "SAVE SETTINGS") {
        
        saveAllParameters();
        
    } else if(e.target->getLabel() == "LOAD SETTINGS") {
        loadAllParameters();
    }
#ifdef WEBPARAMS
    else if (e.target->getLabel() == "WEB INTERFACE") {
        launchBrowser();
    }
#endif /* WEBPARAMS */
    
    ofxDatGuiFolder * projectorCalibrationFolder = gui->getFolder("Projector Calibration");

    for (auto child : projectorCalibrationFolder->children) {
        if (child == e.target){
            for (auto otherChild : projectorCalibrationFolder->children) {
                if(otherChild != child){
                    ((ofxDatGuiToggle *) otherChild)->setEnabled(false);
                }
            }
            break;
        }
    }
}

void ofApp::onFolderEvent(ofxDatGuiFolderEvent e){
    if(e.target->getLabel() == "PROJECTOR CALIBRATION"){
        calibrate_projector.set(e.expanded);
    }
}

void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e){
    if(e.target->getLabel() == "PERSPECTIVE MODEL VIEW"){
        worldModelCam.setPosition(ofVec3f(700, 600, 1500));
        worldModelCam.lookAt(ofVec3f(0,0,0));
        worldModelCam.setNearClip(20);
    }
    if(e.target->getLabel() == "CAMERA MODEL VIEW"){
        worldModelCam.setPosition(world.physical_camera_pos_cm);
        worldModelCam.lookAt(ofVec3f(0,0,0));
        worldModelCam.setNearClip(20);
    }
    gui->getDropdown("MODEL VIEW")->collapse();
}

void ofApp::onSliderEvent(ofxDatGuiSliderEvent e){
    if(e.target->getLabel() == "PIXELSCM"){
        flagStageResized = true;
    }
}

void ofApp::stageResized(ofVec3f& v){
    flagStageResized = true;
}

void ofApp::updateStage(){
    if(flagStageResized){
    world.getPlane("WALL")->ofPlanePrimitive::setHeight(stage_size_cm->y);
    world.getPlane("WALL")->ofPlanePrimitive::setWidth(stage_size_cm->x);
    world.getPlane("WALL")->ofPlanePrimitive::setGlobalPosition(0, stage_size_cm->y/2.0, 0);
    world.getPlane("WALL")->dimensionsChanged();
        world.getPlane("FLOOR")->ofPlanePrimitive::setHeight(stage_size_cm->z);
        world.getPlane("FLOOR")->ofPlanePrimitive::setWidth(stage_size_cm->x);
        world.getPlane("FLOOR")->ofPlanePrimitive::setGlobalPosition(0, 0, stage_size_cm->z/2.0);
        world.getPlane("FLOOR")->dimensionsChanged();
    flagStageResized = false;
    }
}


void ofApp::keyPressedGui(int key){
    if (key == 'c') {
        if(lastChangedParam != nullptr) {
            ofGetWindowPtr()->setClipboardString(qlab.findOscAddress(*lastChangedParam));
        }
    }
    if (key == 'q' || key == 'Q') {
        
        if(lastChangedParam != nullptr) {
            
            if(key == 'Q'){
                qlab.newOscCueFromParameter(*lastChangedParam, 2.0);
            } else {
                qlab.newOscCueFromParameter(*lastChangedParam);
            }
        }
    }
    if (key == ' ') {
        worldModelCam.enableMouseInput();
    }
    
    if(calibrate_projector){
        world.calibrator.keyPressed(key);
    }
}

void ofApp::keyReleasedGui(int key){
    if(calibrate_projector){
        world.calibrator.keyReleased(key);
    }
    if (key == ' ') {
        worldModelCam.disableMouseInput();
    }
}

void ofApp::mouseMovedGui(int x, int y ){
    if(calibrate_projector){
        world.calibrator.mouseMoved(screenToCalibrator(ofVec3f(x,y,0)));
    }
}
void ofApp::mouseDraggedGui(int x, int y, int button){
    if(calibrate_projector){
        world.calibrator.mouseDragged(screenToCalibrator(ofVec3f(x,y,0)), button);
    }
}
void ofApp::mousePressedGui(int x, int y, int button){
    if(calibrate_projector){
        world.calibrator.mousePressed(screenToCalibrator(ofVec3f(x,y,0)), button);
    }
}
void ofApp::mouseReleasedGui(int x, int y, int button){
    if(calibrate_projector){
        world.calibrator.mouseReleased(screenToCalibrator(ofVec3f(x,y,0)), button);
    }
}
void ofApp::mouseEnteredGui(int x, int y){
}

void ofApp::mouseExitedGui(int x, int y){
}

void ofApp::mouseScrolledGui(int x, int y, int scrollX, int scrollY){

}

void ofApp::windowResizedGui(int w, int h){

}
void ofApp::dragEventGui(ofDragInfo dragInfo){

}
void ofApp::gotMessageGui(ofMessage msg){

}

// TODO check if it worked
// it fails silently on write protected file
void ofApp::saveAllParameters() {
    saveParameters(globalParams);
    for( auto s : scenes) {
        saveParameters(s->params);
    }

}

void ofApp::loadAllParameters() {
    loadParameters(globalParams);
    for( auto s : scenes) {
        loadParameters(s->params);
    }
}

// save parameters in parameter group
void ofApp::saveParameters(ofParameterGroup & params) {
    ofxPanel panel(params);
    panel.saveToFile(params.getName() + ".xml");
}

// load parameters in parameter group
void ofApp::loadParameters(ofParameterGroup & params) {
    ofxPanel panel(params);
    panel.loadFromFile(params.getName() + ".xml");
}