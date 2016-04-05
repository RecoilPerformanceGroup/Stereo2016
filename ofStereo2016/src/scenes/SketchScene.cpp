//
//  SketchScene.cpp
//  ofStereo2016
//
//  Created by Ole Kristensen on 10/03/2016.
//
//

#include "SketchScene.hpp"


void SketchScene::setup(){

    globalParams->getVec3f("stage_size_cm").addListener(this, &SketchScene::onStageSize);
    
    path.setFilled(false);
    path.setStrokeColor(ofColor::white);
    path.setStrokeWidth(1); //otherwise it is not a line, but a shape to of
    path.setMode(ofPath::POLYLINES);
    
    wideLines.setupShaderFromSource(GL_VERTEX_SHADER,vertexShader);
    wideLines.setupShaderFromSource(GL_GEOMETRY_SHADER,geometryShader);
    wideLines.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentShader);
    wideLines.bindDefaults();
    wideLines.linkProgram();

    resetLine();
}

void SketchScene::update(){
    if (reset) {
        resetLine();
        reset = false;
    }
    
    noisePos.x += ofGetLastFrameTime() * speed;
    noisePos.y += ofGetLastFrameTime() * speed;
    noisePos.z += ofGetLastFrameTime() * speed;

    pivotNoisePos.x += ofGetLastFrameTime() * pivotSpeed / pivotRadius;
    pivotNoisePos.y += ofGetLastFrameTime() * pivotSpeed / pivotRadius;
    pivotNoisePos.z += ofGetLastFrameTime() * pivotSpeed / pivotRadius;

    
    ofVec3f currentPoint(ofSignedNoise(noisePos.x),
                         ofSignedNoise(noisePos.y),
                         ofSignedNoise(noisePos.z));
    
    ofVec3f currentPivot(ofSignedNoise(pivotNoisePos.x),
                         ofSignedNoise(pivotNoisePos.y),
                         ofSignedNoise(pivotNoisePos.z));
    
    currentPoint*=space;
    currentPoint+=origin;
    currentPoint.interpolate(world->physical_camera_pos_cm, positionTowardsCamera);
    points.push_back((currentPoint)+(currentPivot*pivotRadius));
    
    path.rotate(0.025, ofVec3f(0.5,1,0.33));

    if(points.size()%3==0){
        ofVec3f & c1 = points[points.size()-3];
        ofVec3f & c2 = points[points.size()-2];
        ofVec3f & p = points[points.size()-1];
        path.quadBezierTo(c1, c2, p);
    }
    path.simplify();
    
    
}

void SketchScene::draw(){
    
    ofSetColor(255,255);
    path.draw();
    
    
    if(points.size() > 2){
        mat.begin();
        ofDrawSphere(points[points.size()-1], radius);
        mat.end();
        ofPushMatrix();
        ofTranslate(points[points.size()-1]*ofVec3f(1,0,1));
        ofRotate(90, 1,0,0);
        ofDrawEllipse(0,0,radius, radius);
        ofPopMatrix();
        ofPushMatrix();
        ofScale(1,0.0,1);
        path.draw();
        ofPopMatrix();
    }
    
}

void SketchScene::onStageSize(ofVec3f& vec){

}

void SketchScene::resetLine(){
    noisePos.set(2.123, 4.46, 32.9);
    pivotNoisePos.set(0.945, -7.777, 0);
    points.clear();
    path.clear();
}

void SketchScene::drawModel(){
    wideLines.begin();
    wideLines.setUniform2f("_line_width", lineWidth.get(),lineWidth.get()/10.0);
    wideLines.setUniform4f("_line_color", 1.0, 1.0, 0.0, 1.0);
    wideLines.setUniform4f("_viewport", 0.0,0.0, ofGetViewportWidth(), ofGetViewportHeight());
    path.draw();
    int i = 0;

    vbo.setVertexData(&path.getOutline()[0].getVertices()[0], path.getOutline()[0].size(), GL_DYNAMIC_DRAW);
    vbo.draw(GL_TRIANGLE_STRIP, 0, path.getOutline()[0].size());

    //vbo.draw(GL_LINE_STRIP, 0, path.getOutline()[0].size());

    wideLines.end();
    if(points.size()>2){
        ofDrawSphere(points[points.size()-1], radius);
    }
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
    vs_color    = vec4(1.0,1.0,1.0,1.0); //color;
    gl_PointSize = 1.0;
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
    vec2    projection  = v_start + clamp(t, 0.0, 1.0) * v_line;
    vec2    delta       = gl_FragCoord.xy - projection;
    float   d2          = dot(delta, delta);
    float   k           = clamp(_line_width.y - d2, 0.0, 1.0);
    float   endWeight   = step(abs(t * 2.0 - 1.0), 1);
    float   alpha       = mix(k, 1.0, endWeight);
    
    //out_color = vec4(_line_color.rgb, 1.0);
    out_color = vec4(v_color.rgb, alpha);
}

)";
