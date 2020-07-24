#ifndef XGL_CAMERA_H
#define XGL_CAMERA_H

#include <Math/View.h>
#include <Math/Projection.h>
#include <Math/Tool.h>

namespace XGL
{
	class Camera
	{
		private:
			// --- position ---
			Vec3 position;
			Vec3 target_position;
			float smooth_position;

			// --- axis ---
			Vec3 front;
			Vec3 up;
			Vec3 right;

			// --- euler ---
			float yaw;
			float pitch;
			float roll;
			float target_yaw;
			float target_pitch;
			float target_roll;
			float smooth_euler;

			// --- len ---
			float fov;
			float aspect;
			float near;
			float far;
			float target_fov;
			float smooth_fov;

			// --- output ---
			Mat4 view;
			Mat4 projection;

			// --- update from property ---
			void updateAxis();
			void updateEuler();
			void updatePosition();
			void updateLen();

			// --- update to property ---
			void updateToAxis();
			void updateToEuler();

		public:
			Camera();
			~Camera() {}

			void setPosition(Vec3 pos);
			void setEuler(float yaw, float pitch, float roll = 0);
			void setLen(float fov, float aspect, float near = 0.1, float far = 100);
			void setAspect(float aspect);

			void moveForward(float distance);
			void moveBackward(float distance);
			void moveForwardAl(float distance);
			void moveBackwardAl(float distance);
			void moveLeft(float distance);
			void moveRight(float distance);
			void moveLeftAl(float distance);
			void moveRightAl(float distance);
			void moveUp(float distance);
			void moveDown(float distance);
			void moveUpAl(float distance);
			void moveDownAl(float distance);
			void move(Vec3 shift);
			void moveAl(Vec3 shift);

			void rotateYaw(float angle);
			void rotatePitch(float angle);
			void rotateRoll(float angle);
			void rotate(float yaw, float pitch, float roll = 0);

			void zoom(float coe);

			void smoothMove(float factor);
			void smoothRotate(float factor);
			void smoothZoom(float factor);

			void update(float deltaT);

			Mat4& viewMat();
			Mat4& projectionMat();
	};
}

#endif // !XGL_CAMERA_H
