#include "Commands.h"

#include "GLFW/glfw3.h"
#include "glm/gtx/transform.hpp"

#include "Camera.h"
#include "Constants.h"
#include "Input.h"
#include "Utils.h"
#include "Grid.h"

namespace loom
{
	void bindCameraToGrid(Camera& camera, Grid& grid)
	{
		// WASD movement relative to where the camera is pointing (x, y)
		Input::KeyHold([&]()
		{
			vec4 dir = normalize(camera.ctr - camera.eye) * -CAMERA_MOVEMENT_SPEED;
			dir.z = 0;
			camera.eye += dir;
			camera.ctr += dir;
		}, GLFW_KEY_W, GLFW_PRESS, 0);
		Input::KeyHold([&]()
		{
			vec4 dir = camera.eye - camera.ctr;
			dir = normalize(vec4(dir.y, -dir.x, 0, 1)) * -CAMERA_MOVEMENT_SPEED;
			camera.eye += dir;
			camera.ctr += dir;
		}, GLFW_KEY_A, GLFW_PRESS, 0);
		Input::KeyHold([&]()
		{
			vec4 dir = normalize(camera.eye - camera.ctr) * -CAMERA_MOVEMENT_SPEED;
			dir.z = 0;
			camera.eye += dir;
			camera.ctr += dir;
		}, GLFW_KEY_S, GLFW_PRESS, 0);
		Input::KeyHold([&]()
		{
			vec4 dir = camera.ctr - camera.eye;
			dir = normalize(vec4(dir.y, -dir.x, 0, 1)) * -CAMERA_MOVEMENT_SPEED;
			camera.eye += dir;
			camera.ctr += dir;
		}, GLFW_KEY_D, GLFW_PRESS, 0);
		//



		// Click and drag m3 to rotate camera
		Input::MouseButtonHold([&]()
		{
			static double mx, my;
			Input::GetRelativeMousePos(mx, my);

			vec4 eye = vec4(camera.eye.x, camera.eye.y, 0, 0);
			vec4 ctr = vec4(camera.ctr.x, camera.ctr.y, 0, 0);
			vec4 rot = rotate(mat4(1), (float)mx * -CAMERA_ROTATION_SPEED, vec3(0, 0, 1)) * (eye - ctr) + ctr;

			camera.eye = vec4(rot.x, rot.y, camera.eye.z, camera.eye.w);
		}, GLFW_MOUSE_BUTTON_3, GLFW_PRESS, 0);
		//



		// Scroll to zoom in / out
		Input::ScrollWheel([&](double sx, double sy)
		{
			camera.eye += normalize(camera.ctr - camera.eye) *= sy * CAMERA_ZOOM_SPEED;
		});
		//



		// TODO: Bind camera.ctr to grid
	};
};
