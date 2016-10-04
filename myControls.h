#ifndef ControlDataS_H
#define ControlDataS_H

#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <atlimage.h>
#include <commctrl.h>

#include "Drawing.h"
#include "FunkcjeSprawdzajace.h"
#include "myMath.h"
#include "Text.h"

#define SET_MINVALUE	5000
#define SET_MAXVALUE	5001
#define SET_POSITION	5002
#define SET_DEVIDE		5006
#define SET_BKCOLOR		5008

#define GET_MINVALUE	5003
#define GET_MAXVALUE	5004
#define GET_POSITION	5005
#define GET_DEVIDE		5007
#define GET_BKCOLOR		5009

#define GET_VALUE		5010

#define MCB_ADD_ITEM	5011

#pragma region Functions headers
	void InitMyControls(HINSTANCE hInstance);

	LRESULT CALLBACK NumericProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);

	HWND CreateNumericWindow(HWND parent,HINSTANCE hInstance,LPSTR title,int id,int xPos,int yPos,int width,int height);

#pragma endregion

#pragma region Klasy obs³ugi kontrolek

	#pragma region ControlData - klasa po której ka¿da klasa kontrolki dziedziczy
		class ControlData
		{
		protected:
			POINT MousePos;
			HBITMAP hBitmap;
			RECT wndSize;
		public:
			bool enable;
			COLORREF bkColor;
			COLORREF textColor;
			bool transparent;
			ControlData(HWND hWnd);
			ControlData(HWND hWnd,COLORREF bkColor,COLORREF textColor);
			~ControlData();
			void SetMousePos(HWND hWnd);
			void SetMousePos(LPARAM lParam);
			void SetWndSize(HWND hWnd);
			POINT GetClientMousePos();
			POINT GetScreenMousePos();
			bool Enable();
			void Enable(bool enable);
			void Enable(HWND hWnd, bool enable);
			int Left();
			int Right();
			int Top();
			int Bottom();
			RECT WndSize();
		};
	#pragma endregion

	#pragma region Klasy kontrolki numerycznej
		class NumericWndData : public ControlData, public SprFu
		{
		private:
			double minValue;
			double maxValue;
			double value;
			double devide;
			POINT startPointCircle;
			void PaintUpButton(HDC &hdc,Drawing *RYS);
			void PaintDownButton(HDC &hdc,Drawing *RYS);
			void PaintPath(HDC &hdc,Drawing *RYS);
			void PaintCircle(HDC &hdc,Drawing *RYS);
			int GetCirclePos();
			int idSel; //0 -nic; 1 - tekst; 2 - botton up; 3 - button down; 4 - suwak; 5 - œcieszka
			int idClicked; //0 - nic; 1 - tekst; 2 - button up; 3 - button down; 4 - suwak; 5 - œcie¿ka
		public:
			COLORREF bkTextColor;
			COLORREF frameColor;
			COLORREF bkButtonColor;
			COLORREF ButtonSelect;
			COLORREF ButtonClicked;
			COLORREF PathSelected;
			NumericWndData(HWND hWnd,COLORREF bkColor,COLORREF textColor,double minValue,double maxValue,double value,double devide);
			RECT GetUpButtonRect();
			RECT GetDownButtonRect();
			RECT GetTextRect();
			RECT GetPathRect();
			RECT GetPathWideRect();
			double MinValue();
			void MinValue(double minValue);
			double MaxValue();
			void MaxValue(double maxValue);
			double Value();
			void Value(double value);
			double Devide();
			void Devide(double devide);
			double MinDValue();
			void SetWndSize(HWND hWnd);
			void WmTimer(HWND hWnd);
			void WmLButtonDown(HWND hWnd);
			void WmLButtonUp(HWND hWnd);
			void WmMouseMove(HWND hWnd);
			bool WmKeyDown(HWND hWnd,WPARAM wParam);
			void WmPaint(HDC &hdc,Drawing *RYS);
			void WmEnable(HWND hWnd);
		};

		NumericWndData* SetNumericWnd(HWND hNumericWnd,double minValue,double maxValue, double devide,double value,bool enable,bool refresh);

		class NumericControl{
		private:
			HWND hNumericWndData;
			NumericWndData *nw;
		public:
			// Konstruktory
			NumericControl(HWND hNumericWndData);
			NumericControl();
			NumericControl(HWND parent,HINSTANCE hInstance,LPSTR title,int id,int xPos,int yPos,int width,int height);
			NumericControl(HWND parent,HINSTANCE hInstance,LPSTR title,int id,int xPos,int yPos,int width,int height,NumericWndData *nw);
			// Destruktor
			~NumericControl();
			// Metody
			bool CreateWnd(HWND parent,HINSTANCE hInstance,LPSTR title,int id,int xPos,int yPos,int width,int height);
			void InvalidateWindow();
			void InvalidateWindow(LPRECT lpRect,bool bErase);
			void DestroyWnd();
			// Metody ustawiaj¹ce
			void SetMaxValue(double MaxValue);
			void SetMinValue(double MinValue);
			void SetValue(double Value);
			void SetDevide(double Devide);
			void SetBkColor(COLORREF bkColor);
			void SetBkButtonColor(COLORREF bkButtonColor);
			void SetBkTextColor(COLORREF bkTextColor);
			void SetTextColor(COLORREF bkTextColor);
			void SetEnable(bool e);
			// Metody pobieraj¹ce
			double GetMaxValue();
			double GetMinValue();
			double GetValue();
			double GetDevide();
			COLORREF GetBkColor();
			COLORREF GetBkButtonColor();
			COLORREF GetBkTextColor();
			COLORREF GetTextColor();
		};
	#pragma endregion

#endif