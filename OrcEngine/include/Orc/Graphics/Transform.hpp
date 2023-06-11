#include "Engine/Core.hpp"

namespace orc {
	
class Transform
{
public:
	Transform() = default;
	virtual ~Transform() = default;

	void rotate(float angle);

	void scale(float x, float y);
	void scale(const Vector2f& size);

	void move(float x, float y);
	void move(const Vector2f& offset);
	
	void setRotation(float angle);

	void setScale(float x, float y);
	void setScale(const Vector2f& size);

	void setPosition(float x, float y);
	void setPosition(const Vector2f& offset);

	float getRotation() const;
	Vector2f getScale() const;
	Vector2f getPosition() const;

	const Matrix& getMatrix();
	const Matrix& getInverseMatrix();

protected:
	float m_rotation;
	Vector2f m_scale;
	Vector2f m_position;

	Matrix m_transformMatrix;
	Matrix m_inverseTransformMatrix;

	bool m_isMatrixUpdateNeeded = false;
	bool m_isInverseMatrixUpdateNeeded = false;
};

//class TransformMatrix
//{
//public:
//	TransformMatrix inversed();
//
//	TransformMatrix& rotate(float angle);
//
//	TransformMatrix& scale(float x, float y);
//	TransformMatrix& scale(const Vector2f& size);
//
//	TransformMatrix& translate(float x, float y);
//	TransformMatrix& translate(const Vector2f& offset);
//
//private:
//	Matrix m_matrix;
//};

}
