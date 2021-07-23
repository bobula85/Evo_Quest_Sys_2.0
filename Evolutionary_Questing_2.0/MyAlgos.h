#pragma once


template<class InputIt, class T>
constexpr InputIt findEqualOrGreater(InputIt first, InputIt last, const T& value)
{
	for (; first != last; ++first) {
		if (*first >= value) {
			return first;
		}
	}
	return last;
}


template<class InputIt, class T, class P>
constexpr InputIt findEqualOrGreaterComp(InputIt first, InputIt last, const T& value, const	P& compPtr)
{
	for (; first != last; ++first) {
		if (*first >= value && *first != *compPtr) {
			return first;
		}
	}
	return last;
}