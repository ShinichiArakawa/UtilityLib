/**
 *	@file	Task.h
 *	@date	2017 / 7 / 20
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

namespace Utility
{
	/**
	 *	 @enum	ePriority
	 *	 @brief	タスクの描画優先度
	 */
	enum class ePriority
	{
		BackGround,			//	!<	背景
		GameObjectBack,		//	!<	オブジェクト
		GameObjectFoword,	//	!<	オブジェクト
		Efect,				//	!<	演出
		Ui,					//	!<	UI
	};

	/**
	 *	@enum	eDrawMode
	 *	@brief	タスクの描画モード
	 */
	enum class eDrawMode
	{
		Solid,	//	!<	立体
		Add,	//	!<	加算合成
		Alpha,	//	!<	透過
	};

	class Task abstract
	{
	private:
		bool isDelay_;
	protected:
		ePriority priority_;
		eDrawMode drawMode_;

	public:

		//Task() 
		//	: drawMode_(eDrawMode::Alpha), priority_(ePriority::GameObject), isDelay_(false)
		//{
		//}
		Task(bool argIsDelay)
			: drawMode_(eDrawMode::Alpha), priority_(ePriority::GameObjectBack), isDelay_(argIsDelay)
		{
		}
		virtual ~Task() = default;

	public:
		virtual bool Update() = 0;
		virtual void Draw() = 0;

	public:
		inline ePriority Priority()const { return priority_; }
		inline eDrawMode DrawModeState()const { return drawMode_; }
		inline bool IsDelay()const { return isDelay_; }
#pragma region Operator	//	!<	描画優先度ソート用
	public:
		inline bool operator<(const Task &argTask) const
		{
			return (priority_ < argTask.priority_);
		}
		inline bool operator>(const Task &argTask) const
		{
			return (priority_ > argTask.priority_);
		}
		inline bool operator>=(const Task &argTask) const
		{
			return (priority_ >= argTask.priority_);
		}
		inline bool operator<=(const Task &argTask) const
		{
			return (priority_ <= argTask.priority_);
		}
#pragma endregion
	};
};