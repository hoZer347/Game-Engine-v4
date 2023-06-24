#pragma once

#include "Loom.h"
#include "Enums.h"
#include "Utils.h"
#include "Control.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include <glm/gtx/transform.hpp>

#include <glm/glm.hpp>
using namespace glm;

#include <vector>

namespace loom
{
	struct Control;
	struct Camera :
		virtual public Renderable
	{
		// Initiates "Free Cam"
		// Controls:
		// - Press and hold middle mouse button to rotate camera
		// - WASD movement relative to camera rotation around the Z axis
		// - LEFT_CTRL + SPACE go up and down on the Z axis
		// - ESCAPE exits Free Cam
		static void InitiateFreeCam(Control& control);

		static inline mat4	vp			= mat4(1),
							mvp			= mat4(1),
							trns		= mat4(1),
							rotn		= mat4(1),
							mode		= mat4(1),
							view		= mat4(1),
							proj		= mat4(1),
							roll_mat	= mat4(1),
							ptch_mat	= mat4(1),
							yaww_mat	= mat4(1);

		static inline float	fovy		= 45.f,
							aspect		= 1.f,
							near		= .1f,
							far			= 1000.f,
							roll		= 0.f,
							ptch		= 0.f,
							yaww		= 0.f;

		static inline vec3	eye			= vec3(0, 0, 1),
							ctr			= vec3(0, 0, 0),
							up			= vec3(0, 1, 0);

		static inline vec4	mpos = vec4(1), mdir = vec4(1);

		static inline int	screen_w = 1, screen_h = 1;

	private:
		void render() override;
	};
	//
};



namespace loom
{
	static inline const float
		CAMERA_ROTATION_SPEED = .1f,
		CAMERA_MOVEMENT_SPEED = .1f,
		CAMERA_ZOOM_SPEED = .4f,
		MIN_CAMERA_ZOOM = 1.f,
		MAX_CAMERA_ZOOM = 20.f;


	inline void Camera::InitiateFreeCam(Control& control)
	{
		static mat4 mvp;
		mvp = (mat4)Camera::mvp;

		// Wrap into one mutex
		control.next([&control]()
		{
			// WASD movement relative to where the camera is pointing (x, y)
			control.AddTask([]()
			{
				if (inputs[GLFW_KEY_W])
					Camera::trns *= translate(vec3(Camera::yaww_mat * vec4(0, -1, 0, 1)) * CAMERA_MOVEMENT_SPEED);
				if (inputs[GLFW_KEY_A])
					Camera::trns *= translate(vec3(Camera::yaww_mat * vec4(1, 0, 0, 1)) * CAMERA_MOVEMENT_SPEED);
				if (inputs[GLFW_KEY_S])
					Camera::trns *= translate(vec3(Camera::yaww_mat * vec4(0, 1, 0, 1)) * CAMERA_MOVEMENT_SPEED);
				if (inputs[GLFW_KEY_D])
					Camera::trns *= translate(vec3(Camera::yaww_mat * vec4(-1, 0, 0, 1)) * CAMERA_MOVEMENT_SPEED);
				if (inputs[GLFW_KEY_SPACE])
					Camera::trns *= translate(vec3(Camera::yaww_mat * vec4(0, 0, -1, 1)) * CAMERA_MOVEMENT_SPEED);
				if (inputs[GLFW_KEY_LEFT_CONTROL])
					Camera::trns *= translate(vec3(Camera::yaww_mat * vec4(0, 0, 1, 1)) * CAMERA_MOVEMENT_SPEED);
				if (inputs[GLFW_MOUSE_BUTTON_MIDDLE])
				{
					Camera::yaww += -(float)(Control::mx - Control::pmx) * CAMERA_ROTATION_SPEED;
					Camera::roll += -(float)(Control::my - Control::pmy) * CAMERA_ROTATION_SPEED;
				};
			});
		});
	};


	inline void  calculateRotationMatrix(float& roll, float& pitch, float& yaw, mat4& rotationMatrix) {
		// Convert degrees to radians
		float rollRad = radians(roll);
		float pitchRad = radians(pitch);
		float yawRad = radians(yaw);

		// Calculate the sine and cosine of the angles
		float sinRoll = sin(rollRad);
		float cosRoll = cos(rollRad);
		float sinPtch = sin(pitchRad);
		float cosPtch = cos(pitchRad);
		float sinYaww = sin(yawRad);
		float cosYaww = cos(yawRad);

		// Calculate the elements of the rotation matrix
		rotationMatrix[0][0] = cosYaww * cosPtch;
		rotationMatrix[0][1] = cosYaww * sinPtch * sinRoll - sinYaww * cosRoll;
		rotationMatrix[0][2] = cosYaww * sinPtch * cosRoll + sinYaww * sinRoll;
		rotationMatrix[0][3] = 0.0f;
		rotationMatrix[1][0] = sinYaww * cosPtch;
		rotationMatrix[1][1] = sinYaww * sinPtch * sinRoll + cosYaww * cosRoll;
		rotationMatrix[1][2] = sinYaww * sinPtch * cosRoll - cosYaww * sinRoll;
		rotationMatrix[1][3] = 0.0f;
		rotationMatrix[2][0] = -sinPtch;
		rotationMatrix[2][1] = cosPtch * sinRoll;
		rotationMatrix[2][2] = cosPtch * cosRoll;
		rotationMatrix[2][3] = 0.0f;
		rotationMatrix[3][0] = 0.0f;
		rotationMatrix[3][1] = 0.0f;
		rotationMatrix[3][2] = 0.0f;
		rotationMatrix[3][3] = 1.0f;
	};


	inline void Camera::render()
	{
		if (GLFWwindow* window = glfwGetCurrentContext())
		{
			glfwGetWindowSize(window, &screen_w, &screen_h);
			if (screen_w <= 0 || screen_h <= 0)
				return;

			aspect = (float)screen_w / (float)screen_h;
			proj = perspective(fovy, aspect, near, far);
			view = lookAt(eye, ctr, up);
			calculateRotationMatrix(roll, ptch, yaww, rotn);
			mode = rotn * trns;
			vp = proj * view;
			mvp = proj * view * mode;

			roll_mat = rotate(radians(roll), vec3(1, 0, 0));
			ptch_mat = rotate(radians(ptch), vec3(0, 1, 0));
			yaww_mat = rotate(radians(yaww), vec3(0, 0, 1));

			mpos = inverse(Camera::mvp) * vec4(2 * Control::mx / screen_w - 1, 1 - 2 * Control::my / screen_h, 0, 1);
			mdir = inverse(Camera::rotn) *
				inverse(Camera::proj) *
				inverse(Camera::view) *
				vec4(2 * Control::mx / screen_w - 1, 1 - 2 * Control::my / screen_h, -1, 1);

			mpos /= mpos.w;
			mdir /= mdir.w;
		};
	};
};
