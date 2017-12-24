/**
 *	@file	PaintBrush.cpp
 *	@date	2017 / 7 / 25
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#include "PaintBrush.h"
#include "../../Math/Math.h"

#include <wrl/client.h>

class Utility::PaintBrush::Impl
{
public:
	Impl()
		:mainColor_(D2D1::ColorF(D2D1::ColorF::White)), subColor_(D2D1::ColorF(D2D1::ColorF::White)),
		colorShadow_(D2D1::ColorF(D2D1::ColorF::Gray, 0.5f)), round_(D2D1::Size(0.f, 0.f)),isShadow_(false),
		lineWidth_(1.0f), arcStartAngle_(Utility::Math::ToRadian(0.0f))
	{
		// RenderTarget依存のブラシは、ここでは作成できない
		ShadowOffset(5.f, 5.f);
	}

	~Impl() = default;
private:
	/**
	 *	@fn				CreateGeoAndSink
	 *	@brief			ジオメトリとシンクの取得
	 *	@param[in,out]	argGeo	!<	ジオメトリを格納する変数
	 *	@param[out]		argSink	!<	シンクを格納する変数
	 */
	void CreateGeoAndSink(Microsoft::WRL::ComPtr<ID2D1PathGeometry> *argGeo, Microsoft::WRL::ComPtr<ID2D1GeometrySink> *argSink)
	{
		// パスジオメトリ生成
		factory_->CreatePathGeometry(argGeo->GetAddressOf());
		// シンク作成
		(*argGeo)->Open(argSink->GetAddressOf());
	}

public:
	/**
	 *	@fn			Begin
	 *	@brief		描画の開始コール
	 *	@param[in]	argRt	!<	描画するレンダーターゲット
	 */
	void Begin(ID2D1RenderTarget* argRt)
	{
		renderTarget_ = argRt;
		// ファクトリの取得
		Microsoft::WRL::ComPtr<ID2D1Factory> factory;
		argRt->GetFactory(factory.GetAddressOf());
		factory_ = factory.Get();

		// ブラシの作成
		SetMainColor(mainColor_);
		SetSubColor(subColor_);
		SetShadowColor(colorShadow_);
	}
	/**
	 *	@fn		End
	 *	@brief	描画の終了コール
	 */
	void End()
	{
		renderTarget_->SetTransform(D2D1::IdentityMatrix());
	}

	// カラー設定
	void SetMainColor(const D2D1_COLOR_F& argColor)
	{
		mainColor_ = argColor;
		renderTarget_->CreateSolidColorBrush(argColor, mainBrush_.ReleaseAndGetAddressOf());
	}

	// サブカラー設定
	void SetSubColor(const D2D1_COLOR_F& argColor)
	{
		subColor_ = argColor;
		renderTarget_->CreateSolidColorBrush(argColor, subBrush_.ReleaseAndGetAddressOf());
	}

	// 影カラー設定
	void SetShadowColor(const D2D1_COLOR_F argColor)
	{
		colorShadow_ = argColor;
		renderTarget_->CreateSolidColorBrush(argColor, shadowBrush_.ReleaseAndGetAddressOf());
	}

	// ドロップシャドウのオフセット設定
	void ShadowOffset(float argPosX, float argPosY)
	{
		shadowOffset_ = D2D1::SizeF(argPosX, argPosY);
		shadowMatrix_ = D2D1::Matrix3x2F::Translation(shadowOffset_);
	}

public:
	/**
	 *	@fn			ToTransform
	 *	@brief		回転移動行列
	 *	@param[in]	argAngle	!<	角度
	 *	@param[in]	argCenterX	!<	円座標x
	 *	@param[in]	argCenterY	!<	円座標y
	 *	@param[in]	argPosX		!<	座標x
	 *	@param[in]	argPosY		!<	座標y
	 */
	D2D1_MATRIX_3X2_F ToTransform(float argAngle, float argCenterX, float argCenterY, float argPosX, float argPosY)
	{
		return D2D1::Matrix3x2F::Translation(-argCenterX, -argCenterY) * D2D1::Matrix3x2F::Rotation(Utility::Math::ToDegree(argAngle)) * D2D1::Matrix3x2F::Translation(argPosX, argPosY);
	}

	// 描画
	void Draw(ID2D1RenderTarget* argRt)
	{
		Begin(argRt);

		// 描画開始
		lineWidth_ = 10;

		ShadowOffset(5.f, 5.f);

		SetMainColor(D2D1::ColorF(D2D1::ColorF::LightGreen));
		SetSubColor(D2D1::ColorF(D2D1::ColorF::LightBlue));
		DrawChecker(0.f, 0.f, 40, 23, 32.f, 32.f);

		isShadow_ = true;
		SetMainColor(D2D1::ColorF(D2D1::ColorF::Pink));
		FillArc(0.f, 0.f, 300.f, 300.f, 1.0f, -Utility::Math::PI);
		SetMainColor(D2D1::ColorF(D2D1::ColorF::LightPink));
		DrawArc(0.f, 0.f, 300.f, 300.f, 1.0f, -Utility::Math::PI);

		round_ = D2D1::Size(20.f, 20.f);
		SetMainColor(D2D1::ColorF(D2D1::ColorF::Lime));
		FillRoundRect(100.f, 300.f, 200.f, 300.f);
		SetMainColor(D2D1::ColorF(D2D1::ColorF::LimeGreen));
		DrawRoundRect(100.f, 300.f, 200.f, 300.f);

		SetMainColor(D2D1::ColorF(D2D1::ColorF::LightCyan));
		FillRect(200.f, 200.f, 500.f, 200.f);
		SetMainColor(D2D1::ColorF(D2D1::ColorF::Cyan));
		DrawRect(200.f, 200.f, 500.f, 200.f);

		SetMainColor(D2D1::ColorF(D2D1::ColorF::Lavender));
		FillEllipse(500.f, 200.f, 100.f, 100.f);
		SetMainColor(D2D1::ColorF(D2D1::ColorF::LavenderBlush));
		DrawEllipse(500.f, 200.f, 100.f, 100.f);

		SetMainColor(D2D1::ColorF(D2D1::ColorF::Red));
		DrawLine(30.f, 60.f, 90.f, 120.f);

		{
			D2D1_POINT_2F points[] =
			{
				D2D1::Point2F(400.f,300.f),
				D2D1::Point2F(300.f,400.f),
				D2D1::Point2F(200.f,100.f),
				D2D1::Point2F(100.f,200.f),
				D2D1::Point2F(10.f,50.f),
			};

			SetMainColor(D2D1::ColorF(D2D1::ColorF::DarkBlue));
			DrawLinesOpen(points, 5);
		}

		{

			D2D1_POINT_2F points[] =
			{
				D2D1::Point2F(600.f,300.f),
				D2D1::Point2F(600.f,400.f),
				D2D1::Point2F(400.f,100.f),
				D2D1::Point2F(300.f,200.f),
				D2D1::Point2F(210.f, 50.f),
			};

			SetMainColor(D2D1::ColorF(D2D1::ColorF::DarkCyan));
			DrawLinesClosed(points, 5);
		}

		{
			D2D1_POINT_2F points[] =
			{
				D2D1::Point2F(400.f,400.f),
				D2D1::Point2F(300.f,600.f),
				D2D1::Point2F(500.f,450.f),
				D2D1::Point2F(300.f,450.f),
				D2D1::Point2F(500.f,600.f),
			};
			SetMainColor(D2D1::ColorF(D2D1::ColorF::LightYellow));
			FillLinesClosed(points, 5);
			SetMainColor(D2D1::ColorF(D2D1::ColorF::Yellow));
			DrawLinesClosed(points, 5);
		}

		{
			D2D1_POINT_2F points[] =
			{
				D2D1::Point2F(700.f,200.f),
				D2D1::Point2F(1100.f,200.f),
				D2D1::Point2F(800.f,600.f),
				D2D1::Point2F(1000.f,600.f),
			};
			SetMainColor(D2D1::ColorF(D2D1::ColorF::GreenYellow));
			FillCurve(points, 4);
			SetMainColor(D2D1::ColorF(D2D1::ColorF::YellowGreen));
			DrawCurve(points, 4);
		}

		{
			D2D_POINT_2F points[] =
			{
				D2D1::Point2F(320.f,100.f),
				D2D1::Point2F(300.f,120.f),
				D2D1::Point2F(100.f,120.f),
				D2D1::Point2F(120.f,100.f),
			};
			D2D_POINT_2F points2[] =
			{
				D2D1::Point2F(320.f,100.f),
				D2D1::Point2F(300.f,120.f),
				D2D1::Point2F(100.f,120.f),
				D2D1::Point2F(120.f,100.f),
			};
			points2[0].x = 200.f;
			points2[1].x = 180.f;

			ShadowOffset(3.f, 3.f);
			isShadow_ = true;
			SetMainColor(D2D1::ColorF(D2D1::ColorF::LightGray));
			FillLinesClosed(points, 4);
			isShadow_ = false;
			SetMainColor(D2D1::ColorF(D2D1::ColorF::LightGreen));
			FillLinesClosed(points2, 4);
			isShadow_ = true;
			lineWidth_ = 5.f;
			SetMainColor(D2D1::ColorF(D2D1::ColorF::White));
			DrawLinesClosed(points, 4);
		}


		{
			isShadow_ = true;
			SetMainColor(D2D1::ColorF(D2D1::ColorF::LightGray));
			FillCircle(300.f, 300.f, 50.f);
			isShadow_ = false;
			SetMainColor(D2D1::ColorF(D2D1::ColorF::Red));
			FillArc(300.f, 300.f, 50.f, 0.f, Utility::Math::ToRadian(270));
			SetMainColor(D2D1::ColorF(D2D1::ColorF::White));
			isShadow_ = true;
			FillCircle(300.f, 300.f, 30.f);
			isShadow_ = true;
			DrawCircle(300.f, 300.f, 50.f);
		}

		End();
	}


	// 影描画汎用関数
	template<class T>
	void DrawShadow(T func)
	{
		if (!isShadow_)
			return;

		D2D1_MATRIX_3X2_F old_world;
		renderTarget_->GetTransform(&old_world);
		D2D1_MATRIX_3X2_F world = old_world * shadowMatrix_;
		renderTarget_->SetTransform(world);
		func();
		renderTarget_->SetTransform(old_world);
	}

	/**
	 *	@fn			DrawArc
	 *	@brief		円弧描画（センター指定）
	 *	@param[in]	argCirlceX		!<	中心座標x
	 *	@param[in]	argCirlceY		!<	中心座標y
	 *	@param[in]	argRadius		!<	半径
	 *	@param[in]	argStartAngle	!<	開始角度
	 *	@param[in]	argEndAngle		!<	終了角度角度
	 */
	void DrawArc(float argCircleX, float argCircleY, float argRadius, float argStartAngle, float argEndAngle)
	{
		DrawArc(argCircleX - argRadius, argCircleY - argRadius, argRadius * 2, argRadius * 2, argStartAngle, argEndAngle);
	}

	/**
	 *	@fn			DrawArc
	 *	@brief		円弧描画（サイズ指定）
	 *	@param[in]	argPosX			!<	座標x
	 *	@param[in]	argPosY			!<	座標y
	 *	@param[in]	argWidth		!<	幅
	 *	@param[in]	argHeight		!<	高さ
	 *	@param[in]	argStartAngle	!<	開始角度
	 *	@param[in]	argEndAngle		!<	終了角度角度
	 */
	void DrawArc(float argPosX, float argPosY, float argWidth, float argHeight, float argStartAngle, float argEndAngle)
	{	
		// 半径を求める
		const float RadiusX = argWidth * 0.5f;
		const float RadiusY = argHeight * 0.5f;

		argStartAngle += arcStartAngle_;
		argEndAngle += arcStartAngle_;

		// 角度情報
		const float Angle = argEndAngle - argStartAngle;
		const float AngleAbs = fabsf(Angle);

		if (Angle == 0.0f) 
			return;

		if (AngleAbs < Utility::Math::PI * 2)
		{
			// 360度以下

			// 開始座標
			float sx = cosf(argStartAngle) * RadiusX + argPosX + RadiusX;
			float sy = sinf(argStartAngle) * RadiusY + argPosY + RadiusY;

			// 終了座標
			float ex = cosf(argEndAngle) * RadiusX + argPosX + RadiusX;
			float ey = sinf(argEndAngle) * RadiusY + argPosY + RadiusY;

			// 描画方向
			auto dir = D2D1_SWEEP_DIRECTION_CLOCKWISE;
			if (Angle < 0.0f)
				dir = D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE;

			// 180度より大きいか
			auto arcSize = D2D1_ARC_SIZE_SMALL;
			if (AngleAbs > Utility::Math::PI)
				arcSize = D2D1_ARC_SIZE_LARGE;


			Microsoft::WRL::ComPtr<ID2D1PathGeometry> geometry;
			Microsoft::WRL::ComPtr<ID2D1GeometrySink> sink;
			CreateGeoAndSink(&geometry, &sink);

			// 形状生成開始
			sink->BeginFigure(D2D1::Point2F(sx, sy), D2D1_FIGURE_BEGIN_HOLLOW);

			//D2D1::ArcSegment(終点,半径,楕円の角度,時計周りか、180より大きいか)
			sink->AddArc(D2D1::ArcSegment(D2D1::Point2F(ex, ey), D2D1::SizeF(RadiusX, RadiusY), 0, dir, arcSize));
			// 形状生成終了
			sink->EndFigure(D2D1_FIGURE_END_OPEN);

			// シンククローズ
			sink->Close();

			// 影の描画
			DrawShadow([this, geometry]() { renderTarget_->DrawGeometry(geometry.Get(), shadowBrush_.Get(), lineWidth_); });

			// 描画
			renderTarget_->DrawGeometry(geometry.Get(), mainBrush_.Get(), lineWidth_);

		}
		else
		{
			// 楕円を描画
			renderTarget_->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(argPosX + RadiusX, argPosY + RadiusY), RadiusX, RadiusY), mainBrush_.Get(), 10);
		}
	}

	//  円弧塗りつぶし（センター指定）
	void FillArc(float cx, float cy, float radius, float startAngle, float endAngle)
	{
		FillArc(cx - radius, cy - radius, radius * 2, radius * 2, startAngle, endAngle);
	}

	/**
	 *	@fn			FillArc
	 *	@brief		円弧塗りつぶし（サイズ指定）
	 *	@param[in]	argPosX			!<	x座標
	 *	@param[in]	argPosY			!<	y座標
	 *	@param[in]	argWidth		!<	幅
	 *	@param[in]	argHeight		!<	高さ
	 *	@param[in]	argStartAngle	!<	開始角度
	 *	@param[in]	argEndAngle		!<	終了角度角度
	 */
	void FillArc(float argPosX, float argPosY, float argWidth, float argHeight, float argStartAngle, float argEndAngle)
	{
		// 半径を求める
		const float RadiusX = argWidth * 0.5f;
		const float RadiusY = argHeight * 0.5f;

		argStartAngle += arcStartAngle_;
		argEndAngle += arcStartAngle_;

		// 角度情報
		const float Angle = argEndAngle - argStartAngle;
		const float AngleAbs = fabsf(Angle);

		if (Angle == 0) 
			return;

		if (AngleAbs < Utility::Math::PI * 2)
		{
			// 中心
			const float CenterX = RadiusX + argPosX;
			const float CenterY = RadiusY + argPosY;

			// 開始座標
			const float StartX = cosf(argStartAngle) * RadiusX + argPosX + RadiusX;
			const float StartY = sinf(argStartAngle) * RadiusY + argPosY + RadiusY;

			// 終了座標
			const float EndX = cosf(argEndAngle) * RadiusX + argPosX + RadiusX;
			const float EndY = sinf(argEndAngle) * RadiusY + argPosY + RadiusY;

			Microsoft::WRL::ComPtr<ID2D1PathGeometry> geometry;
			Microsoft::WRL::ComPtr<ID2D1GeometrySink> sink;
			CreateGeoAndSink(&geometry, &sink);


			// 描画向き
			auto dir = D2D1_SWEEP_DIRECTION_CLOCKWISE;
			if (Angle < 0.0f) 
				dir = D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE;

			// 向き
			auto arcSize = D2D1_ARC_SIZE_SMALL;
			if (fabsf(Angle) > Utility::Math::PI) 
				arcSize = D2D1_ARC_SIZE_LARGE;

			// 形状生成開始
			sink->BeginFigure(D2D1::Point2F(CenterX, CenterY), D2D1_FIGURE_BEGIN_FILLED);

			// 中心から、外径への直線
			sink->AddLine(D2D1::Point2F(StartX, StartY));

			// 円弧
			//D2D1::ArcSegment(終点,半径,楕円の角度,時計周りか、180より大きいか)
			/**
			 *	@fn			ArcSegment
			 *	@brief		円弧のセグメントを生成する
			 *	@param[in]	point			!<	終点
			 *	@param[in]	size			!<	半径
			 *	@param[in]	rotationAngle	!<	回転角度
			 *	@param[in]	sweepDirection	!<	回転方向
			 *	@param[in]	arcSize			!<	180より大きいかのフラグ
			 */
			sink->AddArc(D2D1::ArcSegment(D2D1::Point2F(EndX, EndY), D2D1::SizeF(RadiusX, RadiusY), 0, dir, arcSize));

			// 形状整形終了
			sink->EndFigure(D2D1_FIGURE_END_CLOSED);

			// シンククローズ
			sink->Close();

			// 影の描画
			DrawShadow([this, &geometry]() {renderTarget_->FillGeometry(geometry.Get(), shadowBrush_.Get()); });

			// 描画
			renderTarget_->FillGeometry(geometry.Get(), mainBrush_.Get());
		}
		else
		{
			// 描画
			renderTarget_->FillEllipse(D2D1::Ellipse(D2D1::Point2F(argPosX + RadiusX, argPosY + RadiusY), RadiusX, RadiusY), mainBrush_.Get());
		}
	}


	// チェッカー描画
	void DrawChecker(float left, float top, int num_x, int num_y, float width, float height)
	{
		for (int lNumIndex = 0, lNumCount = num_x * num_y; lNumIndex < lNumCount; lNumIndex++)
		{
			const float PosX = float(lNumIndex % num_x) * width + left;
			const float PosY = float(lNumIndex / num_x) * height + top;

			const auto Rect = D2D1::RectF(PosX, PosY, PosX + width, PosY + height);

			ID2D1SolidColorBrush *brush = ((lNumIndex + ((lNumIndex / num_x) % 2)) % 2 == 0) ?
				mainBrush_.Get() :
				subBrush_.Get();

			// 影未対応
			renderTarget_->FillRectangle(Rect, brush);
		}
	}

	/**
	 *	@fn			DrawRoundRect
	 *	@brief		角丸矩形描画
	 *	@param[in]	argLeft		!<	x座標
	 *	@param[in]	argTop		!<	y座標
	 *	@param[in]	argWidth	!<	幅
	 *	@param[in]	argHeight	!<	高さ
	 */
	void DrawRoundRect(float argLeft, float argTop, float argWidth, float argHeight)
	{
		auto rect = D2D1::RoundedRect(D2D1::RectF(argTop, argLeft, argTop + argWidth, argLeft + argHeight), round_.width, round_.height);

		DrawShadow([this, &rect]() {renderTarget_->DrawRoundedRectangle(rect, shadowBrush_.Get(), lineWidth_); });
		renderTarget_->DrawRoundedRectangle(rect, mainBrush_.Get(), lineWidth_);
	}
	/**
	 *	@fn			FillRoundRect
	 *	@brief		角丸矩形塗りつぶし
	 *	@param[in]	argLeft		!<	x座標
	 *	@param[in]	argTop		!<	y座標
	 *	@param[in]	argWidth	!<	幅
	 *	@param[in]	argHeight	!<	高さ
	 */
	void FillRoundRect(float argLeft, float argTop, float argWidth, float argHeight)
	{
		const auto Rect = D2D1::RoundedRect(D2D1::RectF(argTop, argLeft, argTop + argWidth, argLeft + argHeight), round_.width, round_.height);

		DrawShadow([this, &Rect]() {renderTarget_->FillRoundedRectangle(Rect, shadowBrush_.Get()); });
		renderTarget_->FillRoundedRectangle(Rect, mainBrush_.Get());
	}

	/**
	 *	@fn			DrawRect
	 *	@brief		矩形描画
	 *	@param[in]	argLeft		!<	x座標
	 *	@param[in]	argTop		!<	y座標
	 *	@param[in]	argWidth	!<	幅
	 *	@param[in]	argHeight	!<	高さ
	 */
	void DrawRect(float argLeft, float argTop, float argWidth, float argHeight)
	{
		const auto Rect = D2D1::RectF(argLeft, argTop, argLeft + argWidth, argTop + argHeight);

		DrawShadow([this, &Rect] {renderTarget_->DrawRectangle(Rect, shadowBrush_.Get(), lineWidth_); });
		renderTarget_->DrawRectangle(Rect, mainBrush_.Get(), lineWidth_);
	}

	// 矩形塗りつぶし
	//  適応：影、太さ、メインカラー
	void FillRect(float left, float top, float width, float height)
	{
		auto rect = D2D1::RectF(left, top, left + width, top + height);

		DrawShadow([this, &rect] {renderTarget_->FillRectangle(rect, shadowBrush_.Get()); });
		renderTarget_->FillRectangle(rect, mainBrush_.Get());
	}

	/**
	 *	@fn			DrawCircle
	 *	@brief		円描画
	 *	@param[in]	argPoints	!<	塗りつぶす頂点の座標
	 *	@param[in]	argMax		!<	頂点の数
	 */
	void DrawCircle(float argCenterX, float argCenterY, float argRadius)
	{
		const auto Ellipse = D2D1::Ellipse(D2D1::Point2F(argCenterX, argCenterY), argRadius, argRadius);

		DrawShadow([this, &Ellipse] {renderTarget_->DrawEllipse(Ellipse, shadowBrush_.Get(), lineWidth_); });
		renderTarget_->DrawEllipse(Ellipse, mainBrush_.Get(), lineWidth_);
	}
	/**
	 *	@fn			DrawEllipse
	 *	@brief		楕円描画
	 *	@param[in]	argLeft		!<	x座標
	 *	@param[in]	argTop		!<	y座標
	 *	@param[in]	argWidth	!<	幅
	 *	@param[in]	argHeight	!<	高さ
	 */
	void DrawEllipse(float argLeft, float argTop, float argWidth, float argHeight)
	{
		const float RadiusX = argWidth * 0.5f;
		const float RadiusY = argHeight * 0.5f;

		const float CenterX = argLeft + RadiusX;
		const float CenterY = argTop + RadiusY;

		auto ellipse = D2D1::Ellipse(D2D1::Point2F(CenterX, CenterY), RadiusX, RadiusY);

		DrawShadow([this, &ellipse] {renderTarget_->DrawEllipse(ellipse, shadowBrush_.Get(), lineWidth_); });
		renderTarget_->DrawEllipse(ellipse, mainBrush_.Get(), lineWidth_);
	}
	/**
	 *	@fn			FillCircle
	 *	@brief		円塗りつぶし
	 *	@param[in]	argCenterX	!<	中心座標x
	 *	@param[in]	argCenterY	!<	中心座標y
	 *	@param[in]	argRadius	!<	半径
	 */
	void FillCircle(float argCenterX, float argCenterY, float argRadius)
	{
		const auto Ellipse = D2D1::Ellipse(D2D1::Point2F(argCenterX, argCenterY), argRadius, argRadius);

		DrawShadow([this, &Ellipse] {renderTarget_->FillEllipse(Ellipse, shadowBrush_.Get()); });
		renderTarget_->FillEllipse(Ellipse, mainBrush_.Get());
	}
	/**
	 *	@fn			FillEllipse
	 *	@brief		楕円塗りつぶし
	 *	@param[in]	argLeft		!<	x座標
	 *	@param[in]	argTop		!<	y座標
	 *	@param[in]	argWidth	!<	幅
	 *	@param[in]	argHeight	!<	高さ
	 */
	void FillEllipse(float argLeft, float argTop, float argWidth, float argHeight)
	{
		const float RadiusX = argWidth * 0.5f;
		const float RadiusY = argHeight * 0.5f;

		const float CenterX = argLeft + RadiusX;
		const float CenterY = argTop + RadiusY;

		auto ellipse = D2D1::Ellipse(D2D1::Point2F(CenterX, CenterY), RadiusX, RadiusY);
		DrawShadow([this, &ellipse] {renderTarget_->FillEllipse(ellipse, shadowBrush_.Get()); });
		renderTarget_->FillEllipse(ellipse, mainBrush_.Get());
	}

	/**
	 *	@fn			DrawLine
	 *	@brief		直線描画
	 *	@param[in]	argStartX	!<	開始頂点座標x
	 *	@param[in]	argStartY	!<	開始頂点座標y
	 *	@param[in]	argEndX		!<	終了頂点座標x
	 *	@param[in]	argEndY		!<	終了頂点座標y
	 */
	void DrawLine(float argStartX, float argStartY, float argEndX, float argEndY)
	{
		DrawShadow([&] {renderTarget_->DrawLine(D2D1::Point2F(argStartX, argStartY), D2D1::Point2F(argEndX, argEndY), shadowBrush_.Get(), lineWidth_); });
		renderTarget_->DrawLine(D2D1::Point2F(argStartX, argStartY), D2D1::Point2F(argEndX, argEndY), mainBrush_.Get(), lineWidth_);
	}

	/**
	 *	@fn			DrawLinesOpen
	 *	@brief		開いた連続ライン
	 *	@param[in]	argPoints	!<	塗りつぶす頂点の座標
	 *	@param[in]	argMax		!<	頂点の数
	 */
	void DrawLinesOpen(D2D1_POINT_2F *argPoints, int argMax)
	{
		Microsoft::WRL::ComPtr<ID2D1PathGeometry> geometry;
		Microsoft::WRL::ComPtr<ID2D1GeometrySink> sink;
		CreateGeoAndSink(&geometry, &sink);

		sink->BeginFigure(argPoints[0], D2D1_FIGURE_BEGIN_FILLED);

		sink->AddLines(&argPoints[1], argMax - 1);

		sink->EndFigure(D2D1_FIGURE_END_OPEN);

		sink->Close();

		DrawShadow([this, &geometry] {renderTarget_->DrawGeometry(geometry.Get(), shadowBrush_.Get(), lineWidth_); });

		renderTarget_->DrawGeometry(geometry.Get(), mainBrush_.Get(), lineWidth_);
	}
	/**
	 *	@fn			DrawLinesClosed
	 *	@brief		閉じた連続ライン
	 *	@param[in]	argPoints	!<	塗りつぶす頂点の座標
	 *	@param[in]	argMax		!<	頂点の数
	 */
	void DrawLinesClosed(D2D1_POINT_2F *argPoints, int argMax)
	{
		Microsoft::WRL::ComPtr<ID2D1PathGeometry> geometry;
		Microsoft::WRL::ComPtr<ID2D1GeometrySink> sink;
		CreateGeoAndSink(&geometry, &sink);

		sink->BeginFigure(argPoints[0], D2D1_FIGURE_BEGIN_FILLED);

		sink->AddLines(&argPoints[1], argMax - 1);

		sink->EndFigure(D2D1_FIGURE_END_CLOSED);

		sink->Close();

		DrawShadow([this, &geometry] {renderTarget_->DrawGeometry(geometry.Get(), shadowBrush_.Get(), lineWidth_); });

		renderTarget_->DrawGeometry(geometry.Get(), mainBrush_.Get(), lineWidth_);
	}


	/**
	 *	@fn			FillLinesClosed
	 *	@brief		閉じた連続ライン塗りつぶし
	 *	@param[in]	argPoints	!<	塗りつぶす頂点の座標
	 *	@param[in]	argMax		!<	頂点の数
	 */
	void FillLinesClosed(D2D1_POINT_2F *argPoints, int argMax)
	{
		Microsoft::WRL::ComPtr<ID2D1PathGeometry> geometry;
		Microsoft::WRL::ComPtr<ID2D1GeometrySink> sink;
		CreateGeoAndSink(&geometry, &sink);

		sink->BeginFigure(argPoints[0], D2D1_FIGURE_BEGIN_FILLED);

		sink->AddLines(&argPoints[1], argMax - 1);

		sink->EndFigure(D2D1_FIGURE_END_CLOSED);

		sink->Close();

		DrawShadow([this, &geometry] {renderTarget_->FillGeometry(geometry.Get(), shadowBrush_.Get()); });

		renderTarget_->FillGeometry(geometry.Get(), mainBrush_.Get());
	}

	/**
	 *	@fn			DrawCurve
	 *	@brief		カーブライン
	 *	@param[in]	argPoints	!<	塗りつぶす頂点の座標
	 *	@param[in]	argMax		!<	頂点の数
	 */
	void DrawCurve(D2D1_POINT_2F *argPoints, int argMax)
	{
		D2D1_QUADRATIC_BEZIER_SEGMENT b[100] = {};

		for (int lPointIndex = 0; lPointIndex < argMax + 1; lPointIndex++)
		{
			int index = lPointIndex%argMax;
			int next = (lPointIndex + 1) % argMax;

			b[lPointIndex].point1 = argPoints[index];
			b[lPointIndex].point2 = D2D1::Point2F((argPoints[index].x + argPoints[next].x) * 0.5f, (argPoints[index].y + argPoints[next].y) * 0.5f);
		}

		Microsoft::WRL::ComPtr<ID2D1PathGeometry> geometry;
		Microsoft::WRL::ComPtr<ID2D1GeometrySink> sink;
		CreateGeoAndSink(&geometry, &sink);

		sink->BeginFigure(b[0].point2, D2D1_FIGURE_BEGIN_HOLLOW);

		sink->AddQuadraticBeziers(&b[1], argMax);

		sink->EndFigure(D2D1_FIGURE_END_OPEN);

		sink->Close();

		DrawShadow([this, &geometry] {renderTarget_->DrawGeometry(geometry.Get(), shadowBrush_.Get(), lineWidth_); });

		renderTarget_->DrawGeometry(geometry.Get(), mainBrush_.Get(), lineWidth_);
	}

	/**
	 *	@fn			FillCurve
	 *	@brief		カーブ塗りつぶし
	 *	@param[in]	argPoints	!<	塗りつぶす頂点の座標
	 *	@param[in]	argMax		!<	頂点の数
	 */
	void FillCurve(D2D1_POINT_2F *argPoints, int argMax)
	{
		D2D1_QUADRATIC_BEZIER_SEGMENT b[100] = {};

		for (int lPointIndex = 0; lPointIndex < argMax + 1; lPointIndex++)
		{
			int index = lPointIndex % argMax;
			int next = (lPointIndex + 1) % argMax;

			b[lPointIndex].point1 = argPoints[index];
			b[lPointIndex].point2 = D2D1::Point2F((argPoints[index].x + argPoints[next].x) * 0.5f, (argPoints[index].y + argPoints[next].y) * 0.5f);
		}

		Microsoft::WRL::ComPtr<ID2D1PathGeometry> geometry;
		Microsoft::WRL::ComPtr<ID2D1GeometrySink> sink;
		CreateGeoAndSink(&geometry, &sink);

		sink->BeginFigure(b[0].point2, D2D1_FIGURE_BEGIN_FILLED);

		sink->AddQuadraticBeziers(&b[1], argMax);

		sink->EndFigure(D2D1_FIGURE_END_OPEN);

		sink->Close();

		DrawShadow([this, &geometry] {renderTarget_->FillGeometry(geometry.Get(), shadowBrush_.Get()); });
		renderTarget_->FillGeometry(geometry.Get(), mainBrush_.Get());
	}
	

public:
	ID2D1Factory* factory_;				//	!<	D2DFactory
	ID2D1RenderTarget* renderTarget_;	//	!<	D2DRenderTarget

	D2D1_SIZE_F round_;					//	!<	ラウンド半径

	D2D1_COLOR_F mainColor_;			//	!<	メインカラー
	D2D1_COLOR_F subColor_;				//	!<	サブカラー
	D2D1_COLOR_F colorShadow_;			//	!<	ドロップシャドウカラー

	D2D1_SIZE_F shadowOffset_;			//	!<	ドロップシャドウのオフセット
	D2D1_MATRIX_3X2_F shadowMatrix_;	//	!<	ドロップシャドウの変換行列

	float arcStartAngle_;				//	!<	円弧の開始角度
	float lineWidth_;					//	!<	線の太さ
	bool isShadow_;						//	!<	ドロップシャドウの有無

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> mainBrush_;	//	!<	メインブラシ
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> subBrush_;		//	!<	サブブラシ
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> shadowBrush_;	//	!<	ドロップシャドウブラシ

};

Utility::PaintBrush::PaintBrush() 
	: pImpl(std::make_unique<Impl>())
{
}

Utility::PaintBrush::~PaintBrush() = default;

ID2D1Factory* Utility::PaintBrush::GetFactory()
{
	return pImpl->factory_;
}

ID2D1RenderTarget* Utility::PaintBrush::GetRenderTarget()
{
	return pImpl->renderTarget_;
}

void Utility::PaintBrush::Draw(ID2D1RenderTarget* argRt)
{
	pImpl->Begin(argRt);
	Draw();
	pImpl->End();
}

void Utility::PaintBrush::Round(float argWidth, float argHeight)
{
	pImpl->round_ = D2D1::SizeF(argWidth, argHeight);
}

void Utility::PaintBrush::Round(const D2D1_SIZE_F & argSize)
{
	pImpl->round_ = argSize;
}

const D2D1_SIZE_F & Utility::PaintBrush::Round() const
{
	return pImpl->round_;
}

void Utility::PaintBrush::MainColor(const D2D1_COLOR_F & argColor)
{
	pImpl->SetMainColor(argColor);
}

const D2D1_COLOR_F & Utility::PaintBrush::MainColor() const
{
	return pImpl->mainColor_;
}

void Utility::PaintBrush::SubColor(const D2D1_COLOR_F & argColor)
{
	pImpl->SetSubColor(argColor);
}

const D2D1_COLOR_F & Utility::PaintBrush::SubColor() const
{
	return pImpl->subColor_;
}

void Utility::PaintBrush::SetShadowColor(const D2D1_COLOR_F argColor)
{
	pImpl->SetShadowColor(argColor);
}

void Utility::PaintBrush::LineWidth(float argWidth)
{
	pImpl->lineWidth_ = argWidth;
}

float Utility::PaintBrush::LineWidth() const
{
	return pImpl->lineWidth_;
}

void Utility::PaintBrush::ShadowEnable(bool argIsShadow)
{
	pImpl->isShadow_ = argIsShadow;
}

bool Utility::PaintBrush::ShadowEnable() const
{
	return pImpl->isShadow_;
}

void Utility::PaintBrush::ShadowOffset(float argPosX, float argPosY)
{
	pImpl->ShadowOffset(argPosX, argPosY);
}

const D2D_SIZE_F & Utility::PaintBrush::ShadowOffset() const
{
	return pImpl->shadowOffset_;
}

void Utility::PaintBrush::ArcStartAngle(float argAngle)
{
	pImpl->arcStartAngle_ = argAngle;
}

float Utility::PaintBrush::ArcStartAngle() const
{
	return pImpl->arcStartAngle_;
}

/**
 *	@fn			ToTransform
 *	@brief		回転移動行列
 *	@param[in]	argAngle	!<	角度
 *	@param[in]	argCenterX	!<	円座標x
 *	@param[in]	argCenterY	!<	円座標y
 *	@param[in]	argPosX		!<	座標x
 *	@param[in]	argPosY		!<	座標y
 */
D2D1_MATRIX_3X2_F Utility::PaintBrush::ToTransform(float argAngle, float argCenterX, float argCenterY, float argPosX, float argPosY)
{
	return pImpl->ToTransform(argAngle, argCenterX, argCenterY, argPosX, argPosY);
}
/**
 *	@fn			DrawArc
 *	@brief		円弧描画（センター指定）
 *	@param[in]	argCirlceX		!<	中心座標x
 *	@param[in]	argCirlceY		!<	中心座標y
 *	@param[in]	argRadius		!<	半径
 *	@param[in]	argStartAngle	!<	開始角度
 *	@param[in]	argEndAngle		!<	終了角度角度
 */
void Utility::PaintBrush::DrawArc(float argCircleX, float argCircleY, float argRadius, float argStartAngle, float argEndAngle)
{
	pImpl->DrawArc(argCircleX, argCircleY, argRadius, argStartAngle, argEndAngle);
}
/**
 *	@fn			DrawArc
 *	@brief		円弧描画（サイズ指定）
 *	@param[in]	argPosX			!<	座標x
 *	@param[in]	argPosY			!<	座標y
 *	@param[in]	argWidth		!<	幅
 *	@param[in]	argHeight		!<	高さ
 *	@param[in]	argStartAngle	!<	開始角度
 *	@param[in]	argEndAngle		!<	終了角度角度
 */
void Utility::PaintBrush::DrawArc(float argPosX, float argPosY, float argWidth, float argHeight, float argStartAngle, float argEndAngle)
{
	pImpl->DrawArc(argPosX, argPosY, argWidth, argHeight, argStartAngle, argEndAngle);
}
/**
 *	@fn			FillArc
 *	@brief		円弧塗りつぶし（センター指定）
 *	@param[in]	argCirlceX		!<	中心座標x
 *	@param[in]	argCirlceY		!<	中心座標y
 *	@param[in]	argRadius		!<	半径
 *	@param[in]	argStartAngle	!<	開始角度
 *	@param[in]	argEndAngle		!<	終了角度角度
 */
void Utility::PaintBrush::FillArc(float argCirlceX, float argCirlceY, float argRadius, float argStartAngle, float argEndAngle)
{
	pImpl->FillArc(argCirlceX, argCirlceY, argRadius, argStartAngle, argEndAngle);
}
/**
 *	@fn			FillArc
 *	@brief		円弧塗りつぶし（サイズ指定）
 *	@param[in]	argPosX			!<	x座標
 *	@param[in]	argPosY			!<	y座標
 *	@param[in]	argWidth		!<	幅
 *	@param[in]	argHeight		!<	高さ
 *	@param[in]	argStartAngle	!<	開始角度
 *	@param[in]	argEndAngle		!<	終了角度角度
 */
void Utility::PaintBrush::FillArc(float argPosX, float argPosY, float argWidth, float argHeight, float argStartAngle, float argEndAngle)
{
	pImpl->FillArc(argPosX, argPosY, argWidth, argHeight, argStartAngle, argEndAngle);
}
/**
 *	@fn			DrawChecker
 *	@brief		チェッカー描画
 *	@param[in]	argLeft		!<	x座標
 *	@param[in]	argTop		!<	y座標
 *	@param[in]	argNumX		!<
 *	@param[in]	argNumY		!<
 *	@param[in]	argWidth	!<	幅
 *	@param[in]	argHeight	!<	高さ
 */
void Utility::PaintBrush::DrawChecker(float argLeft, float argTop, int argNumX, int argNumY, float argWidth, float argHeight)
{
	pImpl->DrawChecker(argLeft, argTop, argNumX, argNumY, argWidth, argHeight);
}
/**
 *	@fn			DrawRoundRect
 *	@brief		角丸矩形描画
 *	@param[in]	argLeft		!<	x座標
 *	@param[in]	argTop		!<	y座標
 *	@param[in]	argWidth	!<	幅
 *	@param[in]	argHeight	!<	高さ
 */
void Utility::PaintBrush::DrawRoundRect(float argLeft, float argTop, float argWidth, float argHeight)
{
	pImpl->DrawRoundRect(argLeft, argTop, argWidth, argHeight);
}
/**
 *	@fn			FillRoundRect
 *	@brief		角丸矩形塗りつぶし
 *	@param[in]	argLeft		!<	x座標
 *	@param[in]	argTop		!<	y座標
 *	@param[in]	argWidth	!<	幅
 *	@param[in]	argHeight	!<	高さ
 */
void Utility::PaintBrush::FillRoundRect(float argLeft, float argTop, float argWidth, float argHeight)
{
	pImpl->FillRoundRect(argLeft, argTop, argWidth, argHeight);
}
/**
 *	@fn			DrawRect
 *	@brief		矩形描画
 *	@param[in]	argLeft		!<	x座標
 *	@param[in]	argTop		!<	y座標
 *	@param[in]	argWidth	!<	幅
 *	@param[in]	argHeight	!<	高さ
 */
void Utility::PaintBrush::DrawRect(float argLeft, float argTop, float argWidth, float argHeight)
{
	pImpl->DrawRect(argLeft, argTop, argWidth, argHeight);
}
/**
 *	@fn			FillRect
 *	@brief		矩形塗りつぶし
 *	@param[in]	argLeft		!<	x座標
 *	@param[in]	argTop		!<	y座標
 *	@param[in]	argWidth	!<	幅
 *	@param[in]	argHeight	!<	高さ
 */
void Utility::PaintBrush::FillRect(float argLeft, float argTop, float argWidth, float argHeight)
{
	pImpl->FillRect(argLeft, argTop, argWidth, argHeight);
}

/**
 *	@fn			DrawCircle
 *	@brief		円描画
 *	@param[in]	argPoints	!<	塗りつぶす頂点の座標
 *	@param[in]	argMax		!<	頂点の数
 */
void Utility::PaintBrush::DrawCircle(float argCenterX, float argCenterY, float argRadius)
{
	pImpl->DrawCircle(argCenterX, argCenterY, argRadius);
}
/**
 *	@fn			DrawEllipse
 *	@brief		楕円描画
 *	@param[in]	argLeft		!<	x座標
 *	@param[in]	argTop		!<	y座標
 *	@param[in]	argWidth	!<	幅
 *	@param[in]	argHeight	!<	高さ
 */
void Utility::PaintBrush::DrawEllipse(float argLeft, float argTop, float argWidth, float argHeight)
{
	pImpl->DrawEllipse(argLeft, argTop, argWidth, argHeight);
}
/**
 *	@fn			FillCircle
 *	@brief		円塗りつぶし
 *	@param[in]	argCenterX	!<	中心座標x
 *	@param[in]	argCenterY	!<	中心座標y
 *	@param[in]	argRadius	!<	半径
 */
void Utility::PaintBrush::FillCircle(float argCenterX, float argCenterY, float argRadius)
{
	pImpl->FillCircle(argCenterX, argCenterY, argRadius);
}
/**
 *	@fn			FillEllipse
 *	@brief		楕円塗りつぶし
 *	@param[in]	argLeft		!<	x座標
 *	@param[in]	argTop		!<	y座標
 *	@param[in]	argWidth	!<	幅
 *	@param[in]	argHeight	!<	高さ
 */
void Utility::PaintBrush::FillEllipse(float argLeft, float argTop, float argWidth, float argHeight)
{
	pImpl->FillEllipse(argLeft, argTop, argWidth, argHeight);
}
/**
 *	@fn			DrawLine
 *	@brief		直線描画
 *	@param[in]	argStartX	!<	開始頂点座標x
 *	@param[in]	argStartY	!<	開始頂点座標y
 *	@param[in]	argEndX		!<	終了頂点座標x
 *	@param[in]	argEndY		!<	終了頂点座標y
 */
void Utility::PaintBrush::DrawLine(float argStartX, float argStartY, float argEndX, float argEndY)
{
	pImpl->DrawLine(argStartX, argStartY, argEndX, argEndY);
}
/**
 *	@fn			DrawLinesOpen
 *	@brief		開いた連続ライン
 *	@param[in]	argPoints	!<	塗りつぶす頂点の座標
 *	@param[in]	argMax		!<	頂点の数
 */
void Utility::PaintBrush::DrawLinesOpen(D2D1_POINT_2F * argPoints, int argMax)
{
	pImpl->DrawLinesOpen(argPoints, argMax);
}
/**
 *	@fn			DrawLinesClosed
 *	@brief		閉じた連続ライン
 *	@param[in]	argPoints	!<	塗りつぶす頂点の座標
 *	@param[in]	argMax		!<	頂点の数
 */
void Utility::PaintBrush::DrawLinesClosed(D2D1_POINT_2F * argPoints, int argMax)
{
	pImpl->DrawLinesClosed(argPoints, argMax);
}
/**
 *	@fn			FillLinesClosed
 *	@brief		閉じた連続ライン塗りつぶし
 *	@param[in]	argPoints	!<	塗りつぶす頂点の座標
 *	@param[in]	argMax		!<	頂点の数
 */
void Utility::PaintBrush::FillLinesClosed(D2D1_POINT_2F * argPoints, int argMax)
{
	pImpl->FillLinesClosed(argPoints, argMax);
}
/**
 *	@fn			DrawCurve
 *	@brief		カーブライン
 *	@param[in]	argPoints	!<	塗りつぶす頂点の座標
 *	@param[in]	argMax		!<	頂点の数
 */
void Utility::PaintBrush::DrawCurve(D2D1_POINT_2F * argPoints, int argMax)
{
	pImpl->DrawCurve(argPoints, argMax);
}
/**
 *	@fn			FillCurve
 *	@brief		カーブ塗りつぶし
 *	@param[in]	argPoints	!<	塗りつぶす頂点の座標
 *	@param[in]	argMax		!<	頂点の数
 */
void Utility::PaintBrush::FillCurve(D2D1_POINT_2F * argPoints, int argMax)
{
	pImpl->FillCurve(argPoints, argMax);
}
