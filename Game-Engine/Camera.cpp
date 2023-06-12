#include "Camera.h"

#include "Enums.h"
#include "Utils.h"
#include "Input.h"

#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

#include <glm/gtx/transform.hpp>

namespace loom
{
	// TODO: Add this to a presets file
	static inline const float
		CAMERA_ROTATION_SPEED = .1f,
		CAMERA_MOVEMENT_SPEED = .1f,
		CAMERA_ZOOM_SPEED = .4f,
		MIN_CAMERA_ZOOM = 1.f,
		MAX_CAMERA_ZOOM = 20.f;

	void Camera::InitiateFreeCam()
	{
		Inputs::next();
		static mat4 mvp;
		mvp = (mat4)Camera::mvp;


		// WASD movement relative to where the camera is pointing (x, y)
		Inputs::AddInput([&]()
		{
			Camera::trns *= translate(vec3(Camera::yaww_mat * vec4(0, -1, 0, 1)) * CAMERA_MOVEMENT_SPEED);
		}, { GLFW_KEY_W, 255, GLFW_PRESS, 0 });
		Inputs::AddInput([&]()
		{
			Camera::trns *= translate(vec3(Camera::yaww_mat * vec4(1, 0, 0, 1)) * CAMERA_MOVEMENT_SPEED);
		}, { GLFW_KEY_A, 255, GLFW_PRESS, 0 });
		Inputs::AddInput([&]()
		{
			Camera::trns *= translate(vec3(Camera::yaww_mat * vec4(0, 1, 0, 1)) * CAMERA_MOVEMENT_SPEED);
		}, { GLFW_KEY_S, 255, GLFW_PRESS, 0 });
		Inputs::AddInput([&]()
		{
			Camera::trns *= translate(vec3(Camera::yaww_mat * vec4(-1, 0, 0, 1)) * CAMERA_MOVEMENT_SPEED);
		}, { GLFW_KEY_D, 255, GLFW_PRESS, 0 });
		Inputs::AddInput([&]()
		{
			Camera::trns *= translate(vec3(Camera::yaww_mat * vec4(0, 0, -1, 1)) * CAMERA_MOVEMENT_SPEED);
		}, { GLFW_KEY_SPACE, 255, GLFW_PRESS, 0 });
		Inputs::AddInput([&]()
		{
			Camera::trns *= translate(vec3(Camera::yaww_mat * vec4(0, 0, 1, 1)) * CAMERA_MOVEMENT_SPEED);
		}, { GLFW_KEY_LEFT_CONTROL, 255, GLFW_PRESS, 0 });


		// Click and drag m3 to rotate camera
		Inputs::AddInput([&]()
		{
			static double mx, my;
			Inputs::GetRelativeMousePos(mx, my);

			Camera::yaww += (float)-mx * CAMERA_ROTATION_SPEED;
			Camera::roll += (float)-my * CAMERA_ROTATION_SPEED;
			if (Camera::roll > 180)
				Camera::roll = 180;
			if (Camera::roll < 0)
				Camera::roll = 0;

		}, { 0, GLFW_MOUSE_BUTTON_3, GLFW_PRESS, 0 });


		// Reset camera on prev
		Inputs::AddOnPrev([]() { Camera::mvp = mvp; });
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

	void Camera::update()
	{
		if (GLFWwindow* window = glfwGetCurrentContext())
		{
			int w, h;
			glfwGetWindowSize(window, &w, &h);
			if (w == 0 || h == 0)
				return;
			aspect = (float)w / (float)h;
			proj = perspective(fovy, aspect, near, far);
            view = lookAt(eye, ctr, up);
            calculateRotationMatrix(roll, ptch, yaww, rotn);
			mode = rotn * trns;
			mvp = proj * view * mode;

            roll_mat = rotate(radians(roll), vec3(1, 0, 0));
            ptch_mat = rotate(radians(ptch), vec3(0, 1, 0));
            yaww_mat = rotate(radians(yaww), vec3(0, 0, 1));

			double mx, my;
			Inputs::GetMousePos(mx, my);

			mpos = inverse(Camera::mvp) * vec4(2 * mx / w - 1, 1 - 2 * my / h,  0, 1);
			mdir =
                inverse(Camera::rotn) *
                inverse(Camera::proj) *
                inverse(Camera::view) *
                vec4(2 * mx / w - 1, 1 - 2 * my / h, -1, 1);

			mpos /= mpos.w;
			mdir /= mdir.w;

			Renderable::render_all();
		};
	};
};
