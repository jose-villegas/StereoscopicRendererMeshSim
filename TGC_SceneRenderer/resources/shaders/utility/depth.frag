#version 440 core
//--include shared_data.glsl

// Ouput data
layout(location = 0) out float fragmentDepth;

void main(){
	fragmentDepth = gl_FragCoord.z;
}