/**
 *	@file	Encode.h
 *	@date	2017 / 10 / 08
 *	@author	Katsumi Takei
 *	Copyright (c) Kastumi Takei. All rights reserved.
 */
#pragma once
#include <algorithm>

namespace Utility
{
	class Encode
	{
	private:
		int key_;
	public:
		explicit Encode(int key)
			: key_(key) 
		{
		}
	public:
		template<typename Range> Range
		operator()(Range range) const
		{//	引数の値とキーを排他的論理和で暗号化する
			using ValueType = typename std::iterator_traits<decltype(std::begin(range))>::value_type;
			auto result = std::move(range);
			std::transform(std::begin(result), std::end(result), std::begin(result),
				[this](ValueType const& value) {return value ^ this->key_; });
			return result;
		}
	};
}