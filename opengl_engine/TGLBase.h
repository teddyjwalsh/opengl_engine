#pragma once
#ifndef _TGL_BASE
#define _TGL_BASE

#define GLM_ENABLE_EXPERIMENTAL

#include "tgl_gl.h"

#include <iostream>
#include <vector>
#include <chrono>

#include "TGLMesh.h"
#include "TGLCamera.h"
#include "TGLPlayer.h"
#include "TGLPhysicsEngine.h"
#include "TGLHudElement.h"
#include "TGLRayBounce.h"



class TGLBase
{
	int window_height;
	int window_width;
	

#ifdef _TGL_CLIENT
	GLFWwindow* window;
#endif

	std::vector <TGLMesh*> meshes;
	std::vector <TGLActor*> actors;
	std::vector <TGLHudElement*> HUD_elements;
	TGLCamera * active_camera;
	TGLActor * chunks_spawner;
	TGLPhysicsEngine physics_engine;

	std::chrono::steady_clock::time_point end;
	std::chrono::steady_clock::time_point begin;
	double time_sum;
	int time_count;

	GLuint default_shader_program;

	glm::vec3 shadow_pos1;
	glm::vec3 shadow_pos2;
	glm::mat4 depthMVP1;
	glm::mat4 depthMVP2;

public:
	TGLBase();
	~TGLBase();

	TGLRayBounce ray_bounce;

#ifdef _TGL_CLIENT
	bool gl_init();
	GLFWwindow * gl_create_window(int in_width, int in_height);
	bool glad_init();
	void processInput(GLFWwindow *window);
	GLFWwindow * get_window();
	void add_camera(TGLCamera * in_camera);
	void add_hud_element(TGLHudElement * in_element);
	void load_model(float * vertices);
	void load_shader(char * vertex_shader, char * fragment_shader);
#endif

	
	int init();
	void update();
	void add_mesh(TGLMesh * in_mesh);
	void add_actor(TGLActor * in_actor);
	void remove_actor(TGLActor * in_actor);
	
	TGLPlayer * get_player();
	glm::vec3 get_player_pos();
	void set_world_actor(TGLActor * in_actor);
	void get_game_state();
};


#endif
