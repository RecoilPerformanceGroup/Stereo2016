//
//  SketchScene.cpp
//  ofStereo2016
//
//  Created by Ole Kristensen on 10/03/2016.
//
//

#include "SketchScene.hpp"


void SketchScene::setup(){

    
    wideLines.setupShaderFromSource(GL_VERTEX_SHADER,vertexShader);
    wideLines.setupShaderFromSource(GL_GEOMETRY_SHADER,geometryShader);
    wideLines.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentShader);
    wideLines.setGeometryInputType(GL_LINES); // type: GL_POINTS, GL_LINES, GL_LINES_ADJACENCY_EXT, GL_TRIANGLES, GL_TRIANGLES_ADJACENCY_EXT
    wideLines.setGeometryOutputType(GL_TRIANGLE_STRIP); // type: GL_POINTS, GL_LINE_STRIP or GL_TRIANGLE_STRIP
    wideLines.setGeometryOutputCount(6); // set number of output vertices
    
    wideLines.bindDefaults();
    wideLines.linkProgram();

    spline3DCubic.verbose = true;
    
    spline3DCubic.setInterpolation(msa::kInterpolationCubic);
    spline3DCubic.setUseLength(true);

    colors = new ofFloatColor[1];
    
    posFilter.setFc(0.05);
    
    ofSeedRandom(23.03);
    shards.setupFromBoundingBox(1.0, 1.0, 1.0, 5, true, true, true);
    shards.setParent(world->origin);
    
    int i = 0;
    for (auto vn : shards.getChildren()){
        if(i == 0)
            shardNode = vn;
        else
            vn->bDraw = false;
        i++;
    }

}

void SketchScene::update(){
    
    ofVec3f inputPos = globalParams->getVec3f("input3d").get();
    
    if(lineUse3dInput){
        lineNextPos.set(inputPos);
    }
    
    if (lineAddPos) {
        spline3DCubic.push_back(lineNextPos);
        lineAddPos = false;
    }
    
    if (lineAddRandomPos) {
        randomIter += 30.0;
        spline3DCubic.push_back(rotationCenter.get() + ofVec3f(ofSignedNoise(0.3+randomIter)*0.4, ofSignedNoise(2.723+randomIter)*0.2, ofSignedNoise(4.7732+randomIter)*0.4));
        lineAddRandomPos = false;
    }
    
    if (lineClear) {
        randomIter = 0.0;
        spline3DCubic.clear();
        spline3DCubic.setInterpolation(msa::kInterpolationCubic);
        colors = new ofFloatColor[1];
        lineClear = false;
    }
    
    if (shardThrow) {
        int i = 0;
        for (auto vn : shards.getChildren()){
            if(!vn->bDraw && vn != shardNode){
                vn->setGlobalPosition(shardThrowFrom.get()*getWorldSize());
                vn->velocity = shardThrowVelocity;
                vn->bDraw = true;
                shardThrow = false;
                break;
            }
            i++;
        }
    }
    
    for (auto vn : shards.getChildren()){
        vn->setScale(2*shardSize);
        if(vn->bDraw && vn != shardNode){
            vn->setGlobalPosition(vn->getGlobalPosition()+vn->velocity*ofGetLastFrameTime());
            if(vn->getGlobalPosition().length() > 3000){
                vn->bDraw = false;
            }
        }
    }

    ofMatrix4x4 m;
    m.translate(-rotationCenter.get()*getWorldSize());
    m.rotate(rotationEuler->x, 1, 0, 0);
    m.rotate(rotationEuler->y, 0, 1, 0);
    m.rotate(rotationEuler->z, 0, 0, 1);
    m.translate(rotationCenter.get()*getWorldSize());

    verticesCubic.clear();
    
    if(spline3DCubic.size() > 2){
        
        float spacing = abs(lineStart-lineEnd)*1.0/lineResolution;
        spacing = ofClamp(spacing, 0.0001, 100000.0);
        for(float t=lineStart; t < lineEnd.get(); t += spacing){
            ofVec3f vSample(spline3DCubic.sampleAt(t));
            ofVec3f vWorld(vSample*getWorldSize());
            if(lineZinCam) vWorld = world->zInCam(vWorld);
            verticesCubic.push_back((vWorld+ofVec3f(ofSignedNoise((vSample.x+ofGetElapsedTimef())*lineNoisePhase)*lineNoiseAmplitude, ofSignedNoise((vSample.y+ofGetElapsedTimef())*lineNoisePhase)*lineNoiseAmplitude, 0.0))*m);
        }

        ofVec3f shardWorldPosition = spline3DCubic.sampleAt(shardPos)*getWorldSize();
        if(lineZinCam) shardWorldPosition = world->zInCam(shardWorldPosition);

        
        shardNode->setGlobalPosition(shardWorldPosition*m);
        
    }
    
    vbo.setVertexData(verticesCubic.data(), verticesCubic.size(), GL_DYNAMIC_DRAW);
    
    delete colors;
    colors = new ofFloatColor[verticesCubic.size()];
    
    for(int i = 0; i < verticesCubic.size(); i++){
        float brightness = ofMap(verticesCubic[i].z, lineDepthFade, getWorldSize().z, 0.0, 1.0);
        colors[i].setHsb(1.0, 0.0, brightness);
    }
    
    shardMat.setDiffuseColor(shardColor.get());
    shardMat.updateParameters();
    
}

void SketchScene::draw(){
    
    ofSetColor(255,255);
    drawLine();
    if(lineShowDots){
        for(int i = 0; i < spline3DCubic.size(); i++){
            ofVec3f pos(world->zInCam(spline3DCubic.at(i)*getWorldSize()));
            ofSetColor(255,255,0,200);
            ofDrawSphere(pos, 10);
        }
    }
    
}

void SketchScene::drawLine() {
    
    wideLines.begin();
    wideLines.setUniform2f("_line_width", lineWidth,lineWidth*1.5);
    wideLines.setUniform4f("_line_color", 1.0, 1.0, 0.0, 1.0);
    wideLines.setUniform4f("_viewport", ofGetCurrentViewport().x,ofGetCurrentViewport().y, ofGetViewportWidth(), ofGetViewportHeight());

    vbo.setColorData(colors, verticesCubic.size(), GL_DYNAMIC_DRAW);
    vbo.draw(GL_LINE_STRIP, 0, verticesCubic.size());
    
    wideLines.end();
    
    shardMat.begin();
    shards.draw();
    shardMat.end();
    
}

void SketchScene::drawModel(){
    ofEnableDepthTest();
    drawLine();
    ofDisableDepthTest();
    ofSetColor(255,255,0, 200);
    ofDrawSphere(lineNextPos.get()*getWorldSize(), 10);
    for(int i = 0; i < spline3DCubic.size(); i++){
        ofVec3f pos(world->zInCam(spline3DCubic.at(i)*getWorldSize()));
        ofSetColor(255,255,0,200);
        ofDrawSphere(pos, 20);
        ofSetColor(0, 255, 0,200);
        ofDrawBitmapString(ofToString(i), pos.x, pos.y, pos.z);
    }
    /* an incomplete attempt at getting absolute positions relative to segments instead of positions normalised along the length of the spline.
    if(spline3DCubic.size() > 2){
        float absPosClamped = ofClamp(shardPosAbs, 0, spline3DCubic.size()-1);
        if(absPosClamped < spline3DCubic.size() ){
        float distanceAtAbsPos = spline3DCubic.getLength(floor(absPosClamped));
        float distanceAtNextAbsPos = spline3DCubic.getLength(floor(absPosClamped)
                                                         +1);
    
        float tAbs = ofMap(fmodf(shardPosAbs, 1.0), 0.0, 1.0, distanceAtAbsPos, distanceAtNextAbsPos) / spline3DCubic.getLength();
    
    ofVec3f absPos(spline3DCubic.sampleAt(tAbs) * getWorldSize());
    ofSetColor(255, 0, 255,200);
    ofDrawSphere(absPos, 5);
    }
     */
    
}

string SketchScene::vertexShader = R"(
//  wide_line.vs

#version 330

in vec4 position;
in vec4 normal;
in vec2 texcoord;
in vec4 color;

// these are passed in from OF programmable renderer
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat4 normalMatrix;

out vec4 vs_color;

void main()
{
    gl_Position = modelViewProjectionMatrix * position;
    vs_color    = color;
}

)";

string SketchScene::geometryShader = R"(
//  wide_line.gs

//  Geometry shader for rendering wide lines
//  with GL 3.3+ with forward compatible context.
//
//  Input and output positions must be in clip space.
//
//  Not optimized.
//
//  Timo Suoranta - tksuoran@gmail.com

#version 330

#define SHOW_DEBUG_LINES        0
#define PASSTHROUGH_BASIC_LINES 0
#define STRIP                   1

layout(lines) in;

#if SHOW_DEBUG_LINES || PASSTHROUGH_BASIC_LINES
layout(line_strip, max_vertices = 20) out;
#else
layout(triangle_strip, max_vertices = 6) out;
#endif

uniform vec4 _viewport;
uniform vec2 _line_width;

in vec4 position[];
in vec4 vs_color[];

out vec2    v_start;
out vec2    v_line;
out vec4    v_color;
out float   v_l2;

void main(void)
{
    
    //  a - - - - - - - - - - - - - - - - b
    //  |      |                   |      |
    //  |      |                   |      |
    //  |      |                   |      |
    //  | - - -start - - - - - - end- - - |
    //  |      |                   |      |
    //  |      |                   |      |
    //  |      |                   |      |
    //  d - - - - - - - - - - - - - - - - c
    
    vec4 start  = gl_in[0].gl_Position;
    vec4 end    = gl_in[1].gl_Position;
    
#if PASSTHROUGH_BASIC_LINES
    gl_Position = start; v_color = vs_color[0]; EmitVertex();
    gl_Position = end;   v_color = vs_color[1]; EmitVertex();
    EndPrimitive();
    return;
#endif
    
    // It is necessary to manually clip the line before homogenization.
    // Compute line start and end distances to nearplane in clipspace
    // Distances are t0 = dot(start, plane) and t1 = dot(end, plane)
    // If signs are not equal then clip
    float t0 = start.z + start.w;
    float t1 = end.z + end.w;
    if(t0 < 0.0)
    {
        if(t1 < 0.0)
        {
            return;
        }
        start = mix(start, end, (0 - t0) / (t1 - t0));
    }
    if(t1 < 0.0)
    {
        end = mix(start, end, (0 - t0) / (t1 - t0));
    }
    
    vec2 vpSize         = _viewport.zw;
    
    //  Compute line axis and side vector in screen space
    vec3 startInNDC     = start.xyz / start.w;       //  clip to NDC: homogenize and drop z
    vec3 endInNDC       = end.xyz / end.w;
    vec3 lineInNDC      = endInNDC - startInNDC;
    vec2 startInScreen  = (0.5 * startInNDC.xy + vec2(0.5)) * vpSize + _viewport.xy;
    vec2 endInScreen    = (0.5 * endInNDC.xy   + vec2(0.5)) * vpSize + _viewport.xy;
    vec2 lineInScreen   = lineInNDC.xy * vpSize;       //  ndc to screen (direction vector)
    vec2 axisInScreen   = normalize(lineInScreen);
    vec2 sideInScreen   = vec2(-axisInScreen.y, axisInScreen.x);    // rotate
    vec2 axisInNDC      = axisInScreen / vpSize;                    // screen to NDC
    vec2 sideInNDC      = sideInScreen / vpSize;
    vec4 axis           = vec4(axisInNDC, 0.0, 0.0) * _line_width.x;  // NDC to clip (delta vector)
    vec4 side           = vec4(sideInNDC, 0.0, 0.0) * _line_width.x;

    /*
    vec4 a = (start + (side - axis) * start.w);
    vec4 b = (end   + (side + axis) * end.w);
    vec4 c = (end   - (side - axis) * end.w);
    vec4 d = (start - (side + axis) * start.w);
    */

    vec4 a = (start + (side - axis) * start.w);
    vec4 b = (end   + (side + axis) * end.w);
    vec4 c = (end   - (side - axis) * end.w);
    vec4 d = (start - (side + axis) * start.w);

    
    v_start = startInScreen;
    v_line  = endInScreen - startInScreen;
    v_l2    = dot(v_line, v_line);
    
#if SHOW_DEBUG_LINES
    gl_Position = gl_in[0].gl_Position; v_color = vs_color[0]; EmitVertex();
    gl_Position = gl_in[1].gl_Position; v_color = vs_color[1]; EmitVertex();
    EndPrimitive();
    
    gl_Position = a; v_color = vs_color[1]; EmitVertex();
    gl_Position = b; v_color = vs_color[0]; EmitVertex();
    EndPrimitive();
    
    gl_Position = b; v_color = vs_color[1]; EmitVertex();
    gl_Position = c; v_color = vs_color[0]; EmitVertex();
    EndPrimitive();
    
    gl_Position = c; v_color = vs_color[1]; EmitVertex();
    gl_Position = d; v_color = vs_color[0]; EmitVertex();
    EndPrimitive();
    
    gl_Position = d; v_color = vs_color[0]; EmitVertex();
    gl_Position = a; v_color = vs_color[0]; EmitVertex();
    EndPrimitive();

#else
    
#if STRIP
#if 0
    gl_Position = d; v_color = vs_color[0]; EmitVertex();
    gl_Position = a; v_color = vs_color[0]; EmitVertex();
    gl_Position = c; v_color = vs_color[1]; EmitVertex();
    gl_Position = b; v_color = vs_color[1]; EmitVertex();
#else
    gl_Position = a; v_color = vs_color[0]; EmitVertex();
    gl_Position = d; v_color = vs_color[0]; EmitVertex();
    gl_Position = b; v_color = vs_color[1]; EmitVertex();
    gl_Position = c; v_color = vs_color[1]; EmitVertex();
#endif
    EndPrimitive();
#else
    gl_Position = d; v_color = vs_color[0];  EmitVertex();
    gl_Position = a; v_color = vs_color[0];  EmitVertex();
    gl_Position = c; v_color = vs_color[1];  EmitVertex();
    EndPrimitive();
    gl_Position = c; v_color = vs_color[1];  EmitVertex();
    gl_Position = a; v_color = vs_color[0];  EmitVertex();
    gl_Position = b; v_color = vs_color[1];  EmitVertex();
    EndPrimitive();
#endif
    
#endif
}

)";

string SketchScene::fragmentShader = R"(
//  wide_line.fs

//  Fragment shader for rendering wide lines
//  with GL 3.3+ with forward compatible context.
//
//  To be used with line.gs geometry shader.
//
//  Not optimized.
//
//  Timo Suoranta - tksuoran@gmail.com

#version 330

uniform vec4 _line_color;
uniform vec2 _line_width;

in  vec2    v_start;
in  vec2    v_line;
in  vec4    v_color;
in  float   v_l2;
out vec4    out_color;

void main(void)
{
    float   t           = dot(gl_FragCoord.xy - v_start, v_line) / v_l2;
                        // normalised position of this fragment along the line
    vec2    projection  = v_start + clamp(t, 0.0, 1.0) * v_line;
    vec2    delta       = gl_FragCoord.xy - projection;
                        // distance of this fragment from line center
    float   d2          = dot(delta, delta);
                        // length of delta squared
    float   k           = clamp(_line_width.y - d2, 0.0, 1.0);
                        // alpha for width
    float   endWeight   = step(abs(t * 2.0 - 1.0), 1);
                        // alpha for round ends
    float   alpha       = mix(k, 1.0, endWeight);
    
    if(k < 1.0) discard;
    
    //out_color = vec4(_line_color.rgb, 1.0);
    //out_color = vec4(v_color.rgb, k*v_color.a);
    out_color = vec4(v_color.rgb , k*v_color.a);

}

)";
