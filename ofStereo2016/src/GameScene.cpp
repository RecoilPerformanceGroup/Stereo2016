//
//  GameScene.cpp
//  ofStereo2016
//
//  Created by Ole Kristensen on 10/03/2016.
//
//

#include "GameScene.hpp"


void GameScene::setup(){
    
    globalParams->getVec3f("stage_size_cm").addListener(this, &GameScene::onStageSize);
    
    path.setFilled(false);
    path.setStrokeColor(ofColor::white);
    path.setStrokeWidth(1); //otherwise it is not a line, but a shape to of
    path.setMode(ofPath::POLYLINES);
    
    wideLines.setupShaderFromSource(GL_VERTEX_SHADER,vertexShader);
    wideLines.setupShaderFromSource(GL_GEOMETRY_SHADER,geometryShader);
    wideLines.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentShader);
    wideLines.setGeometryInputType(GL_LINES); // type: GL_POINTS, GL_LINES, GL_LINES_ADJACENCY_EXT, GL_TRIANGLES, GL_TRIANGLES_ADJACENCY_EXT
    wideLines.setGeometryOutputType(GL_TRIANGLE_STRIP); // type: GL_POINTS, GL_LINE_STRIP or GL_TRIANGLE_STRIP
    wideLines.setGeometryOutputCount(6);	// set number of output vertices
    
    wideLines.bindDefaults();
    wideLines.linkProgram();
    
    resetLine();
}

void GameScene::update(){
   /* if (reset) {
        resetLine();
        reset = false;
    }
    */
    path.clear();
    path.setCurveResolution(200);
    ofVec3f startFrom = world->physical_camera_pos_cm.get() + lineOffset.get();
    path.moveTo(startFrom);
    path.lineTo(startFrom.getInterpolated(dp(1), length));
    
}

void GameScene::draw(){
    
    ofSetColor(255,255);
    //ofDisableDepthTest();
    drawLine();
    //ofEnableDepthTest();
    ofPushMatrix();
    ofScale(1,0.0,1);
    //ofDrawLine(dp(1), world->physical_camera_pos_cm);
    ofPopMatrix();
    
}

void GameScene::onStageSize(ofVec3f& vec){
    
}

void GameScene::drawLine() {
    /*
    wideLines.begin();
    wideLines.setUniform2f("_line_width", lineWidth.get().x,lineWidth.get().y);
    wideLines.setUniform4f("_line_color", 1.0, 1.0, 1.0, 1.0);
    wideLines.setUniform4f("_viewport", ofGetCurrentViewport().x,ofGetCurrentViewport().y, ofGetViewportWidth(), ofGetViewportHeight());
    
    int pathLength = path.getOutline()[0].size();

    vbo.setVertexData(&path.getOutline()[0].getVertices()[0], pathLength, GL_DYNAMIC_DRAW);
    
    delete colors;
    
    colors = new ofFloatColor[pathLength];
    
    for(int i = 0; i < pathLength; i++){
        colors[i].set(1.0, 1.0, 1.0, 1.0);
    }
    
    vbo.setColorData(colors, pathLength, GL_DYNAMIC_DRAW);
    vbo.draw(GL_LINE_STRIP, 0, pathLength);
    
    wideLines.end();*/
    
    
    ofVec3f startFrom = world->physical_camera_pos_cm.get() + lineOffset.get();

    /*ofPushMatrix();
    ofVec3f center = startFrom.getMiddle(dp(1));
    ofTranslate(center);
    ofRotate()
    ofDrawCylinder(of(), float radius, float height)
    ofPopMatrix();
    */
    
    ofSetColor(255,255);
    
    for (float i = startDistance; i < startFrom.distance(dp(1))*length; i += lineWidth.get().x/2.0) {
        ofDrawEllipse(startFrom.getInterpolated(dp(1), i/startFrom.distance(dp(1))), lineWidth.get().x/2.0, lineWidth.get().x/2.0);
    }
}

void GameScene::resetLine(){
    colors = new ofFloatColor[1];
    path.clear();
}

void GameScene::drawModel(){
    ofEnableDepthTest();
    drawLine();
    path.draw();
    ofDisableDepthTest();
}

string GameScene::vertexShader = R"(
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

string GameScene::geometryShader = R"(
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
    vec2 startInNDC     = start.xy / start.w;       //  clip to NDC: homogenize and drop z
    vec2 endInNDC       = end.xy / end.w;
    vec2 lineInNDC      = endInNDC - startInNDC;
    vec2 startInScreen  = (0.5 * startInNDC + vec2(0.5)) * vpSize + _viewport.xy;
    vec2 endInScreen    = (0.5 * endInNDC   + vec2(0.5)) * vpSize + _viewport.xy;
    vec2 lineInScreen   = lineInNDC * vpSize;       //  ndc to screen (direction vector)
    vec2 axisInScreen   = normalize(lineInScreen);
    vec2 sideInScreen   = vec2(-axisInScreen.y, axisInScreen.x);    // rotate
    vec2 axisInNDC      = axisInScreen / vpSize;                    // screen to NDC
    vec2 sideInNDC      = sideInScreen / vpSize;
    vec4 axis           = vec4(axisInNDC, 0.0, 0.0) * _line_width.x;  // NDC to clip (delta vector)
    vec4 side           = vec4(sideInNDC, 0.0, 0.0) * _line_width.x;
    
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

string GameScene::fragmentShader = R"(
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
    vec2    projection  = v_start + clamp(t, 0.0, 1.0) * v_line;
    vec2    delta       = gl_FragCoord.xy - projection;
    float   d2          = dot(delta, delta);
    float   k           = clamp(_line_width.y - d2, 0.0, 1.0);
    float   endWeight   = step(abs(t * 2.0 - 1.0), 1);
    float   alpha       = mix(k, 1.0, endWeight);
    
    if(k < 0.01) discard;
    
    //out_color = vec4(_line_color.rgb, 1.0);
    out_color = vec4(v_color.rgb, k*v_color.a);
}

)";
