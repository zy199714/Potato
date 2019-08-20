/**
 * 
 */

#ifndef GAMEAPP_H
#define GAMEAPP_H

#pragma once
#include "tools/RenderStates.h"
#include "core/App3D.h"
#include "core/GameObject.h"
#include "core/FirstPersonGameCamera.h"
#include "core/RenderSky.h"
#include "core/RenderTerrain.h"

#include "Effects.h"


class GameApp : public Potato::App3D
{
public:
	GameApp(HINSTANCE hInstance, uint16_t width, uint16_t height);
	~GameApp();

	bool Init();
	void OnResize();

	void UpdateScene(float dt) override;
	void DrawScene() override;

private:
	bool InitResource();

	ID2D1SolidColorBrush* mColorBrush;				// ��ɫ��ˢ
	IDWriteFont* mFont;										// ����
	IDWriteTextFormat* mTextFormat;					// �ı���ʽ

	/* camera */
	FirstPersonGameCamera* mCamera;
	FirstPersonGameCamera* mShadowCamera;

	int cameraSpeed;

	/* effect */
	DefaultEffect defaultEffect;
	SkyEffect skyEffect;
	TerrainEffect terrainEffect;

	/* Render Sky */
	RenderSky* mSkySunset;

	/* Terrain */
	RenderTerrain* mTerrain;

	/*  */
	GameObject mBox;
	GameObject sphere;
	GameObject people;

	bool mShadowEnabled;
	float mShadowBits;
};
#endif // !GAMEAPP_H


