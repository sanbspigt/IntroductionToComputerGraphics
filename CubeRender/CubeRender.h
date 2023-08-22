#pragma once
#include<glad/glad.h>
#include<glfw/glfw3.h>

#include<ShaderH.h>
#include<iostream>
#include<cmath>



void InitCubeVerts(float x,float y,float length);
void RenderCube(const char* vertex, const char* fragment);
void DeleteCubeData();