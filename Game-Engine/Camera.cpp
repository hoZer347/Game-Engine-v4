#include "Camera.h"

#include "Loom.h"
#include "Enums.h"
#include "Utils.h"
#include "Control.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include <glm/gtx/transform.hpp>

namespace loom
{
	static inline const float
		CAMERA_ROTATION_SPEED = .1f,
		CAMERA_MOVEMENT_SPEED = .1f,
		CAMERA_ZOOM_SPEED = .4f,
		MIN_CAMERA_ZOOM = 1.f,
		MAX_CAMERA_ZOOM = 20.f;

	Camera::Camera()
	{
		Engine::Add(this);
	};
    void Camera::InitiateFreeCam(Control& control)
	{
		static mat4 mvp;
		mvp = (mat4)Camera::mvp;

		// Wrap into one mutex
		control.next([&control]()
		{
			// WASD movement relative to where the camera is pointing (x, y)
			control.AddOutput([](float& f)
			{
				if (f) Camera::trns *= translate(vec3(Camera::yaww_mat * vec4(0, -1, 0, 1)) * CAMERA_MOVEMENT_SPEED);
			}, GLFW_KEY_W);
			control.AddOutput([](float& f)
			{
				if (f) Camera::trns *= translate(vec3(Camera::yaww_mat * vec4(1, 0, 0, 1)) * CAMERA_MOVEMENT_SPEED);
			}, GLFW_KEY_A);
			control.AddOutput([](float& f)
			{
				if (f) Camera::trns *= translate(vec3(Camera::yaww_mat * vec4(0, 1, 0, 1)) * CAMERA_MOVEMENT_SPEED);
			}, GLFW_KEY_S);
			control.AddOutput([](float& f)
			{
				if (f) Camera::trns *= translate(vec3(Camera::yaww_mat * vec4(-1, 0, 0, 1)) * CAMERA_MOVEMENT_SPEED);
			}, GLFW_KEY_D);
			control.AddOutput([](float& f)
			{
				if (f) Camera::trns *= translate(vec3(Camera::yaww_mat * vec4(0, 0, -1, 1)) * CAMERA_MOVEMENT_SPEED);
			}, GLFW_KEY_SPACE);
			control.AddOutput([](float& f)
			{
				if (f) Camera::trns *= translate(vec3(Camera::yaww_mat * vec4(0, 0, 1, 1)) * CAMERA_MOVEMENT_SPEED);
			}, GLFW_KEY_LEFT_CONTROL);

			// Click and drag m3 to rotate camera
			control.AddOutput([](float& f)
			{
				if (f)
				{
					Camera::yaww += -(float)(Control::mx - Control::pmx) * CAMERA_ROTATION_SPEED;
					Camera::roll += -(float)(Control::my - Control::pmy) * CAMERA_ROTATION_SPEED;

					if (Camera::roll > 180)
						Camera::roll = 180;
					if (Camera::roll < 0)
						Camera::roll = 0;
				};
			}, GLFW_MOUSE_BUTTON_MIDDLE);
		});
	};


    void  calculateRotationMatrix(float& roll, float& pitch, float& yaw, mat4& rotationMatrix) {
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


	void Camera::render()
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

			mpos =	inverse(Camera::mvp) * vec4(2 * Control::mx / screen_w - 1, 1 - 2 * Control::my / screen_h, 0, 1);
			mdir =	inverse(Camera::rotn) *
					inverse(Camera::proj) *
					inverse(Camera::view) *
					vec4(2 * Control::mx / screen_w - 1, 1 - 2 * Control::my / screen_h, -1, 1);

			mpos /= mpos.w;
			mdir /= mdir.w;
		};
	};
};
