#include "Commands.h"

#include "GLFW/glfw3.h"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/intersect.hpp"

#include "Camera.h"
#include "Constants.h"
#include "Input.h"

namespace loom
{
	void Commands::InitiateFreeCam()
	{
		// WASD movement relative to where the camera is pointing (x, y)
		Input::KeyHold([&]()
		{
			Camera::trns *= translate(vec3(Camera::yaww_mat * vec4(0, -1, 0, 1)) * CAMERA_MOVEMENT_SPEED);
		}, GLFW_KEY_W, GLFW_PRESS, 0);
		Input::KeyHold([&]()
		{
			Camera::trns *= translate(vec3(Camera::yaww_mat * vec4(1, 0, 0, 1)) * CAMERA_MOVEMENT_SPEED);
		}, GLFW_KEY_A, GLFW_PRESS, 0);
		Input::KeyHold([&]()
		{
			Camera::trns *= translate(vec3(Camera::yaww_mat * vec4(0, 1, 0, 1)) * CAMERA_MOVEMENT_SPEED);
		}, GLFW_KEY_S, GLFW_PRESS, 0);
		Input::KeyHold([&]()
		{
			Camera::trns *= translate(vec3(Camera::yaww_mat * vec4(-1, 0, 0, 1)) * CAMERA_MOVEMENT_SPEED);
		}, GLFW_KEY_D, GLFW_PRESS, 0);
		Input::KeyHold([&]()
		{
			Camera::trns *= translate(vec3(Camera::yaww_mat * vec4(0, 0, -1, 1)) * CAMERA_MOVEMENT_SPEED);
		}, GLFW_KEY_SPACE, GLFW_PRESS, 0);
		Input::KeyHold([&]()
		{
			Camera::trns *= translate(vec3(Camera::yaww_mat * vec4(0, 0, 1, 1)) * CAMERA_MOVEMENT_SPEED);
		}, GLFW_KEY_LEFT_CONTROL, GLFW_PRESS, 0);
		//



		// Click and drag m3 to rotate camera
		Input::MouseButtonHold([&]()
		{
			static double mx, my;
			Input::GetRelativeMousePos(mx, my);

			Camera::yaww += (float)-mx * CAMERA_ROTATION_SPEED;
			Camera::roll += (float)-my * CAMERA_ROTATION_SPEED;
		}, GLFW_MOUSE_BUTTON_3, GLFW_PRESS, 0);
		//
	};
};
