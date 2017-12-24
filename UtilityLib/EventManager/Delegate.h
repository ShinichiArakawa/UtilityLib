/**
 *	@file	Delegate.h
 *	@date	2017 / 12 / 21
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once

#include <vector>
#include <functional>
#include <assert.h>

namespace Utility
{
	template <class>
	class Delegate {};

	template <class Ret, class... Args>
	class Delegate <Ret(Args...)>
	{
	private:
		using KeyPair = std::pair <void*, void*>;
		struct Value
		{
			std::function <Ret(Args...)> method;
			KeyPair keys;
		};
		/**
		 *	@fn			Find
		 *	@brief		引数のメソッドが登録されているか探す
		 *	@param[in]	argMethod	!<		検索したいメソッド
		 *	@retval		引数のメソッド
		 *	@retval		nullptr
		 */
		auto Find(void* argMethod)
		{
			KeyPair arg{ argMethod, nullptr };
			return std::find_if(methods_.begin(), methods_.end(), [&](Value& argVal) { return (argVal.keys == arg); });
		}

	private:
		std::vector <Value> methods_;

#pragma region		operators
		
#pragma region		Add

	public:
		/**
		 *	@fn			operator+=
		 *	@brief		関数ポインタ用オペレーター
		 *	@param[in]	argMethod	!<	登録したい関数
		 */
		template <class T>
		void operator+= (T* argMethod)
		{
			if (Find(argMethod) != methods_.end())
				return;
			methods_.push_back({ [=](Args... args) -> Ret {
				return argMethod(std::forward <Args>(args)...); }, KeyPair(argMethod, nullptr) 
			});
		}
		/**
		 *	@fn			operator+=
		 *	@brief		関数オブジェクト用オペレーター（左辺値）
		 *	@param[in]	argMethod	!<	登録したい関数
		 */
		template <class T>
		void operator+= (T& argMethod)
		{
			if (Find(&argMethod) != methods_.end())
				return;
			methods_.push_back({ [&](Args... args) -> Ret {
				return argMethod(std::forward <Args>(args)...); }, KeyPair(&argMethod, nullptr) 
			});
		}
		/**
		 *	@fn			operator+=
		 *	@brief		関数オブジェクト用オペレーター（右辺値）
		 *	@param[in]	argMethod	!<	登録したい関数
		 */
		template <class T>
		void operator+= (T&& argMethod)
		{
			methods_.push_back({ [argMethod = std::move(argMethod)](Args... args) mutable->Ret { 
				return argMethod(std::forward <Args>(args)...); }, KeyPair(nullptr, nullptr)
			});
		}
#pragma endregion	Add


#pragma region		Remove

	public:
		/**
		 *	@fn			operator-=
		 *	@brief		関数ポインタ用オペレーター
		 *	@param[in]	argMethod	!<	削除したい関数
		 */
		template <class T>
		void operator-=(T* argMethod)
		{
			auto itr = Find(argMethod);
			if (itr != methods_.end())
				methods_.erase(itr);
		}
		/**
		 *	@fn			operator-=
		 *	@brief		関数オブジェクト用オペレーター（左辺値）
		 *	@param[in]	argMethod	!<	削除したい関数
		 */
		template <class T>
		void operator-=(T& argMethod)
		{
			auto itr = Find(&argMethod);
			if (itr != methods_.end())
				methods_.erase(itr);
		}
#pragma endregion	Remove

#pragma region		Substitution
	public:
		/**
		 *	@fn			operator=
		 *	@brief		関数ポインタ用オペレーター
		 *	@param[in]	argMethod	!<	登録したい関数
		 */
		template <class T>
		void operator= (T* argMethod)
		{
			methods_.clear();
			methods_.push_back({ [=](Args... args) -> Ret { 
				return argMethod(std::forward <Args>(args)...); }, KeyPair(argMethod, nullptr) 
			});
		}
		/**
		 *	@fn			operator=
		 *	@brief		関数ポインタ用オペレーター
		 *	@param[in]	argMethod	!<	登録したい関数（左辺値）
		 */
		template <class T>
		void operator= (T& argMethod)
		{
			methods_.clear();
			methods_.push_back({ [&](Args... args) -> Ret { 
				return argMethod(std::forward <Args>(args)...); }, KeyPair(&argMethod, nullptr) 
			});
		}
		/**
		 *	@fn			operator=
		 *	@brief		関数ポインタ用オペレーター
		 *	@param[in]	argMethod	!<	登録したい関数（右辺値）
		 */
		template <class T>
		void operator= (T&& argMethod)
		{
			methods_.clear();
			methods_.push_back({ [argMethod = std::move(argMethod)](Args... args) mutable->Ret { 
				return argMethod(std::forward <Args>(args)...); }, KeyPair(nullptr, nullptr)
			});
		}

#pragma endregion	Substitution
		/**
		 *	@fn			operato()
		 *	@brief		登録関数呼び出し用オペレーター
		 *	@param[in]	args	!<	登録した関数の引数
		 */
		void operator() (Args... args) const
		{
			assert(methods_.size() > 0 && "delegate methods is not founds...");
			for (auto&& elem : methods_)
				elem.method((args)...);
		}
#pragma endregion	operators
	};

}