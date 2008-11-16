/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "DemoKeeper.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <assert.h>

#include <gcroot.h>



namespace demo
{

	public value struct IntCoord
	{
	public:
		IntCoord(int _left, int _top, int _width, int _height)
		{
			left = _left;
			top = _top;
			width = _width;
			height = _height;
		}
		static operator IntCoord (MyGUI::IntCoord _coord) { return IntCoord(_coord.left, _coord.top, _coord.width, _coord.height); }
		static operator MyGUI::IntCoord (IntCoord _coord) { return MyGUI::IntCoord(_coord.left, _coord.top, _coord.width, _coord.height); }

		int left, top, width, height;
	};

	public value struct Align
	{
		Align(int _align)
		{
			align = _align;
		}
		static operator Align (MyGUI::Align _align) { return Align(_align.align); }
		static operator MyGUI::Align (Align _align) { return MyGUI::Align(_align.align); }
		int align;
	};

	public ref class Widget
	{
	public:

		Widget(Widget ^ _parent, System::String ^ _skin, IntCoord _coord, Align _align, System::String ^ _layer, System::String ^ _name)
		{
			if (_parent == nullptr) {
				m_widget = MyGUI::Gui::getInstance().createWidget<MyGUI::Widget>("Button", _coord, MyGUI::Align::Default, "Main");
			}
			else {
				_parent->GetWidget()->createWidget<MyGUI::Widget>("Button", _coord, MyGUI::Align::Default);
			}
			m_widgetsList.Add(this);
		}

		void Destroy()
		{
			if (m_widget == null) throw gcnew System::Exception("widget is destroy");
			MyGUI::Gui::getInstance().destroyWidget(m_widget);
			m_widgetsList.Remove(this);
		}

		~Widget()
		{
		}

		MyGUI::WidgetPtr GetWidget() { return m_widget; }

	private:
		static System::Collections::Generic::List<Widget^> m_widgetsList = gcnew System::Collections::Generic::List<Widget^>();
		MyGUI::WidgetPtr m_widget;
	};

	struct Param { };

	template <typename T1>
	class StaticManagedDelegate1  : public MyGUI::delegates::IDelegate1<T1>
	{
	public:
		typedef void (*Func)(T1);

		StaticManagedDelegate1(System::MulticastDelegate ^ _delegate)
		{
			if (_delegate->Target != nullptr) throw gcnew System::Exception("only for static method");
			m_delegate = _delegate;
			System::Reflection::MethodInfo^ mInfo = m_delegate->GetType()->GetMethod(
				"Invoke", gcnew cli::array<System::Type^> {T1::typeid });
			if (mInfo == nullptr) throw gcnew System::Exception("error signature");
			mFunc = (Func)System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(m_delegate).ToPointer();
		}

		virtual void invoke(T1 p1)
		{
			mFunc(p1);
		}

		virtual bool compare(MyGUI::delegates::IDelegate1<T1>* _delegate)
		{
			if (!_delegate || typeid(*this) != typeid(*_delegate)) return false;
			return (static_cast<StaticManagedDelegate1<T1>*>(_delegate)->mFunc != mFunc);
		}
	private:
		gcroot<System::MulticastDelegate^> m_delegate;
		Func mFunc;
	};

	delegate void HandleDelegate(Param _value);



	value struct Export
	{

		static void foo(Param _value)
		{
			int test = 0;
		}

	};

    void DemoKeeper::createScene()
    {
		MyGUI::helper::addResourceLocation("D:/MyGUI_Source/trunk/Media/TestApp", "FileSystem", "General", false, false);
		MyGUI::helper::addResourceLocation("D:/MyGUI_Source/trunk/Media/TestApp/2.zip", "Zip", "General", false, false);

		MyGUI::delegates::CDelegate1<Param> eventTest;
		eventTest = new StaticManagedDelegate1<Param>(gcnew HandleDelegate(Export::foo));
		eventTest(Param());

		
		Widget ^ widget = gcnew Widget(nullptr, "", IntCoord(10 ,10 , 100, 100), Align(MyGUI::Align::Default), "", "");

	}
 
    void DemoKeeper::destroyScene()
    {
    }

	 
} // namespace demo
