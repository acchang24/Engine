#include "stdafx.h"
#include "Texture.h"
#include "Graphics.h"

Texture::Texture()
	: mResource(nullptr)
	, mView(nullptr)
	, mWidth(0)
	, mHeight(0)
{

}

Texture::~Texture()
{
	Free();
}

void Texture::Free()
{
	if (mView)
	{
		mView->Release();
		mView = nullptr;
	}
	if (mResource)
	{
		mResource->Release();
		mResource = nullptr;
	}
	mWidth = 0;
	mHeight = 0;
}

void Texture::SetActive(int slot) const
{
	
}