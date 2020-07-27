#include "Camera.h"

namespace XGL
{
	void Camera::updateAxis()
	{
		view = View::lookAt(position, position + front, up);
		updateToAxis();
		updateToEuler();
	}

	void Camera::updateEuler()
	{
		view = View::euler(position, yaw, pitch, roll);
		updateToAxis();
	}

	void Camera::updatePosition()
	{
		view(0, 3) = view(0, 0) * -position.x() + view(0, 1) * -position.y() + view(0, 2) * -position.z();
		view(1, 3) = view(1, 0) * -position.x() + view(1, 1) * -position.y() + view(1, 2) * -position.z();
		view(2, 3) = view(2, 0) * -position.x() + view(2, 1) * -position.y() + view(2, 2) * -position.z();
	}

	void Camera::updateLen()
	{
		projection = Projection::perspFov(fov, aspect, near, far);
	}

	void Camera::updateToAxis()
	{
		front.x() = -view(2, 0);
		front.y() = -view(2, 1);
		front.z() = -view(2, 2);
		right.x() = view(0, 0);
		right.y() = view(0, 1);
		right.z() = view(0, 2);
		up.x() = view(1, 0);
		up.y() = view(1, 1);
		up.z() = view(1, 2);
	}

	void Camera::updateToEuler()
	{
		yaw = atan2(right.x(), -right.z());
		pitch = atan2(front.y(), sqrt(front.x() * front.x() + front.z() * front.z()));
		roll = 0;
	}

	Camera::Camera()
	{
		yaw = pitch = roll = 0;
		target_yaw = target_pitch = target_roll = 0;
		updateEuler();
		fov = degToRad(45.f);
		target_fov = fov;
		aspect = (float)16 / 9;
		near = 0.1;
		far = 100;
		updateLen();
		smooth_euler = smooth_fov = smooth_position = 0;
	}

	void Camera::setPosition(Vec3 pos)
	{
		target_position = pos;
	}

	void Camera::setEuler(float yaw, float pitch, float roll)
	{
		if (pitch > PI / 2 || pitch < -PI / 2)
			std::cerr << "WARNING | XGL::Camera::setEuler(float, float, float) : Value out of bounds.\n";
		target_yaw = yaw;
		pitch = pitch > PI / 2 ? PI / 2 : pitch < -PI / 2 ? -PI / 2 : pitch;
		target_pitch = pitch;
		target_roll = roll;
	}
	void Camera::setLen(float fov, float aspect, float near, float far)
	{
		if (fov > 2 * PI / 3 || fov < PI / 18)
			std::cerr << "WARNING | XGL::Camera::setLen(float, float, float, float) : Value out of bounds.\n";
		fov = fov > 2 * PI / 3 ? 2 * PI / 3 : fov < PI / 18 ? PI / 18 : fov;
		target_fov = fov;
		this->aspect = aspect;
		this->near = near;
		this->far = far;
	}

	void Camera::setFov(float fov)
	{
		if (fov > 2 * PI / 3 || fov < PI / 18)
			std::cerr << "WARNING | XGL::Camera::setFov(float) : Value out of bounds.\n";
		fov = fov > 2 * PI / 3 ? 2 * PI / 3 : fov < PI / 18 ? PI / 18 : fov;
		target_fov = fov;
	}

	void Camera::setAspect(float aspect)
	{
		if (aspect <= 0)
		{
			std::cerr << "ERROR | XGL::Camera::setAspect(float) : Invalid set value.\n";
			throw INVALID_SET_VALUE;
		}
		this->aspect = aspect;
	}

	void Camera::moveForward(float distance)
	{
		target_position += distance * front;
	}

	void Camera::moveBackward(float distance)
	{
		target_position -= distance * front;
	}

	void Camera::moveForwardAl(float distance)
	{
		target_position.x() += distance * sin(yaw);
		target_position.z() -= distance * cos(yaw);
	}

	void Camera::moveBackwardAl(float distance)
	{
		target_position.x() -= distance * sin(yaw);
		target_position.z() += distance * cos(yaw);
	}

	void Camera::moveLeft(float distance)
	{
		target_position -= distance * right;
	}

	void Camera::moveRight(float distance)
	{
		target_position += distance * right;
	}

	void Camera::moveLeftAl(float distance)
	{
		target_position.x() -= distance * cos(yaw);
		target_position.z() -= distance * sin(yaw);
	}

	void Camera::moveRightAl(float distance)
	{
		target_position.x() += distance * cos(yaw);
		target_position.z() += distance * sin(yaw);
	}

	void Camera::moveUp(float distance)
	{
		target_position += distance * up;
	}

	void Camera::moveDown(float distance)
	{
		target_position -= distance * up;
	}

	void Camera::moveUpAl(float distance)
	{
		target_position.y() += distance;
	}

	void Camera::moveDownAl(float distance)
	{
		target_position.y() -= distance;
	}

	void Camera::move(Vec3 shift)
	{
		target_position += shift.x() * right + shift.y() * up - shift.z() * front;
	}

	void Camera::moveAl(Vec3 shift)
	{
		target_position.x() += shift.x() * cos(yaw) - shift.z() * sin(yaw);
		target_position.y() += shift.y();
		target_position.z() += shift.y() * sin(yaw) + shift.z() * cos(yaw);
	}

	void Camera::rotateYaw(float angle)
	{
		target_yaw += angle;
	}

	void Camera::rotatePitch(float angle)
	{
		target_pitch += angle;
		target_pitch = target_pitch > PI / 2 ? PI / 2 : target_pitch < -PI / 2 ? -PI / 2 : target_pitch;
	}

	void Camera::rotateRoll(float angle)
	{
		target_roll += angle;
	}

	void Camera::rotate(float yaw, float pitch, float roll)
	{
		target_yaw += yaw;
		target_pitch += pitch;
		target_roll += roll;
		target_pitch = target_pitch > PI / 2 ? PI / 2 : target_pitch < -PI / 2 ? -PI / 2 : target_pitch;
	}

	void Camera::zoom(float coe)
	{
		target_fov = 2 * atan((1 - coe) * tan(target_fov / 2));
		target_fov = target_fov > 2 * PI / 3 ? 2 * PI / 3 : target_fov < PI / 18 ? PI / 18 : target_fov;
	}

	void Camera::smoothMove(float factor)
	{
		smooth_position = factor < 0 ? 0 : factor;
	}

	void Camera::smoothRotate(float factor)
	{
		smooth_euler = factor < 0 ? 0 : factor;
	}

	void Camera::smoothZoom(float factor)
	{
		smooth_fov = factor < 0 ? 0 : factor;
	}

	void Camera::update(float deltaT)
	{
		float k;

		k = 1 / (1 + smooth_euler / deltaT);
		yaw = target_yaw * k + yaw * (1 - k);
		pitch = target_pitch * k + pitch * (1 - k);
		roll = target_roll * k + roll * (1 - k);

		k = 1 / (1 + smooth_position / deltaT);
		position = target_position * k + position * (1 - k);

		updateEuler();

		k = 1 / (1 + smooth_fov / deltaT);
		fov = target_fov * k + fov * (1 - k);
		updateLen();

	}

	Mat4& Camera::viewMat()
	{
		return view;
	}

	Mat4& Camera::projectionMat()
	{
		return projection;
	}
}
