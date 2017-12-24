#pragma once

#ifndef UTILITY_NAMESPACE_USING
#define UTILITY_NAMESPACE_USING 1
#endif

//--------------------------------
#include <UtilityLib\ConstantBuffer.h>
#include <UtilityLib\Encode.h>
#include <UtilityLib\Function.h>
#include <UtilityLib\Macro.h>
#include <UtilityLib\Tween.h>
#include <UtilityLib\Archive\Archive.h>
#include <UtilityLib\Camera\BottomViewCamera.h>
#include <UtilityLib\Camera\Camera.h>
#include <UtilityLib\Camera\DebugCamera.h>
#include <UtilityLib\Camera\FollowingCamera.h>
#include <UtilityLib\Collision\Collision2D.h>
#include <UtilityLib\Collision\Collision3D.h>
#include <UtilityLib\Collision\Shape2D.h>
#include <UtilityLib\ConfigManager\ConfigManager.h>
#include <UtilityLib\Debug\Debug.h>
#include <UtilityLib\Debug\InitializeSpy.h>
#include <UtilityLib\DeviceResources\DeviceResources.h>
#include <UtilityLib\DirectX\Direct2DBase.h>
#include <UtilityLib\DirectX\DWriteBase.h>
#include <UtilityLib\EventManager\Delegate.h>
#include <UtilityLib\EventManager\Event.h>
#include <UtilityLib\EventManager\EventManager.h>
#include <UtilityLib\Fead\CFade.h>
#include <UtilityLib\Fead\RippleFade.h>
#include <UtilityLib\Fead\SimpleFade.h>
#include <UtilityLib\Fead\SlideFade.h>
#include <UtilityLib\GraphicManager\BufferObjectStructure.h>
#include <UtilityLib\GraphicManager\Graphic2DBase.h>
#include <UtilityLib\GraphicManager\Graphic3DBase.h>
#include <UtilityLib\GraphicManager\GraphManager.h>
#include <UtilityLib\GraphicManager\BlendStateManager\BlendStateManager.h>
#include <UtilityLib\GraphicManager\Gauge\CircleGauge.h>
#include <UtilityLib\GraphicManager\Gauge\Gauge.h>
#include <UtilityLib\GraphicManager\Gauge\PaintBrush.h>
#include <UtilityLib\GraphicManager\Gauge\RectGauge.h>
#include <UtilityLib\GraphicManager\RasterizerStateManager\RasterizerStateManager.h>
#include <UtilityLib\GraphicManager\ScreenShot\ScreenShot.h>
#include <UtilityLib\GraphicManager\Shader\ComputeShader.h>
#include <UtilityLib\GraphicManager\Shader\GeometryShader.h>
#include <UtilityLib\GraphicManager\Shader\HullShader.h>
#include <UtilityLib\GraphicManager\Shader\IndexBuffer.h>
#include <UtilityLib\GraphicManager\Shader\InputLayout.h>
#include <UtilityLib\GraphicManager\Shader\PixelShader.h>
#include <UtilityLib\GraphicManager\Shader\Shader.h>
#include <UtilityLib\GraphicManager\Shader\ShaderSummary.h>
#include <UtilityLib\GraphicManager\Shader\StructuredBuffer.h>
#include <UtilityLib\GraphicManager\Shader\VertexBuffer.h>
#include <UtilityLib\GraphicManager\Shader\VertexShader.h>
#include <UtilityLib\GraphicManager\SpriteBatch\SpriteBatch.h>
#include <UtilityLib\GraphicManager\TextureImage\TextureImage.h>
#include <UtilityLib\Grid\Grid.h>
#include <UtilityLib\InputManager\GamePad.h>
#include <UtilityLib\InputManager\InputManager.h>
#include <UtilityLib\InputManager\Keyboard.h>
#include <UtilityLib\InputManager\Mouse.h>
#include <UtilityLib\Loader\File.h>
#include <UtilityLib\Loader\Loader.h>
#include <UtilityLib\Math\Math.h>
#include <UtilityLib\Math\Matrix.h>
#include <UtilityLib\Math\Quaternion.h>
#include <UtilityLib\Math\Vector2f.h>
#include <UtilityLib\Math\Vector3f.h>
#include <UtilityLib\Math\Vector4f.h>
#include <UtilityLib\Menu\CMenu.h>
#include <UtilityLib\Menu\CMenuButton.h>
#include <UtilityLib\Menu\IButton.h>
#include <UtilityLib\QueryPerformanceTimer\QueryPerformanceTimer.h>
#include <UtilityLib\RenderTarget\RenderTarget.h>
#include <UtilityLib\Resource\GridPS.h>
#include <UtilityLib\Resource\GridVS.h>
#include <UtilityLib\Resource\Icon.h>
#include <UtilityLib\Resource\Resource.h>
#include <UtilityLib\Resource\RippleFadePS.h>
#include <UtilityLib\Resource\RippleFadeVS.h>
#include <UtilityLib\Resource\SlideFadePS.h>
#include <UtilityLib\Resource\SlideFadeVS.h>
#include <UtilityLib\Resource\SpriteBatchPS.h>
#include <UtilityLib\Resource\SpriteBatchVS.h>
#include <UtilityLib\Singleton\Singleton.h>
#include <UtilityLib\Sound\Bgm.h>
#include <UtilityLib\Sound\OrgSound.h>
#include <UtilityLib\Sound\Sound.h>
#include <UtilityLib\Sound\SoundManager.h>
#include <UtilityLib\String\String.h>
#include <UtilityLib\Task\Task.h>
#include <UtilityLib\Task\TaskManager.h>
#include <UtilityLib\Window\Window.h>
#include <UtilityLib\Window\Viewport\ViewportManager.h>

//--------------------------------
#if defined(UTILITY_NAMESPACE_USING) && (UTILITY_NAMESPACE_USING == 1)
	using namespace Utility;
#endif
