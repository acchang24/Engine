#pragma once
#include "EngineMath.h"

class VertexBuffer;

// Struct that matches the constants set in vertex shader
struct ObjectConstants
{
	Matrix4 modelToWorld;
};

class RenderObj
{
public:
	RenderObj();
	RenderObj(const VertexBuffer* vBuffer);
	virtual ~RenderObj();
	RenderObj(const RenderObj&) = delete;
	RenderObj& operator=(const RenderObj&) = delete;

	virtual void Draw();

	ObjectConstants mObjConsts;

protected:
	const VertexBuffer* mVertexBuffer;

	ID3D11Buffer* mConstBuffer;
};

