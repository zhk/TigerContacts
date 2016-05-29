/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			Callback.h
*	Description:
*			Modified from MaNGOS.
*			
*	Revision history:
*			2010-8-3	* Created by Zhang Kun
*				Created
===============================================================================*/
#pragma once

#include "PreInclude.h"

namespace Cactus
{
	//使用类成员函数作为回调函数，请注意类对象的生存期
	template < class T, typename PT1 = void, typename PT2 = void>
	class _Callback
	{
	protected:
		typedef void (T::*Method)(PT1, PT2);
		T*		_object;
		Method	_method;
		PT1		_param1;
		PT2		_param2;
		void	_Execute() { (_object->*_method)(_param1, _param2); }
	public:
		_Callback(T* object, Method m, PT1 param1, PT2 param2)
			: _object(object), _method(m), _param1(param1), _param2(param2) {}
		_Callback(_Callback < T, PT1, PT2 > const& cb)
			: _object(cb._object), _method(cb._method), _param1(cb._param1), _param2(cb._param2) {}
	};

	template < class T, typename PT1>
	class _Callback < T, PT1>
	{
	protected:
		typedef void (T::*Method)(PT1);
		T*		_object;
		Method	_method;
		PT1		_param1;
		void	_Execute() { (_object->*_method)(_param1); }
	public:
		_Callback(T* object, Method m, PT1 param1)
			: _object(object), _method(m), _param1(param1) {}
		_Callback(_Callback < T, PT1 > const& cb)
			: _object(cb._object), _method(cb._method), _param1(cb._param1) {}
	};

	template < class T >
	class _Callback < T >
	{
	protected:
		typedef void (T::*Method)();
		T*		_object;
		Method	_method;
		void	_Execute() { (_object->*_method)(); }
	public:
		_Callback(T *object, Method m)
			: _object(object), _method(m) {}
		_Callback(_Callback < T > const& cb)
			: _object(cb._object), _method(cb._method) {}
	};

	/// ---- Statics ----使用全局或静态函数作为回调函数
	template < typename PT1 = void, typename PT2 = void>
	class _SCallback
	{
	protected:
		typedef void (*Method)(PT1, PT2);
		Method	_method;
		PT1		_param1;
		PT2		_param2;
		void	_Execute() { (*_method)(_param1, _param2); }
	public:
		_SCallback(Method m, PT1 param1, PT2 param2)
			: _method(m), _param1(param1), _param2(param2) {}
		_SCallback(_SCallback < PT1, PT2 > const& cb)
			: _method(cb._method), _param1(cb._param1), _param2(cb._param2) {}
	};

	template < typename PT1>
	class _SCallback < PT1 >
	{
	protected:
		typedef void (*Method)(PT1);
		Method	_method;
		PT1		_param1;
		void	_Execute() { (*_method)(_param1); }
	public:
		_SCallback(Method m, PT1 param1)
			: _method(m), _param1(param1) {}
		_SCallback(_SCallback < PT1 > const& cb)
			: _method(cb._method), _param1(cb._param1) {}
	};

	template < >
	class _SCallback < >
	{
	protected:
		typedef void (*Method)();
		Method	_method;
		void	_Execute() { (*_method)(); }
	public:
		_SCallback(Method m)
			: _method(m) {}
		_SCallback(_SCallback <> const& cb)
			: _method(cb._method) {}
	};


	// --------- GENERIC CALLBACKS ----------
	class ICallback : public Cactus::Object
	{
	public:
		virtual void Execute() = 0;
		virtual ~ICallback() {}
	};

	template < class CB >
	class _ICallback : public CB, public ICallback
	{
	public:
		_ICallback(CB const& cb) : CB(cb) {}
		void Execute() { CB::_Execute(); }
	};

	template < class T, typename PT1 = void, typename PT2 = void >
	class Callback : public _ICallback< _Callback < T, PT1, PT2 > >
	{
	private:
		typedef _Callback < T, PT1, PT2 > C2;
	public:
		Callback(T *object, typename C2::Method m, PT1 param1, PT2 param2)
			: _ICallback< C2 >(C2(object, m, param1, param2)) {}
	};

	template < class T, typename PT1>
	class Callback < T, PT1> : public _ICallback< _Callback < T, PT1 > >
	{
	private:
		typedef _Callback < T, PT1 > C1;
	public:
		Callback(T *object, typename C1::Method m, PT1 param1)
			: _ICallback< C1 >(C1(object, m, param1)) {}
	};

	template < class T >
	class Callback < T > : public _ICallback< _Callback < T > >
	{
	private:
		typedef _Callback < T > C0;
	public:
		Callback(T *object, typename C0::Method m)
			: _ICallback< C0 >(C0(object, m)) {}
	};
}
