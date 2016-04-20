//
//  SketchScene.hpp
//  ofStereo2016
//
//  Created by Ole Kristensen on 10/03/2016.
//
//

#pragma once

#include "ofMain.h"
#include "ofxStereoscopy.hpp"
#include "VoroNode.hpp"
#include "ofxGui.h"
#include "ofxBiquadFilter.h"
#include "MSAInterpolator.h"


class SketchScene : public ofxStereoscopy::Scene {

public:

    ofParameter<float> lineStart {"start", 0.0, 0.0, 1.0};
    ofParameter<float> lineEnd {"end", 1.0, 0.0, 1.0};
    ofParameter<int> lineResolution {"resolution", 500, 3, 10000};
    ofParameter<float> lineWidth {"width", 25, 0, 100};
    ofParameter<float> lineDepthFade {"depthFade", 0, -1000, 1000};
    ofParameter<float> lineNoiseAmplitude {"noiseAmplitude", 0.00, 0.0, 100};
    ofParameter<float> lineNoisePhase {"noisePhase", 0.00, 0.0, 100.0};
    ofParameter<bool> lineUse3dInput {"use3dInput", true};
    ofParameter<ofVec3f> lineNextPos {"nextPos", ofVec3f(0, 0, 0),ofVec3f(-2, -2, -2),ofVec3f(2,2,2)};
    ofParameter<bool> lineAddPos {"lineAddPos", true};
    ofParameter<bool> lineClear {"clear", false};
    ofParameter<bool> lineLoad {"load", true};
    ofParameter<bool> lineSave {"save", false};
    
    ofxBiquadFilter3f posFilter;
    

    ofParameterGroup lineParams { "line",
        lineStart,
        lineEnd,
        lineResolution,
        lineWidth,
        lineDepthFade,
        lineNoiseAmplitude,
        lineNoisePhase,
        lineUse3dInput,
        lineNextPos,
        lineAddPos,
        lineClear,
        lineLoad,
        lineSave
    };

    ofParameter<float> shardSize {"size", 20, 0.0, 800};
    ofParameter<float> shardPos {"position", 0, 0.0, 1.0};
    ofParameter<ofFloatColor> shardColor {"color", ofFloatColor(1,1,1,1), ofFloatColor(0,0,0,0), ofFloatColor(1,1,1,1)};
    ofParameter<ofVec3f> shardThrowFrom {"shardThrowFromPos", ofVec3f(0, 0, 0),ofVec3f(-2, -2, -2),ofVec3f(2,2,2)};
    ofParameter<ofVec3f> shardThrowVelocity {"shardThrowVelocity", ofVec3f(0, 0, 0),ofVec3f(-1000, -1000, -1000),ofVec3f(1000, 1000, 1000)};
    ofParameter<bool> shardThrow {"throw", false};

    ofParameterGroup shardParams { "shard",
        shardPos,
        shardSize,
        shardColor,
        shardThrowFrom,
        shardThrowVelocity,
        shardThrow
    };

    ofParameter<ofVec3f> rotationCenter {"rotationCenter", ofVec3f(0, 0, 0),ofVec3f(-2, -2, -2),ofVec3f(2,2,2)};
    ofParameter<ofVec3f> rotationEuler {"rotationEuler", ofVec3f(0, 0, 0),ofVec3f(-180, -180, -180),ofVec3f(180, 180, 180)};

    ofParameterGroup params {"sketch",
        enabled,
        qlab,
        rotationEuler,
        rotationCenter,
        lineParams
    };
    
    SketchScene() {
        ofxStereoscopy::Scene::params = params;
        shardParams.add(shardMat.params);
        params.add(shardParams);
    }

    msa::Interpolator3D	spline3DCubic;
    vector<ofVec3f> verticesCubic;
    msa::Interpolator3D	spline3DLinear;
    vector<ofVec3f> verticesLinear;
    
    ofVbo vbo;
    ofFloatColor * colors;
    void draw();
    void update();
    void setup();
    void drawLine();
    
    ofShader wideLines;

    OrganicMaterial shardMat;
    
    VoroNode shards;
    int nextTrhow = 0;
    
    VoroNode * shardNode;

    void drawModel();
    
private:
    static string vertexShader;
    static string fragmentShader;
    static string geometryShader;
    
    
};
