#include "Commands.h"

#include "GLFW/glfw3.h"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/intersect.hpp"

#include "Utils.h"
#include "Camera.h"
#include "Geometry.h"
#include "Constants.h"
#include "Input.h"
#include "Utils.h"
#include "Grid.h"

namespace loom
{
	void Commands::bindCameraToGrid(Grid& grid)
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



		// Manage zooming in / out + highlighting the proper grid cell
		Input::AddTask([&]()
		{
			double sx, sy;
			Input::GetRelativeScrollPos(sx, sy);

			if (sy)
			{
				Camera::eye.z += CAMERA_ZOOM_SPEED * -(float)sy;
				if (Camera::eye.z < MIN_CAMERA_ZOOM) Camera::eye.z = MIN_CAMERA_ZOOM;
				else if (Camera::eye.z > MAX_CAMERA_ZOOM) Camera::eye.z = MAX_CAMERA_ZOOM;
			};

			vec2 b;
			float d;

			for (auto& _cells : grid.cells)
				for (auto& cell : _cells)
					if (cell.v0 == -1) { std::cout << "Test: " << cell.pos.x << ", " << cell.pos.y << std::endl; continue; }
					else if (intersectRayTriangle(vec3(Camera::mpos), vec3(Camera::mdir),
							 vec3(grid.vtxs[grid.inds[cell.v0]]),
							 vec3(grid.vtxs[grid.inds[cell.v1]]),
							 vec3(grid.vtxs[grid.inds[cell.v2]]),
							 b, d) ||
							 intersectRayTriangle(vec3(Camera::mpos), vec3(Camera::mdir),
							 vec3(grid.vtxs[grid.inds[cell.v0]]),
							 vec3(grid.vtxs[grid.inds[cell.v3]]),
							 vec3(grid.vtxs[grid.inds[cell.v2]]),
							 b, d))
					{
						if (&cell == Grid::Hovered::cell)
							return;
						if (Grid::Hovered::cell)
							Grid::Hovered::cell->onUnhover();
						Grid::Hovered::cell = &cell;
						Grid::Hovered::cell->onHover();
						Grid::Hovered::inds =
						{
							grid.inds[cell.v0],
							grid.inds[cell.v1],
							grid.inds[cell.v2],
							grid.inds[cell.v3],
						};
					};
		});
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
