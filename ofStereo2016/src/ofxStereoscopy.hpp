//
//  ofxStereoscopy.hpp
//  ofStereo2016
//
//  Created by Ole Kristensen on 25/02/2016.
//
//

#ifndef ofxStereoscopy_hpp
#define ofxStereoscopy_hpp

#include "ofMain.h"

namespace ofxStereoscopy {
    
    class Plane;
    
    class World {
        
    public:
        
        static constexpr const float dimensionMax = 10000.0;
        
        ofParameter<float> physical_eye_seperation_cm {"eye separation", 6.5, 0, 10};
        ofParameter<ofVec3f> physical_camera_pos_cm {"camera position", ofVec3f(0,250,-1000), ofVec3f(-dimensionMax,-dimensionMax,-dimensionMax), ofVec3f(dimensionMax,dimensionMax,dimensionMax)};
        ofParameter<float> physical_focus_distance_cm {"focus distance", 200, 0, dimensionMax};
        ofParameter<float> physical_camera_near_clip {"camera near clip", 20, 0, dimensionMax};
        ofParameter<float> pixels_cm {"pixels pr. cm", 100, 0, 1000};
        
        ofParameterGroup params{
            "world",
            physical_camera_pos_cm,
            physical_eye_seperation_cm,
            physical_focus_distance_cm,
            physical_camera_near_clip,
            pixels_cm,
        };
        
        //TODO: Make parameter changes update all planes...
        
        void addPlane(std::string name, float width_cm, float height_cm, const ofVec3f& pos_cm, const ofQuaternion& orientation_q){
            // TODO: Make planes and put them in map
        }
        
        ;
        
        std::map<std::string, shared_ptr<ofxStereoscopy::Plane>> planes;
        
        //TODO: Make drawWorldModel
        //TODO: Make drawPlane
        
    };
    
    class Plane : ofPlanePrimitive{
        
    public:
        
        Plane(std::string name, float width, float height, const ofVec3f& pos_cm, const ofQuaternion& orientation_q){
            
            setName(name);
            ofPlanePrimitive::setWidth(width);
            ofPlanePrimitive::setHeight(height);
            ofPlanePrimitive::setOrientation(orientation_q);
            
            // TODO: bind size parameter to ofPlanePrimitive::width and ofPlanePrimitive::height
            // TODO: bind position parameter to ofPlanePrimitive::position
            // TODO: make a bound orientation parameter to ofPlanePrimitive::orientation quarternion

            dimensionsChanged();
            
        };
        
        ~Plane() {};
        
        void setName(std::string name){
            params.setName(name);
        }
        
        void setWidth(float w) {
            ofPlanePrimitive::setWidth(w);
            dimensionsChanged();
        }
        
        void setHeight(float h) {
            ofPlanePrimitive::setHeight(h);
            dimensionsChanged();
        }
        
        void setOrientation(const ofQuaternion& q) {
            ofPlanePrimitive::setOrientation(q);
        }
        
        void dimensionsChanged(ofRectangle viewport = ofGetCurrentViewport()){
            
            physical_size_cm.set(ofVec2f(width, height));
            
            ofFbo::Settings fboSettings;
            
            fboSettings.numSamples = 8;
            fboSettings.useDepth = true;
            fboSettings.width = floor(pixels_cm * width);
            fboSettings.height = floor(pixels_cm * height);
            
            fboLeft.allocate(fboSettings);
            fboRight.allocate(fboSettings);
            
            camLeft.setScale(width, height, width);
            camLeft.setNearClip(nearClip);
            
            camRight.setScale(width, height, width);
            camRight.setNearClip(nearClip);
            
            lookAt(ofVec3f(0,0,0));

        }
        
        void setupPerspective(){
            camLeft.setupPerspective();
            camRight.setupPerspective();
        }
        
        void lookAt(ofVec3f v){
            camLeft.lookAt(v);
            camRight.lookAt(v);
        }
        
        //TODO: Setup view portals and eye seperation
        /*
        void setupViewPortals(ofRectangle viewport = ofGetCurrentViewport())
        {
            
            float eye = physical_eye_seperation_cm / physical_focus_distance_cm;
            
            right.setPosition(left.getPosition().x + eye, left.getPosition().y, left.getPosition().z);
            
            right.setupOffAxisViewPortal(viewport.getTopLeft(), viewport.getBottomLeft(), viewport.getBottomRight());
            left.setupOffAxisViewPortal(viewport.getTopLeft(), viewport.getBottomLeft(), viewport.getBottomRight());
            
        }
        */
        
        //TODO: Add begin and end calls to enter and exit cameras and FBOs
        
        ofParameter<bool> enabled {true};
        ofParameter<ofVec3f> physical_pos_cm {"camera position", ofVec3f(0,0,0), ofVec3f(-World::dimensionMax,-World::dimensionMax,-World::dimensionMax), ofVec3f(World::dimensionMax,World::dimensionMax,World::dimensionMax)};
        ofParameter<ofVec2f> physical_size_cm {"size", ofVec2f(100,100), ofVec2f(0,0), ofVec2f(World::dimensionMax*2,World::dimensionMax*2)};
        ofParameter<float> pixels_cm {"pixels pr. cm", 100, 0, 1000};

        ofParameterGroup params{
            "plane",
            enabled,
            physical_pos_cm,
            physical_size_cm,
            pixels_cm,
        };
        
        ofCamera camLeft, camRight;
        ofFbo fboLeft, fboRight;
        
        float focusDistance;
        float nearClip;
        float eyeSeperation;

    };
    
    class Eyes {
    public:
        
        ofCamera left;
        ofCamera right;
        
        Eyes(): physical_eye_seperation_cm(6.5), physical_focus_distance_cm(200), focus_distance(200) {};
        ~Eyes(){};
        
        void setup(int w, int h)
        {
            height = 2.;
            width = height *(w/h);
            
            ofFbo::Settings settings;
            
            settings.height = h;
            settings.width = w;
            settings.numSamples = 8;
            settings.useDepth = true;
            //settings.colorFormats = GL_RGBA;
            
            leftFbo.allocate(settings);
            rightFbo.allocate(settings);
            
            left.setScale(1, 1, 1);
            left.setNearClip(0.1);
            
            right.setScale(1, 1, 1);
            right.setNearClip(0.1);
        }
        
        void setPosition(float x, float y, float z) {
            setPosition(ofVec3f(x,y,z));
        }
        
        void setPosition(ofVec3f pos) {
            left.setPosition(pos);
            right.setPosition(pos);
        }
        
        void setupPerspective(){
            left.setupPerspective();
            right.setupPerspective();
        }
        
        void lookAt(ofVec3f v){
            left.lookAt(v);
            right.lookAt(v);
        }
        
        void update(ofRectangle viewport = ofGetCurrentViewport())
        {
            
            eye = physical_eye_seperation_cm / physical_focus_distance_cm;
            
            //FIXME: WHAT IS THIS?
            
            /*aspect = viewport.width / viewport.height;
             fovy_tan = tanf(PI * left.getFov() / 360.);
             fovx_tan = fovy_tan * aspect;
             */
            //zNear = left.getNearClip();
            //zFar  = left.getFarClip();
            
            right.setPosition(left.getPosition().x + eye, left.getPosition().y, left.getPosition().z);
            
            right.setupOffAxisViewPortal(viewport.getTopLeft(), viewport.getBottomLeft(), viewport.getBottomRight());
            left.setupOffAxisViewPortal(viewport.getTopLeft(), viewport.getBottomLeft(), viewport.getBottomRight());
            
        }
        
        void setPhysicalEyeSeparation(float cm) { physical_eye_seperation_cm = cm; }
        void setPhysicalFocusDistance(float cm) { physical_focus_distance_cm = cm; }
        void setFocusDistance(float v) { focus_distance = v; }
        
        void beginLeft()
        {
            leftFbo.begin();
            ofFloatColor c = ofGetCurrentRenderer()->getBackgroundColor();
            ofClear(c);
            left.begin();
        }
        
        void endLeft()
        {
            left.end();
            leftFbo.end();
        }
        
        void beginRight()
        {
            rightFbo.begin();
            ofFloatColor c = ofGetCurrentRenderer()->getBackgroundColor();
            ofClear(c);
            right.begin();
        }
        
        void endRight()
        {
            right.end();
            rightFbo.end();
        }
        
        ofFbo leftFbo, rightFbo;
        
    protected:
        
        int width, height;
        
        float eye, focus_distance;
        
        float physical_eye_seperation_cm;
        float physical_focus_distance_cm;
        
        //FIXME: WHAT IS THIS?
        // parallel
        float zNear, zFar;
        float fovx_tan, fovy_tan;
        float aspect;
        
    private:
        
    };
    
    class oldPlane {
        
    public:
        // TODO frustrum - dynamicly calculate
        // TODO viewer world location - inherit from world
        
        //ofxStereoCamera<ofCamera> cam;
        
        oldPlane(string n) {
            name = n;
        };
        
        ~oldPlane() {
        };

        ofxStereoscopy::Eyes cam;
        
        ofRectangle viewport;
        
        int width;
        int height;
        
        int camWidth; // resolution
        int camHeight;
        
        float aspect = 1.0;
        
        ofVec3f camPos;
        
        string name;
        
        ofVec2f pos;
        
        
        //    ofxXmlSettings * settings;
        
        void setup(int w, int h /*, ofxXmlSettings * s*/) {
            
            viewport = ofRectangle(-1, -1, 2, 2);
            
            /*        settings = s;
             
             if( !settings->tagExists(name) ) {
             settings->addTag(name);
             }
             */
            camHeight = 2.;
            camWidth = height *(w/h);
            
            width = w;
            height = h;
            
            cam.setup(w, h);
            
            cam.setPhysicalFocusDistance(120);
            cam.setFocusDistance(50);
            
            
        };
        
        void setViewPort(ofRectangle _viewport) {
            viewport = _viewport;
        };
        
        void drawGrid() {
            
            
            ofFill();
            
            ofDrawGrid(1., 4, true);
            ofDrawGrid(1., 4, true,false,true,false);
            
            ofLight l;
            
            l.setDirectional();
            l.setPosition(ofVec3f(-1,1,-1));
            l.lookAt(ofVec3f(0,0,0));
            
            l.enable();
            
            ofPushMatrix();
            ofTranslate(0,0,1);
            ofEnableLighting();
            ofDrawSphere(0.1);
            ofDisableLighting();
            ofPopMatrix();
            
            ofEnableLighting();
            ofDrawSphere(0.1);
            ofDisableLighting();
            
            ofPushMatrix();
            ofTranslate(0,0,-1);
            ofEnableLighting();
            ofDrawSphere(0.1);
            ofDisableLighting();
            ofPopMatrix();
            
            l.disable();
            
            ofPushMatrix();
            ofTranslate(0,1,0);
            ofDrawGrid(1., 4, true,false,true,false);
            ofPopMatrix();
            
            ofPushMatrix();
            ofTranslate(0,-1,0);
            ofDrawGrid(1., 4, true,false,true,false);
            ofPopMatrix();
            
        }
        
        void drawChessboard() {
            
            
            
            ofPushStyle();
            ofFill();
            ofDisableDepthTest();
            
            float chessSize = 0.05;
            
            ofPushMatrix(); {
                ofScale(1./(aspect*height*1.0/width),1, 1);
                
                bool white = true;
                bool red = true;
                int row = 0;
                
                ofColor col;
                
                ofTranslate(-1, -1);
                for(float x = 0; x < 2; x+=chessSize){
                    
                    row += 1;
                    
                    for(float y = 0; y < 2; y+=chessSize){
                        
                        col.set(200,200,200);
                        
                        if(!white) col.a = 0;
                        
                        ofSetColor(col.r, col.g, col.b, col.a);
                        white = !white;
                        
                        ofDrawRectangle(x, y, chessSize, chessSize);
                    }
                }
            } ofPopMatrix();
            
            
            ofPushMatrix();
            
            ofSetColor(255,255,255);
            ofDrawCircle(0, 0, 0.5, 0.5);
            
            
            ofSetColor(255,255,0);
            
            ofDrawCircle(-0.9, -0.9, 0.04);
            ofDrawCircle(0.9, 0.9, 0.04);
            ofDrawCircle(-0.9, 0.9, 0.04);
            ofDrawCircle(0.9, -0.9, 0.04);
            
            ofDrawCircle(0, 0.9, 0.04);
            ofDrawCircle(0, -0.9, 0.04);
            
            
            /*ofRect(-0.95, -0.95, 1.85, 0.1);
             ofSetColor(0,155,255);
             ofRect(-0.95, -0.1, 1.85, 0.1);
             ofSetColor(255,155,0);
             ofRect(0, -0.95, 0.1, 1.95);*/
            
            //ofDrawAxis(1);
            ofPopMatrix();
            ofPopStyle();
            
        }
        
        
        /*void drawMappingGrid() {
         
         
         ofPushStyle();
         ofFill();
         ofDisableDepthTest();
         
         float chessSize = 0.05;
         
         ofPushMatrix(); {
         ofScale(1./(aspect*height*1.0/width),1, 1);
         
         
         ofRect(-1,-1, 0.2, 2);
         
         
         
         
         } ofPopMatrix();
         
         
         ofPushMatrix();
         
         ofSetColor(255,0,0);
         ofEllipse(0, 0, 1, 1);
         //ofDrawAxis(1);
         ofPopMatrix();
         ofPopStyle();
         
         }
         
         void drawMappingGrids() {
         
         beginLeft();
         drawChessboard();
         endLeft();
         
         beginRight();
         drawChessboard();
         endRight();
         }*/
        
        
        void drawChessboards() {
            
            beginLeft();
            drawChessboard();
            endLeft();
            
            beginRight();
            drawChessboard();
            endRight();
        }
        
        void drawGrids() {
            cam.beginLeft();
            drawGrid();
            cam.endLeft();
            
            cam.beginRight();
            drawGrid();
            cam.endRight();
        }
        
        
        int controlSide = 0; // 0: left, 1: right
        
        void beginLeft(){
            ofPushView();
            glPushMatrix();
            cam.beginLeft();
            ofScale(aspect*height*1.0/width, 1.0, 1.0);
        }
        
        void endLeft(){
            cam.endLeft();
            glPopMatrix();
            ofPopView();
        }
        
        void beginRight(){
            ofPushView();
            glPushMatrix();
            cam.beginRight();
            ofScale(aspect*height*1.0/width, 1.0, 1.0);
        }
        
        void endRight(){
            cam.endRight();
            glPopMatrix();
            ofPopView();
        }
        
        void drawLeft() {
            ofPushMatrix();
            ofTranslate(pos.x, pos.y);
            cam.leftFbo.draw(0,0,width,height);
            ofPopMatrix();
        }
        
        void drawRight() {
            ofPushMatrix();
            ofTranslate(pos.x + width, pos.y);
            cam.rightFbo.draw(0,0,width,height);
            ofPopMatrix();
        }
        
        void draw(){
            ofSetColor(255);
            if(controlSide == 1) {
                drawRight();
                drawLeft();
            } else {
                drawLeft();
                drawRight();
            }
        }
        
        void update() {
            // observerPosition.set(sin(ofGetElapsedTimef()), cos(ofGetElapsedTimef()), -1);
            cam.setPosition(camPos);
            // cam.setFocusDistance( cam.left.getGlobalPosition().length() );
            cam.update(viewport);
        }
        
        void drawInfo() {
            ofDrawBitmapString(name, 10, 10);
        }
        
        /*    void exit() {
         
         ofLog(OF_LOG_NOTICE, "Saving data for plane: " + name);
         
         settings->pushTag(name);
         
         settings->popTag();
         }
         */
        
    private:
        
    };
    
    class Scene {
        
    public:
        
        int index;
        string indexStr;
        string name = "";
        string oscAddress = "/default";
        
        bool enabled;
        float time;
        
        //    ofxTimeline * mainTimeline;
        //    ofxTLCurves * tlenabled;
        
        Scene() {
        };
        
        virtual ~Scene(){}
        
        virtual void debugDraw(int _surfaceId=0) {};
        virtual void exit(){};
        //    virtual void receiveOsc(ofxOscMessage * m, string rest) {};
        
        /*    virtual void setGui(ofxUICanvas * gui, float width){
         gui->addWidgetDown(new ofxUILabel(name, OFX_UI_FONT_SMALL));
         gui->addWidgetDown(new ofxUILabel("OSC Address: " + oscAddress, OFX_UI_FONT_SMALL));
         gui->addSpacer(width, 1);
         //gui->addToggle(indexStr+"Enabled", &enabled);
         }
         
         virtual void guiEvent(ofxUIEventArgs &e) {};
         */
        void setName(string _name) {
            setName(_name, "/"+_name);
        }
        
        void setName(string _name, string _osc) {
            name = _name;
            oscAddress = _osc;
        }
        
        void setupScene(int _index) {
            if(name == "") setName("scene-" + ofToString(_index));
            
            index = _index;
            indexStr = "["+ ofToString(_index) + "] ";
            
            setup();
            
            //        mainTimeline->setCurrentPage("Page One");
            //        tlenabled = mainTimeline->addCurves("Enable " + name);
            
        };
        
        void updateScene() {
            
            //        enabled = (tlenabled->getValue() == 1);
            
            if(enabled) {
                update();
            }
        };
        
        void drawScene(int _surfaceId=0) {
            if(enabled) {
                glPushMatrix();ofPushMatrix();ofPushStyle();
                draw(_surfaceId);
                ofPopStyle();ofPopMatrix();glPopMatrix();
                
                glPushMatrix();ofPushMatrix();ofPushStyle();
                debugDraw(_surfaceId);
                ofPopStyle();ofPopMatrix();glPopMatrix();
            }
        };
        void beginSceneWorld(int _surfaceId=0){
            if(enabled) {
                beginWorld(_surfaceId);
            }
        };
        void endSceneWorld(int _surfaceId=0){
            if(enabled) {
                endWorld(_surfaceId);
            }
        };
        
        void setSurface(int _surfaceId) {
            primarySurface = _surfaceId;
        }
        
        int primarySurface = 0;
        
    private:
        virtual void setup(){};
        virtual void update(){};
        virtual void beginWorld(int _surfaceId=0){};
        virtual void endWorld(int _surfaceId=0){};
        virtual void draw(int _surfaceId=0){};
        
    };
    
}

#endif /* ofxStereoscopy_hpp */
