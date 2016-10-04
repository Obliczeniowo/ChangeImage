#include "document.h"

extern Drawing *RYS;

extern HWND mdiClient;

extern HFONT textFont;

extern HBRUSH	brGray150;
extern HBRUSH	brGray230;
extern HBRUSH	brRed;

extern HPEN		pnGray200;
extern HPEN		pnRed;

#pragma region Procedura ImageWindowProc

LRESULT CALLBACK ImageWindowProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam){
	static COLORREF kTla=RGB(0,0,0);
	static POINT MousePos;
	static UINT MK;
	switch(msg){
		case WM_SETTINGCHANGE:
			{
				Document *doc = (Document*) GetWindowLongPtr(GetParent(hWnd), GWL_USERDATA);
				SystemParametersInfo (SPI_GETWHEELSCROLLLINES, 0, &doc->ulScrollLines, 0);
		        if (doc->ulScrollLines)
					doc->iDeltaPerLine = WHEEL_DELTA / doc->ulScrollLines ;
				else
					doc->iDeltaPerLine = 0 ;
			}
			break;
		case WM_LBUTTONDBLCLK:
			{
				Document *doc = (Document*) GetWindowLongPtr(GetParent(hWnd), GWL_USERDATA);
				doc->WmDblLeftClk((int)wParam);
			}
			break;
		case WM_KEYDOWN:
			{
				switch(wParam){
					case VK_UP:
						{

						}
						break;
					case VK_DOWN:
						{

						}
						break;
					case VK_LEFT:
						{

						}
						break;
					case VK_RIGHT:
						{

						}
						break;
				}
			}
			break;
		case WM_HSCROLL:
			{
				SCROLLINFO si;
				si.cbSize		= sizeof(SCROLLINFO);
				si.fMask		= SIF_ALL;

				GetScrollInfo(hWnd,SB_HORZ,&si);

				si.fMask		= SIF_POS;
				
				switch(LOWORD(wParam)){
					case SB_ENDSCROLL:
						break;
					case SB_LEFT:

						break;
					case SB_RIGHT:
						break;
					case SB_LINELEFT:
						{
							if((int)si.nPos - 1 < 0){
								si.nPos	= 0;
							}else{
								si.nPos	-= 1;
							}
						}
						break;
					case SB_LINERIGHT:
						{
							if((int)si.nPos + 1 > (int)si.nMax){
								si.nPos	= si.nMax;
							}else{
								si.nPos	+= 1;
							}
						}
						break;
					case SB_PAGELEFT:
						{
							if((int)si.nPos - (int)si.nPage < 0){
								si.nPos	= 0;
							}else{
								si.nPos	-= si.nPage;
							}
						}
						break;
					case SB_PAGERIGHT:
						{
							if((int)si.nPos + (int)si.nPage > (int)si.nMax){
								si.nPos		= si.nMax;
							}else{
								si.nPos		+= si.nPage;
							}
						}
						break;
					case SB_THUMBTRACK:
						{
							si.nPos		= HIWORD(wParam);
						}
						break;
				}

				Document *doc = (Document*)GetWindowLongPtr(GetParent(hWnd),GWL_USERDATA);
				doc->im->X((int)si.nPos);

				hRulerDataClass *hrdc = doc->hrc.RClass();
				hrdc->X(int(-doc->im->X() * doc->im->Scale()));
				doc->hrc.Invalidate();

				InvalidateRect(hWnd,NULL,false);

				SetScrollInfo(hWnd,SB_HORZ,&si,true);
			}
			break;
		case WM_VSCROLL:
			{
				SCROLLINFO si;
				si.cbSize		= sizeof(SCROLLINFO);
				si.fMask		= SIF_ALL;

				GetScrollInfo(hWnd,SB_VERT,&si);

				si.fMask		= SIF_POS;
				
				switch(LOWORD(wParam)){
					case SB_ENDSCROLL:
						break;
					case SB_LINEUP:
						{
							if((int)si.nPos - 1 < 0){
								si.nPos	= 0;
							}else{
								si.nPos	-= 1;
							}
						}
						break;
					case SB_LINEDOWN:
						{
							if((int)si.nPos + 1 > (int)si.nMax){
								si.nPos	= si.nMax;
							}else{
								si.nPos	+= 1;
							}
						}
						break;
					case SB_PAGEUP:
						{
							if((int)si.nPos - (int)si.nPage < 0){
								si.nPos	= 0;
							}else{
								si.nPos	-= si.nPage;
							}
						}
						break;
					case SB_PAGEDOWN:
						{
							if((int)si.nPos + (int)si.nPage > (int)si.nMax){
								si.nPos		= si.nMax;
							}else{
								si.nPos		+= si.nPage;
							}
						}
						break;
					case SB_THUMBTRACK:
						{
							si.nPos		= HIWORD(wParam);
						}
						break;
				}

				Document *doc = (Document*)GetWindowLongPtr(GetParent(hWnd),GWL_USERDATA);
				doc->im->Y((int)si.nPos);

				vRulerDataClass *vrdc = doc->vrc.RClass();
				vrdc->Y(int(-doc->im->Y() * doc->im->Scale()));
				doc->vrc.Invalidate();

				InvalidateRect(hWnd,NULL,false);

				SetScrollInfo(hWnd,SB_VERT,&si,true);

				/*si.fMask = SIF_ALL;

				GetScrollInfo(hWnd,SB_VERT,&si);*/
				//int a = 100;
			}
			break;
		case WM_SIZE:
			{
			}
			break;
		case WM_LBUTTONDOWN:
			{
				Document *doc = (Document*)GetWindowLongPtr(GetParent(hWnd),GWL_USERDATA);
				doc->WmLButtonDown(MousePos);
				MK = MK_LBUTTON;
				SetCapture(hWnd);
			}
			break;
		case WM_RBUTTONDOWN:
			{
				MK=MK_RBUTTON;
				Document *doc = (Document*)GetWindowLongPtr(GetParent(hWnd),GWL_USERDATA);
				doc->scWnd.Destroy();
			}
			break;
		case WM_RBUTTONUP:
			MK=NULL;
			break;
		case WM_LBUTTONUP:
			{
				Document *doc = (Document*)GetWindowLongPtr(GetParent(hWnd),GWL_USERDATA);
				doc->WmLButtonUp(MousePos, int(wParam));
				MK=NULL;
				ReleaseCapture();
				//InvalidateRect(hWnd, NULL, false);
			}
			break;
		case WM_MOUSEMOVE:
			{
				GetCursorPos(&MousePos);
				ScreenToClient(hWnd, &MousePos);
				//if(MK == MK_LBUTTON){
					Document *doc = (Document*)GetWindowLongPtr(GetParent(hWnd),GWL_USERDATA);
					doc->WmMouseMove(MousePos, UINT(wParam));
					doc->DrawGradient();
					/*HDC hdc = GetDC(GetParent(hWnd));
					doc->im->gr.gr.StretchBltB(hdc,0,HEIGHT,doc->wndImSize.right - doc->wndImSize.left + RULER_HEIGHT, HEIGHT);
					ReleaseDC(GetParent(hWnd),hdc);*/
					InvalidateRect(hWnd,NULL,false);
				//}
			}
			break;
		case WM_MOUSEWHEEL:
			{
				if(MK != MK_LBUTTON){
					Document *doc = (Document*)GetWindowLongPtr(GetParent(hWnd),GWL_USERDATA);
					doc->WmMouseWheel(hWnd,wParam,lParam);
				}
			}
			return 0;
		case WM_PAINT:
			{
				Document *doc = (Document*)GetWindowLongPtr(GetParent(hWnd),GWL_USERDATA);
				//RECT WndSize;
				//GetClientRect(hWnd,&WndSize);
				PAINTSTRUCT ps;
				HDC hdc=BeginPaint(hWnd,&ps);
				HDC mem=CreateCompatibleDC(hdc);
				SelectObject(mem,doc->ImageWindowBitmap);
				RYS->rectangle(mem,doc->wndImSize,kTla,PS_SOLID,1,kTla);

				doc->Draw(mem, MK);

				BitBlt(hdc,0,0,doc->wndImSize.right,doc->wndImSize.bottom,mem,0,0,SRCCOPY);
				DeleteDC(mem);
				EndPaint(hWnd,&ps);
			}
			break;
		case WM_DESTROY:
			//DestroyWindow(hWnd);
			break;
	}
	return DefWindowProc(hWnd,msg,wParam,lParam);
}

#pragma endregion

LRESULT CALLBACK WindowGrPoints(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam){
	switch(msg){
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);

				Document *doc = (Document*)GetWindowLongPtr(GetParent(hWnd),GWL_USERDATA);

				doc->grPtWmPaint(hdc);

				EndPaint(hWnd, &ps);
			}
			break;
		case WM_MOUSELEAVE:
			{
				Document *doc = (Document*)GetWindowLongPtr(GetParent(hWnd),GWL_USERDATA);
				doc->grPtWmMouseLeave();
			}
			break;
		case WM_MOUSEMOVE:
			{
				Document *doc = (Document*)GetWindowLongPtr(GetParent(hWnd),GWL_USERDATA);

				doc->grPtWmMouseMove(int(LOWORD(lParam)),int(HIWORD(lParam)),UINT(wParam));

				TRACKMOUSEEVENT ms;
				ms.cbSize = sizeof(TRACKMOUSEEVENT);
				ms.dwFlags = TME_LEAVE|TME_HOVER;
				ms.hwndTrack = hWnd;
				_TrackMouseEvent(&ms);
			}
			break;
		case WM_LBUTTONDOWN:
			{
				Document *doc = (Document*)GetWindowLongPtr(GetParent(hWnd),GWL_USERDATA);

				doc->grPtWmLButtonDown();
			}
			break;
		case WM_LBUTTONUP:
			{
				Document *doc = (Document*)GetWindowLongPtr(GetParent(hWnd),GWL_USERDATA);

				doc->grPtWmLButtonUp();
			}
			break;
		case WM_RBUTTONDOWN:
			{
				Document *doc = (Document*)GetWindowLongPtr(GetParent(hWnd),GWL_USERDATA);

				doc->grPtWmRButtonDown();
			}
			break;

	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK WndScProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
		case WM_CREATE:
			{
				CREATESTRUCT cs = *(CREATESTRUCT*)lParam;
				SetWindowLong(hWnd,GWL_USERDATA,(LONG)cs.lpCreateParams);
			}
			break;
		case WM_COMMAND:
			{
				Document *doc = (Document*)GetWindowLongPtr(hWnd,GWL_USERDATA);
				switch((int)lParam){
					case 0:
						{
						}
						break;
					case 1:
						{
						}
						break;
					default:
						{
							HWND hControl = (HWND) lParam;
							int id = LOWORD(wParam);
							int notification = HIWORD(wParam);
							switch(id){
								case ID_SC_LEFT_COLOR:
									{
										if(BN_CLICKED == notification){
											if(int(SendMessage(hControl, BM_GETCHECK, 0, 0)) == BST_UNCHECKED){
												SendMessage(hControl, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
												SendMessage(doc->scWnd.scRightColor, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
												SetFocus(NULL);

												SetWindowText(doc->scWnd.scHex, doc->scWnd.left.ChangeColorToHexStringRGBA().c_str());
											}
										}
									}
									break;
								case ID_SC_RIGHT_COLOR:
									{
										if(BN_CLICKED == notification){
											if(int(SendMessage(hControl, BM_GETCHECK, 0, 0)) == BST_UNCHECKED){
												SendMessage(hControl, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
												SendMessage(doc->scWnd.scLeftColor, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
												SetFocus(NULL);

												SetWindowText(doc->scWnd.scHex, doc->scWnd.right.ChangeColorToHexStringRGBA().c_str());
											}
										}
									}
									break;
								case ID_SC_CHAIN:
									{
										if(BN_CLICKED == notification){
											UINT check = SendMessage(hControl, BM_GETCHECK, 0, 0);
											SendMessage(hControl, BM_SETCHECK, (WPARAM)(check == BST_CHECKED ? BST_UNCHECKED : BST_CHECKED), 0);
											SendMessage(hControl,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)(check == BST_CHECKED ? doc->scWnd.hChainClose : doc->scWnd.hChainOpen));
											SetFocus(NULL);

											UINT chLeft = SendMessage(doc->scWnd.scLeftColor,BM_GETCHECK,NULL,NULL);

											if(check == BST_UNCHECKED){
												if(doc->scWnd.scInd == 0){
													doc->im->gr.tSegment[0].SetLeftColorRGB(doc->scWnd.right.r, doc->scWnd.right.g, doc->scWnd.right.b, doc->scWnd.right.a);
												}else if(doc->scWnd.scInd == doc->im->gr.tSegment.size()){
													(doc->im->gr.tSegment.end()-1)->SetRightColorRGB(doc->scWnd.right.r, doc->scWnd.right.g, doc->scWnd.right.b, doc->scWnd.right.a);
												}else{
													doc->im->gr.tSegment[doc->scWnd.scInd - 1].SetRightColorRGB(doc->scWnd.left.r, doc->scWnd.left.g, doc->scWnd.left.b, doc->scWnd.left.a);
													doc->im->gr.tSegment[doc->scWnd.scInd].SetLeftColorRGB(doc->scWnd.right.r, doc->scWnd.right.g, doc->scWnd.right.b, doc->scWnd.right.a);
												}
											}else{
												if(doc->scWnd.scInd == 0){
													doc->im->gr.tSegment[0].SetLeftColorRGB(doc->scWnd.right.r, doc->scWnd.right.g, doc->scWnd.right.b, doc->scWnd.right.a);
												}else if(doc->scWnd.scInd == doc->im->gr.tSegment.size()){
													(doc->im->gr.tSegment.end()-1)->SetRightColorRGB(doc->scWnd.right.r, doc->scWnd.right.g, doc->scWnd.right.b, doc->scWnd.right.a);
												}else{
													if(chLeft == BST_CHECKED){
														doc->im->gr.tSegment[doc->scWnd.scInd - 1].SetRightColorRGB(doc->scWnd.left.r, doc->scWnd.left.g, doc->scWnd.left.b, doc->scWnd.left.a);
														doc->im->gr.tSegment[doc->scWnd.scInd].SetLeftColorRGB(doc->scWnd.left.r, doc->scWnd.left.g, doc->scWnd.left.b, doc->scWnd.left.a);
													}else{
														doc->im->gr.tSegment[doc->scWnd.scInd - 1].SetRightColorRGB(doc->scWnd.right.r, doc->scWnd.right.g, doc->scWnd.right.b, doc->scWnd.right.a);
														doc->im->gr.tSegment[doc->scWnd.scInd].SetLeftColorRGB(doc->scWnd.right.r, doc->scWnd.right.g, doc->scWnd.right.b, doc->scWnd.right.a);
													}
												}
											}

											doc->im->gr.CreateGradientVector();
											doc->DrawGradient();

											doc->RefreshGradient();
										}
									}
									break;
								case ID_SC_RED:
									{
										int sl = SendMessage(doc->scWnd.scLeftColor, BM_GETSTATE, 0, 0);
										if(sl == BST_CHECKED){
											doc->scWnd.left.r = (float)(doc->scWnd.scRed.GetValue() / 255.);
										}else{
											doc->scWnd.right.r = (float)(doc->scWnd.scRed.GetValue() / 255.);
										}

										SetWindowText(doc->scWnd.scHex, sl == BST_CHECKED ? doc->scWnd.left.ChangeColorToHexStringRGBA().c_str() : doc->scWnd.right.ChangeColorToHexStringRGBA().c_str());
										
									}
									break;
								case ID_SC_GREEN:
									{
										int sl = SendMessage(doc->scWnd.scLeftColor, BM_GETSTATE, 0, 0);
										if(sl == BST_CHECKED){
											doc->scWnd.left.g = (float)(doc->scWnd.scGreen.GetValue() / 255.);
										}else{
											doc->scWnd.right.g = (float)(doc->scWnd.scGreen.GetValue() / 255.);
										}

										SetWindowText(doc->scWnd.scHex, sl == BST_CHECKED ? doc->scWnd.left.ChangeColorToHexStringRGBA().c_str() : doc->scWnd.right.ChangeColorToHexStringRGBA().c_str());
									}
									break;
								case ID_SC_BLUE:
									{
										int sl = SendMessage(doc->scWnd.scLeftColor, BM_GETSTATE, 0, 0);
										if(sl == BST_CHECKED){
											doc->scWnd.left.b = (float)(doc->scWnd.scBlue.GetValue() / 255.);
										}else{
											doc->scWnd.right.b = (float)(doc->scWnd.scBlue.GetValue() / 255.);
										}

										SetWindowText(doc->scWnd.scHex, sl == BST_CHECKED ? doc->scWnd.left.ChangeColorToHexStringRGBA().c_str() : doc->scWnd.right.ChangeColorToHexStringRGBA().c_str());
									}
									break;
								case ID_SC_ALPHA:
									{
										int sl = SendMessage(doc->scWnd.scLeftColor, BM_GETSTATE, 0, 0);
										if(sl == BST_CHECKED){
											doc->scWnd.left.a = (float)(doc->scWnd.scAlpha.GetValue() / 255.);
										}else{
											doc->scWnd.right.a = (float)(doc->scWnd.scAlpha.GetValue() / 255.);
										}

										SetWindowText(doc->scWnd.scHex, sl == BST_CHECKED ? doc->scWnd.left.ChangeColorToHexStringRGBA().c_str() : doc->scWnd.right.ChangeColorToHexStringRGBA().c_str());
									}
									break;
								case ID_SC_HUE:
								case ID_SC_SATURATION:
								case ID_SC_VALUE:
									{
										int ch = SendMessage(doc->scWnd.scChainButton, BM_GETSTATE, 0, 0);
										
										float h, s, v;
										
										h = (float)doc->scWnd.scHue.GetValue();
										s = float(doc->scWnd.scSaturation.GetValue() / 100.);
										v = float(doc->scWnd.scValue.GetValue() / 100.);

										float r, g, b;

										HSV2RGB(h, s, v, r, g, b);

										int sl = SendMessage(doc->scWnd.scLeftColor, BM_GETSTATE, 0, 0);
										if(sl == BST_CHECKED){
											doc->scWnd.left.r = r;
											doc->scWnd.left.g = g;
											doc->scWnd.left.b = b;
										}else{
											doc->scWnd.right.r = r;
											doc->scWnd.right.g = g;
											doc->scWnd.right.b = b;
										}

										SetWindowText(doc->scWnd.scHex, sl == BST_CHECKED ? doc->scWnd.left.ChangeColorToHexStringRGBA().c_str() : doc->scWnd.right.ChangeColorToHexStringRGBA().c_str());
									}
									break;
								case ID_SC_HEX:
									{
										int sl = SendMessage(doc->scWnd.scLeftColor, BM_GETSTATE, 0, 0);
										switch(notification){
											case EN_KILLFOCUS:
												{
													if(GetWindowTextLength(doc->scWnd.scHex) == 8){
														char text[9];
														GetWindowText(doc->scWnd.scHex, text, 9);
														if(sl ==BST_CHECKED){
															if(!doc->scWnd.left.SetColorFromHexString(text)){
																std::string s = doc->scWnd.left.ChangeColorToHexStringRGBA();
																SetWindowText(doc->scWnd.scHex,s.c_str());
															}
														}else{
															if(!doc->scWnd.right.SetColorFromHexString(text)){
																std::string s = doc->scWnd.right.ChangeColorToHexStringRGBA();
																SetWindowText(doc->scWnd.scHex,s.c_str());
															}
														}
													}
												}
												break;
											case EN_CHANGE:
												{
													if(GetWindowTextLength(doc->scWnd.scHex) == 8){
														char text[9];
														GetWindowText(doc->scWnd.scHex, text, 9);

														if(sl & BST_CHECKED){
															if(doc->scWnd.left.SetColorFromHexString(text)){
																doc->scWnd.cleft.SetRGBA_Color(doc->scWnd.left.r, doc->scWnd.left.g, doc->scWnd.left.b, doc->scWnd.left.a);
																SendMessage(doc->scWnd.scLeftColor,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)doc->scWnd.cleft.hBitmap);

																if(doc->scWnd.scInd == 0){
																	doc->im->gr.tSegment[0].SetLeftColorRGB(doc->scWnd.left.r,doc->scWnd.left.g, doc->scWnd.left.b, doc->scWnd.left.a);
																}else if(doc->scWnd.scInd == doc->im->gr.tSegment.size()){
																	doc->im->gr.tSegment[doc->scWnd.scInd - 1].SetRightColorRGB(doc->scWnd.left.r,doc->scWnd.left.g, doc->scWnd.left.b, doc->scWnd.left.a);
																}else{
																	int ch = SendMessage(doc->scWnd.scChainButton, BM_GETSTATE, 0, 0);
																	doc->im->gr.tSegment[doc->scWnd.scInd - 1].SetRightColorRGB(doc->scWnd.left.r,doc->scWnd.left.g, doc->scWnd.left.b, doc->scWnd.left.a);
																	if(ch==BST_UNCHECKED){
																		doc->im->gr.tSegment[doc->scWnd.scInd].SetLeftColorRGB(doc->scWnd.left.r,doc->scWnd.left.g, doc->scWnd.left.b, doc->scWnd.left.a);
																	}
																}

																InvalidateRect(doc->scWnd.scLeftColor, NULL, false);

																doc->scWnd.scRed.SetValue(doc->scWnd.left.r * 255.f);
																doc->scWnd.scGreen.SetValue(doc->scWnd.left.g * 255.f);
																doc->scWnd.scBlue.SetValue(doc->scWnd.left.b * 255.f);
																doc->scWnd.scAlpha.SetValue(doc->scWnd.left.a * 255.f);

																float h, s, v;
																h = s = v = 0;
																RGB2HSV(h, s, v, doc->scWnd.left.r, doc->scWnd.left.g, doc->scWnd.left.b);

																if(v){
																	if(s){
																		if(!(h == 0.f && doc->scWnd.scHue.GetValue() == 360.))
																			doc->scWnd.scHue.SetValue(h);
																	}
																	doc->scWnd.scSaturation.SetValue(s * 100.);
																}
																doc->scWnd.scValue.SetValue(v * 100.);

																doc->im->gr.CreateGradientVector();
																doc->DrawGradient();
																doc->RefreshGradient();
															}
														}else{
															if(doc->scWnd.right.SetColorFromHexString(text)){
																doc->scWnd.cright.SetRGBA_Color(doc->scWnd.right.r, doc->scWnd.right.g, doc->scWnd.right.b, doc->scWnd.right.a);
																SendMessage(doc->scWnd.scRightColor,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)doc->scWnd.cright.hBitmap);

																if(doc->scWnd.scInd == 0){
																	doc->im->gr.tSegment[0].SetLeftColorRGB(doc->scWnd.left.r,doc->scWnd.left.g, doc->scWnd.left.b, doc->scWnd.left.a);
																}else if(doc->scWnd.scInd == doc->im->gr.tSegment.size()){
																	doc->im->gr.tSegment[doc->scWnd.scInd - 1].SetRightColorRGB(doc->scWnd.right.r,doc->scWnd.right.g, doc->scWnd.right.b, doc->scWnd.right.a);
																}else{
																	int ch = SendMessage(doc->scWnd.scChainButton, BM_GETSTATE, 0, 0);
																	doc->im->gr.tSegment[doc->scWnd.scInd].SetLeftColorRGB(doc->scWnd.right.r,doc->scWnd.right.g, doc->scWnd.right.b, doc->scWnd.right.a);
																	if(ch==BST_UNCHECKED){
																		doc->im->gr.tSegment[doc->scWnd.scInd - 1].SetRightColorRGB(doc->scWnd.right.r,doc->scWnd.right.g, doc->scWnd.right.b, doc->scWnd.right.a);
																	}
																}

																InvalidateRect(doc->scWnd.scRightColor, NULL, false);

																doc->scWnd.scRed.SetValue(doc->scWnd.right.r * 255.f);
																doc->scWnd.scGreen.SetValue(doc->scWnd.right.g * 255.f);
																doc->scWnd.scBlue.SetValue(doc->scWnd.right.b * 255.f);
																doc->scWnd.scAlpha.SetValue(doc->scWnd.right.a * 255.f);

																float h, s, v;
																h = s = v = 0;
																RGB2HSV(h, s, v, doc->scWnd.right.r, doc->scWnd.right.g, doc->scWnd.right.b);
								
																if(v){
																	if(s){
																		if(!(h == 0.f && doc->scWnd.scHue.GetValue() == 360.))
																			doc->scWnd.scHue.SetValue(h);
																	}
																	doc->scWnd.scSaturation.SetValue(s * 100.);
																}
																doc->scWnd.scValue.SetValue(v * 100.);

																doc->im->gr.CreateGradientVector();
																doc->DrawGradient();
																doc->RefreshGradient();
															}
														}
													};
												}
												break;
										}
									}
									break;
							}
						}
						break;
				}
			}
			break;
		case WM_MOUSEMOVE:
			{
				Document *doc = (Document*)GetWindowLongPtr(hWnd,GWL_USERDATA);


			}
			break;
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				
				HBRUSH hbr = CreateSolidBrush(RGB(240,240,240));

				SelectObject(hdc, hbr);

				Document *doc = (Document*)GetWindowLongPtr(hWnd,GWL_USERDATA);

				RECT r;

				SetRect(&r, 0, 0, doc->scWnd.width, doc->scWnd.height);
				FillRect(hdc, &r, hbr);

				SetBkMode(hdc, TRANSPARENT);

				int pos = 62;

				SelectObject(hdc,textFont);
				std::string str = "Czerwony (red)";
				TextOut(hdc,10,pos,str.c_str(), str.size());

				pos += 50;

				doc->scWnd.gred.BitBltB(hdc, 14, pos);

				pos += 10;

				str = "Zielony (green)";
				TextOut(hdc,10,pos,str.c_str(), str.size());

				pos += 50;
				
				doc->scWnd.ggreen.BitBltB(hdc, 14, pos);

				pos += 10;

				str = "Niebieski (blue)";
				TextOut(hdc,10,pos,str.c_str(), str.size());

				pos += 50;
				
				doc->scWnd.gblue.BitBltB(hdc, 14, pos);

				pos += 10;

				str = "Alfa (alpha)";
				TextOut(hdc,10,pos,str.c_str(), str.size());

				pos += 50;

				doc->scWnd.galpha.BitBltB(hdc, 14, pos);

				pos += 10;

				str = "Barwa (hue)";
				TextOut(hdc,10,pos,str.c_str(), str.size());

				pos += 50;

				doc->scWnd.ghue.BitBltB(hdc, 14, pos);

				pos += 10;

				str = "Nasycenie (saturation)";
				TextOut(hdc,10,pos,str.c_str(), str.size());

				pos += 50;

				doc->scWnd.gsaturation.BitBltB(hdc, 14, pos);

				pos += 10;

				str = "Wartoœæ (value)";
				TextOut(hdc,10,pos,str.c_str(), str.size());

				pos += 50;

				doc->scWnd.gvalue.BitBltB(hdc, 14, pos);

				pos += 20;

				str = "Hex kolor (hex color) RGBA";

				TextOut(hdc, 10, pos, str.c_str(), str.size());


				DeleteObject(hbr);

				EndPaint(hWnd, &ps);
			}
			break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

#pragma region Procedura mdiChildProc

LRESULT CALLBACK mdiChildProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam){
	switch(msg){
		case WM_MOUSEWHEEL:
			{
				Document *doc = (Document*)GetWindowLongPtr(hWnd,GWL_USERDATA);
				SendMessage(doc->imWindow,WM_MOUSEWHEEL,wParam,lParam);
			}
			return 0;
		case WM_SIZE:
			{
				Document *doc = (Document*)GetWindowLongPtr(hWnd,GWL_USERDATA);
				if(doc)
					doc->WmSize(hWnd);
			}
			break;
		case WM_LBUTTONDOWN:
			{
				Document *doc = (Document*)GetWindowLongPtr(hWnd,GWL_USERDATA);
				doc->scWnd.Destroy();
			}
			break;
		case WM_RBUTTONDOWN:
			{
				Document *doc = (Document*)GetWindowLongPtr(hWnd,GWL_USERDATA);
				doc->scWnd.Destroy();
				POINT MousePos;
				GetCursorPos(&MousePos);
				HMENU hMenu = LoadMenu((HINSTANCE)GetWindowLongPtr(hWnd,GWL_HINSTANCE),MAKEINTRESOURCE(IDR_GR_MENU));
				hMenu = GetSubMenu(hMenu,0);
				TrackPopupMenu(hMenu,TPM_RIGHTBUTTON,MousePos.x,MousePos.y,0,hWnd,NULL);
			}
			break;
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				Document *doc = (Document*)GetWindowLongPtr(hWnd,GWL_USERDATA);
				if(doc)
					doc->DrawGradient();
					//doc->im->gr.gr.StretchBltB(hdc,0,HEIGHT,doc->wndImSize.right - doc->wndImSize.left + RULER_HEIGHT, HEIGHT);
				EndPaint(hWnd, &ps);
			}
			break;
		case WM_COMMAND:
			{
				Document *doc = (Document*)GetWindowLongPtr(hWnd,GWL_USERDATA);
				doc->scWnd.Destroy();
				switch(LOWORD(wParam)){
					case 5003:
						{
							POINT MousePos;
							GetCursorPos(&MousePos);
							HMENU hRulerMenu = LoadMenu((HINSTANCE)GetWindowLongPtr(hWnd,GWL_HINSTANCE),MAKEINTRESOURCE(IDR_RULERMENU));
							hRulerMenu = GetSubMenu(hRulerMenu,0);
							switch(doc->vrc.SUnit()){
								case pixel: // unit::pixel
									CheckMenuItem(hRulerMenu,ID_JEDNOSTKI_PIXEL,MF_CHECKED);
									break;
								case milimiter:
									CheckMenuItem(hRulerMenu,ID_JEDNOSTKI_MILIMITER,MF_CHECKED);
									break;
								case inch:
									CheckMenuItem(hRulerMenu,ID_JEDNOSTKI_INCH,MF_CHECKED);
									break;
							}
							TrackPopupMenu(hRulerMenu,TPM_RIGHTBUTTON,MousePos.x,MousePos.y,0,hWnd,NULL);
							SetFocus(GetParent(hWnd));
						}
						break;
					case 5004: // zapis gradientu do pliku ggr lub svg
						{
							doc->im->CreateGradientFile();
						}
						break;
					case 5005:
						{
							doc->SetInd();
						}
						break;
					case 5006: // w³¹czanie i wy³¹czanie trybu rêcznego
						{
							if(doc->mode == BTM_LINE){
								if(HIWORD(wParam) == BN_CLICKED){
									HWND hC = (HWND)lParam;
									doc->btHandClicked = !doc->btHandClicked;
									SendMessage(hC, BM_SETCHECK, doc->btHandClicked,0);
									doc->nc.SetEnable(!doc->btHandClicked);
									InvalidateRect(doc->imWindow, NULL, false);
									if(doc->btHandClicked)
										doc->SetInd(1);
									else
										doc->SetInd();
								}
							}
 						}
						break;
					case 5007: // tryb tworzenia gradientu po linii prostej
						{
							if(HIWORD(wParam) == BN_CLICKED){
								if(doc->mode != BTM_LINE){
									doc->nc.SetEnable(true);
									doc->SetInd();
								}
								doc->mode = BTM_LINE;
								doc->im->CreateGr(doc->fmp,doc->lmp,doc->ind,doc->wndImSize);
								doc->DrawGradient();
								InvalidateRect(doc->imWindow,NULL, false);
								if(SendMessage((HWND)lParam, BM_GETCHECK, 0, 0) == BST_UNCHECKED){
									SendMessage((HWND)lParam, BM_SETCHECK, BST_CHECKED,0);
									SendMessage(doc->btPointMode, BM_SETCHECK, BST_UNCHECKED, 0);
									SendMessage(doc->btCircleMode, BM_SETCHECK, BST_UNCHECKED, 0);
								}
							}
						}
						break;
					case 5008: // tryb tworzenia gradientu z punktów
						{
							if(HIWORD(wParam) == BN_CLICKED){
								if(SendMessage((HWND)lParam, BM_GETCHECK, 0, 0) == BST_UNCHECKED){
									SendMessage((HWND)lParam, BM_SETCHECK, BST_CHECKED,0);
									SendMessage(doc->btLineMode, BM_SETCHECK, BST_UNCHECKED, 0);
									SendMessage(doc->btCircleMode, BM_SETCHECK, BST_UNCHECKED, 0);
									if(doc->btHandClicked){
										PostMessage(doc->btHand, BM_SETCHECK, BST_UNCHECKED, 0);
										doc->btHandClicked = false;
									}
									doc->selPt = -1;
									if(doc->grPtTable.size())
										doc->grPtTable.clear();
									doc->grPtTable.push_back(grPoint(doc->lmp,doc->lmp));
									doc->grPtTable.push_back(grPoint(doc->fmp,doc->fmp));
									doc->mode = BTM_POINT;
									doc->nc.SetEnable(false);
									doc->im->CreateGr(doc->grPtTable);
									doc->DrawGradient();
									InvalidateRect(doc->imWindow,NULL, false);
								}
							}
						}
						break;
					case 5009: // tryb tworzenia gradientu po okrêgu
						{
							if(HIWORD(wParam) == BN_CLICKED){
								if(SendMessage((HWND)lParam, BM_GETCHECK, 0, 0) == BST_UNCHECKED){
									SendMessage((HWND)lParam, BM_SETCHECK, BST_CHECKED,0);
									SendMessage(doc->btLineMode, BM_SETCHECK, BST_UNCHECKED, 0);
									SendMessage(doc->btPointMode, BM_SETCHECK, BST_UNCHECKED, 0);
									if(doc->btHandClicked){
										PostMessage(doc->btHand, BM_SETCHECK, BST_UNCHECKED, 0);
										doc->btHandClicked = false;
									}
									doc->selPt = -1;
									//if(doc->grPtTable.size())
									//	doc->grPtTable.clear();
									//doc->grPtTable.push_back(grPoint(doc->lmp,doc->lmp));
									//doc->grPtTable.push_back(grPoint(doc->fmp,doc->fmp));
									doc->mode = BTM_CIRCLE;
									doc->nc.SetEnable(true);
									doc->CreateGradientByCircle();
									//doc->im->CreateGr(doc->grPtTable);
									doc->DrawGradient();
									InvalidateRect(doc->imWindow,NULL, false);
								}
							}
						}
						break;
					case ID_JEDNOSTKI_PIXEL:
						{
							doc->vrc.SUnit(/*unit::*/pixel,doc->im->yResolution(pixel));
							doc->hrc.SUnit(pixel,doc->im->xResolution(pixel));
							doc->vrc.Invalidate();
							doc->hrc.Invalidate();
						}
						break;
					case ID_JEDNOSTKI_MILIMITER:
						{
							doc->vrc.SUnit(milimiter,doc->im->yResolution(milimiter));
							doc->hrc.SUnit(milimiter,doc->im->xResolution(milimiter));
							doc->vrc.Invalidate();
							doc->hrc.Invalidate();
						}
						break;
					case ID_JEDNOSTKI_INCH:
						{
							doc->vrc.SUnit(inch,doc->im->yResolution(inch));
							doc->hrc.SUnit(inch,doc->im->xResolution(inch));
							doc->vrc.Invalidate();
							doc->hrc.Invalidate();
						}
						break;
					case ID_RWM:
						{
							float dx = 1.f / (doc->im->gr.tSegment.size());
							float dm;
							for(int i = 0; i < doc->im->gr.tSegment.size() - 1; i++){
								dm = dx * float(i + 1);
								doc->im->gr.tSegment[i + 1].SetRightAndLeftPos(dm, dm + dx);
								doc->im->gr.tSegment[i].SetRightPos(dm);
							}
							(doc->im->gr.tSegment.end() -1)->SetRightPos(1.f);
							doc->im->gr.CreateGradientVector();
							doc->DrawGradient();
							InvalidateRect(doc->imWindow, NULL, false);
						}
						break;
					case ID_CFB:
						{
							doc->SetColorsFromBitmap();
						}
						break;
				}
			}
			break;
		case WM_DESTROY:
			{
				Document *doc = (Document*)GetWindowLongPtr(hWnd,GWL_USERDATA);
				if(doc)
					delete doc;
				doc = NULL;
			}
			break;
	}
	return DefMDIChildProc(hWnd, msg, wParam, lParam);
}

#pragma endregion

#pragma region Klasa Document

void Document::InitWindowPos(HWND hWnd, std::string fileName, HINSTANCE hInstance){
	fmp.x = fmp.y = lmp.x = lmp.y = 0;
	grPtMouseIn = false;
	grPtSel = -1;
	grSgSel = -1;
	mode = BTM_LINE;
	this->btHandClicked = false;
	this->selPt = -1;
	isPtOnLine = false;
	SetWindowText(hWnd,fileName.c_str());
	ShowWindow(hWnd,SW_SHOWNORMAL);
	// Okno obrazka
	imWindow = CreateWindowEx(WS_EX_CLIENTEDGE,"Image Window","Image Window",WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL,0,0,100,100,hWnd,NULL,hInstance,NULL);

	grPtWindow = CreateWindow("WindowGrPoints", "WindowGrPoints", WS_CHILD|WS_VISIBLE,0,HEIGHT * 2, 100, RULER_HEIGHT, hWnd, NULL, hInstance, NULL);
	// Przycisk jednostek
	btWindow = CreateWindow("BUTTON","",WS_CHILD|WS_VISIBLE|BS_BITMAP,0,HEIGHT * 2 + RULER_HEIGHT,RULER_HEIGHT,RULER_HEIGHT,hWnd,(HMENU)5003,hInstance,NULL);
	// Zapisywanie gradientu
	btSave = CreateWindow("BUTTON","",WS_CHILD|WS_VISIBLE|BS_BITMAP,0,0,HEIGHT, HEIGHT, hWnd,(HMENU)5004,hInstance, NULL);
	// Rêczne tworzenie gradientu
	btHand = CreateWindow("BUTTON","",WS_CHILD|WS_VISIBLE|BS_BITMAP|BS_PUSHLIKE|BS_RADIOBUTTON , HEIGHT, 0,HEIGHT, HEIGHT,hWnd, (HMENU) 5006,hInstance,NULL);

	btLineMode = CreateWindow("BUTTON", "", WS_CHILD|WS_VISIBLE|BS_BITMAP|BS_PUSHLIKE|BS_RADIOBUTTON , HEIGHT * 2, 0,HEIGHT, HEIGHT,hWnd, (HMENU) 5007,hInstance,NULL);

	btCircleMode = CreateWindow("BUTTON","",WS_CHILD|WS_VISIBLE|BS_BITMAP|BS_PUSHLIKE|BS_RADIOBUTTON, HEIGHT * 3, 0, HEIGHT, HEIGHT, hWnd, (HMENU) 5009, hInstance, NULL);

	btPointMode = CreateWindow("BUTTON", "", WS_CHILD|WS_VISIBLE|BS_BITMAP|BS_PUSHLIKE|BS_RADIOBUTTON , HEIGHT * 4, 0,HEIGHT, HEIGHT,hWnd, (HMENU) 5008,hInstance,NULL);

	SendMessage(btLineMode, BM_SETCHECK, BST_CHECKED, 0);

	nc.CreateWnd(hWnd,hInstance,"title",5005, HEIGHT * 5,0,100,HEIGHT);
	nc.SetMaxValue(100);
	nc.SetMinValue(2);
	nc.SetDevide(98);
	nc.SetValue(ind);

	SendMessage(btWindow,	BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_RULER_BUTTON)));
	SendMessage(btSave,		BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_SAVE_GR)));
	SendMessage(btHand,		BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_BITMAP2)));
	SendMessage(btLineMode,	BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_LINE)));
	SendMessage(btPointMode,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_POINTS)));
	SendMessage(btCircleMode,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_CIRCLE)));
	
	RECT WndSize;

	GetClientRect(hWnd,&WndSize);

	WndSize.bottom -= GetSystemMetrics(SM_CYFRAME);
	WndSize.right -= GetSystemMetrics(SM_CXFRAME);
	
	vrc.CreateVRulerWindow(hWnd,0,RULER_HEIGHT * 2 + HEIGHT * 2,RULER_HEIGHT,WndSize.bottom,5000,im->yResolution(milimiter),milimiter,0,100);
	hrc.CreateHRulerWindow(hWnd,RULER_HEIGHT,HEIGHT * 2 + RULER_HEIGHT,WndSize.right,RULER_HEIGHT,5001,im->xResolution(milimiter),milimiter,0,100);

	WmSize(hWnd);

	im->CreateGr(fmp,lmp,ind,this->wndImSize);
}

Document::Document(HWND hWnd){
	ind = GR_IND;
	HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWL_HINSTANCE);
	dlgOpenSaveFileName ofn(hInstance,"Bitmapa (*.jpg)\0*.jpg\0*Bitmapa (*.bmp)\0*.bmp\0*gif\0*.gif\0*png \0*.png\0");
	if(ofn.ShowOpenFileName(hWnd)){
		im = new myImage(ofn.GetFileName());
		InitWindowPos(hWnd,std::string(ofn.GetFileName()),hInstance);
	}else{
		SendMessage(mdiClient,WM_MDIDESTROY,(WPARAM)hWnd,0);
	}
}
Document::Document(HWND hWnd,const char* docName){
	ind = GR_IND;
	HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWL_HINSTANCE);
	fmp.x = fmp.y = lmp.x = lmp.y = 0;
	if(docName){
		im = new myImage(docName);
		InitWindowPos(hWnd,std::string(docName),hInstance);
	}else{
		SendMessage(mdiClient,WM_MDIDESTROY,(WPARAM)hWnd,0);
	}
}

void Document::SetInd(){ // Ustawia liczbê punktów
	ind = (UINT)nc.GetValue();
	switch(mode){
		case BTM_LINE:
			im->CreateGr(fmp,lmp,ind,wndImSize);
			break;
		case BTM_CIRCLE:
			{
				this->CreateGradientByCircle();
				InvalidateRect(this->imWindow,NULL,false);
			}
			break;
	}

	DrawGradient();
}

void Document::SetInd(UINT ind){
	this->ind = ind;
	im->CreateGr(fmp,lmp,this->ind,wndImSize);
	DrawGradient();
}

void Document::SetColorsFromBitmap(){
	switch(this->mode){
		case BTM_LINE:
			im->SetGradientColors(lmp,fmp-lmp);
			break;
		case BTM_POINT:
			{
				im->SetGradientColors(grPtTable);
			}
			break;
	}
	DrawGradient();
}

void Document::Save(HWND hWnd){
	HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWL_HINSTANCE);
	dlgOpenSaveFileName ofn(hInstance,"Bitmapa (*.jpg)\0*.jpg\0Bitmapa (*.bmp)\0*.bmp\0Bitmapa (*.png)\0*.png\0");
	if(ofn.ShowSaveFileName()){
		this->im->Save(ofn.GetFileName());
	}
}

void Document::DrawGradient(){
	HWND p = GetParent(this->imWindow);
	HDC hdc = GetDC(p);
	im->gr.gr.StretchBltB(hdc,0,HEIGHT,this->wndImSize.right - wndImSize.left + RULER_HEIGHT,HEIGHT);
	ReleaseDC(p,hdc);

	InvalidateRect(this->grPtWindow, NULL, false);
}

void Document::CreateGradientByCircle(){ // Tworzy gradient po okrêgu
	double rx = abs(lmp.x - fmp.x);
	double ry = abs(lmp.y - fmp.y);

	grPtTable.clear();
	POINT b = {rx + lmp.x, lmp.y};

	im->NormalizePoint(b);
	grPtTable.push_back(grPoint(b,b));
	for(UINT i = 1; i < ind; i++){
		b.x = rx * cos(2. * M_PI / (ind - 1) * i) + lmp.x;
		b.y = ry * sin(2. * M_PI / (ind - 1) * i) + lmp.y;
		im->NormalizePoint(b);
		grPtTable.push_back(grPoint(b, b));
	}
	im->CreateGr(grPtTable);
}

void Document::CreateGradientPngFile(HWND hWnd){

	char buffer[MAX_PATH];

	BROWSEINFO bi;
	bi.hwndOwner=hWnd;
	bi.pidlRoot=0; //pidlRoot;
	bi.pszDisplayName=buffer;
	bi.lpszTitle = "Wska¿ folder, w kórym znajduj¹ siê pliki gradientów *.ggr\nSelect the folder where the *.ggr files are.";
	bi.ulFlags=BIF_USENEWUI;
	bi.lpfn=NULL;
	bi.lParam=NULL;
	bi.iImage=0;

	ITEMIDLIST *pidl=SHBrowseForFolder(&bi);

	if(pidl){
		char p[MAX_PATH];
		SHGetPathFromIDList(pidl,p);
		//MessageBox(hWnd,path,buffer,MB_OK);
		struct FILE_AND_PATH{
			std::string path;
			std::string file;
		};
		std::vector<FILE_AND_PATH> tGrP;
		_finddata64i32_t fd;
		intptr_t hFile;
		std::string copy;
		copy = p;
		copy += "\\";
		std::string str = p;
		str += "\\*.ggr";
		hFile = _findfirst(str.c_str(),&fd);
		if(hFile == -1L){
		}else{
			do
			{
				std::string copy2;
				copy2 = copy + fd.name;
				//gr.SetWidth(800);
				FILE_AND_PATH fap;
				fap.path = copy2;
				fap.file = fd.name;
				tGrP.push_back(fap);
			}while(_findnext(hFile,&fd) == 0);
			if(tGrP.size()){
				ATL::CImage tIm;
				UINT width = 1000u;
				UINT n = min((UINT)tGrP.size(), 200u);
				tIm.Create(width,50 * n,24,0);
				HBITMAP hbmp = tIm;//CreateCompatibleBitmap(GetDC(NULL),800,50 * tGr.size());
				HDC hdc = CreateCompatibleDC(GetDC(NULL));
				SelectObject(hdc, hbmp);
				SelectObject(hdc, textFont);
				
				RECT r;
				SetRect(&r, 0, 0, width,50 * n);
				FillRect(hdc, &r,(HBRUSH) GetStockObject(WHITE_BRUSH));
				std::string str;
				for(UINT i = 0; i < n; i++){
					gradient gr;
					gr.SetWidth(width);
					gr.LoadFile(tGrP[i].path);
					str = tGrP[i].file;
					str += " Wewnêtrzna nazwa (insert name): ";
					str += gr.gname;
					TextOut(hdc, 0, i * 50, str.c_str(),(int) str.size());
					gr.gr.BitBltB(hdc,0,18 + i * 50);
				}
				//tIm = hbmp;
				std::string pf = p;
				pf += "\\Gradienty.png";
				tIm.Save(pf.c_str(), Gdiplus::ImageFormatPNG);
				DeleteDC(hdc);
				DeleteObject(hbmp);
				ShellExecute(NULL,"open",pf.c_str(),NULL,NULL,SW_SHOWNORMAL);
			}
			_findclose( hFile );
		}
	}
}

void Document::CreateGradientPngFile(HWND hWnd, std::vector<std::string> &tPath){

	if(tPath.size()){
		ATL::CImage tIm;
		UINT width = 1000u;
		UINT n = min((UINT)tPath.size(), 200u);
		tIm.Create(width,50 * n,24,0);
		HBITMAP hbmp = tIm;//CreateCompatibleBitmap(GetDC(NULL),800,50 * tGr.size());
		HDC hdc = CreateCompatibleDC(GetDC(NULL));
		SelectObject(hdc, hbmp);
		SelectObject(hdc, textFont);
		
		RECT r;
		SetRect(&r, 0, 0, width,50 * n);
		FillRect(hdc, &r,(HBRUSH) GetStockObject(WHITE_BRUSH));
		std::string str;
		for(UINT i = 0; i < n; i++){
			gradient gr;
			gr.SetWidth(width);
			gr.LoadFile(tPath[i]);
			str = tPath[i];
			std::basic_string<char>::size_type k = str.find_last_of('\\',-1);
			str.erase(str.begin(), str.begin() + k + 1);
			str += " Wewnêtrzna nazwa (insert name): ";
			str += gr.gname;
			TextOut(hdc, 0, i * 50, str.c_str(),(int) str.size());
			gr.gr.BitBltB(hdc,0,18 + i * 50);
		}
		HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWL_HINSTANCE);
		dlgOpenSaveFileName ofn(hInstance,"PNG (*.png)\0*.png\0");
		if(ofn.ShowSaveFileName()){
			tIm.Save(ofn.GetFileName(), Gdiplus::ImageFormatPNG);
			ShellExecute(NULL,"open",ofn.GetFileName(),NULL,NULL,SW_SHOWNORMAL);
		}
		DeleteDC(hdc);
		DeleteObject(hbmp);		
	}
}

void Document::CreateGradientSvgFile(HWND hWnd){

	char buffer[MAX_PATH];

	BROWSEINFO bi;
	bi.hwndOwner=hWnd;
	bi.pidlRoot=0;//pidlRoot;
	bi.pszDisplayName=buffer;
	bi.lpszTitle="Wska¿ folder, w kórym znajduj¹ siê pliki gradientów *.ggr\nSelect the folder where the *.ggr files are.";
	bi.ulFlags=BIF_USENEWUI;
	bi.lpfn=NULL;
	bi.lParam=NULL;
	bi.iImage=0;

	ITEMIDLIST *pidl=SHBrowseForFolder(&bi);

	if(pidl){
		char p[MAX_PATH];
		SHGetPathFromIDList(pidl,p);
		//MessageBox(hWnd,path,buffer,MB_OK);
		struct FILE_AND_PATH{
			std::string path;
			std::string file;
		};
		std::vector<FILE_AND_PATH> tGrP;
		_finddata64i32_t fd;
		intptr_t hFile;
		std::string copy;
		copy = p;
		copy += "\\";
		std::string str = p;
		str += "\\*.ggr";
		hFile = _findfirst(str.c_str(),&fd);
		if(hFile == -1L){
		}else{
			do
			{
				std::string copy2;
				copy2 = copy + fd.name;
				FILE_AND_PATH fap;
				fap.path = copy2;
				fap.file = fd.name;
				tGrP.push_back(fap);
			}while(_findnext(hFile,&fd) == 0);

			if(tGrP.size()){
				std::string str;
				std::string gDefs;
				std::string gObj;
				UINT n = min((UINT)tGrP.size(), 200u);
				for(UINT i = 0; i < n; i++){
					gradient gr;
					//MessageBox(NULL,tGrP[i].path.c_str(),"dsds",MB_OK);
					gr.LoadFile(tGrP[i].path);
					if(gr.tSegment.size()){
						gDefs	+= gr.GetSvgGradientDefinition(gr.gname);
						gObj	+= gr.GetSvgGradientObject(gr.gname, i, (i % 7) * 100, i / 7 * 100, 100, 100);
					}else{
						std::string error = "B£¥D (ERROR): coœ nie tak z defnicj¹ pliku (something wrong with the definition file)";
						error += tGrP[i].path;
						MessageBox(hWnd,error.c_str(), "B³¹d (error)",MB_OK);
					}
				}

				std::string s;
				s = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n<!-- Created with Inkscape (http://www.inkscape.org/) -->\n\n";
				s += "<svg";
				s += char(10);
				s += "xmlns:dc=\"http://purl.org/dc/elements/1.1/\"";
				s += char(10);
				s += "xmlns:cc=\"http://creativecommons.org/ns#\"";
				s += char(10);
				s += "xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"";
				s += char(10);
				s += "xmlns:svg=\"http://www.w3.org/2000/svg\"";
				s += char(10);
				s += "xmlns=\"http://www.w3.org/2000/svg\"";
				s += char(10);
				s += "xmlns:xlink=\"http://www.w3.org/1999/xlink\"";
				s += char(10);
				s += "xmlns:sodipodi=\"http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd\"";
				s += char(10);
				s += "xmlns:inkscape=\"http://www.inkscape.org/namespaces/inkscape\"";
				s += char(10);
				s += "width=\"744.09448819\"";
				s += char(10);
				s += "height=\"1052.3622047\"";
				s += char(10);
				s += "id=\"svg2\"";
				s += char(10);
				s += "version=\"1.1\"";
				s += char(10);
				s += "inkscape:version=\"0.48.0 r9654\"";
				s += char(10);
				s += "sodipodi:docname=\"Nowy dokument 1\">";
				s += char(10);
				s += "<defs";
				s += char(10);
				s += "id=\"defs4\">";
				s += char(10);

				s += gDefs;

				s += "</defs>";
				s += char(10);

				s += "<sodipodi:namedview";
				s += char(10);
				s += "id=\"base\"";
				s += char(10);
				s += "pagecolor=\"#ffffff\"";
				s += char(10);
				s += "bordercolor=\"#666666\"";
				s += char(10);
				s += "borderopacity=\"1.0\"";
				s += char(10);
				s += "inkscape:pageopacity=\"0.0\"";
				s += char(10);
				s += "inkscape:pageshadow=\"2\"";
				s += char(10);
				s += "inkscape:zoom=\"0.98994949\"";
				s += char(10);
				s += "inkscape:cx=\"210.46193\"";
				s += char(10);
				s += "inkscape:cy=\"780.98887\"";
				s += char(10);
				s += "inkscape:document-units=\"px\"";
				s += char(10);
				s += "inkscape:current-layer=\"layer1\"";
				s += char(10);
				s += "showgrid=\"false\"";
				s += char(10);
				s += "inkscape:window-width=\"1440\"";
				s += char(10);
				s += "inkscape:window-height=\"820\"";
				s += char(10);
				s += "inkscape:window-x=\"-8\"";
				s += char(10);
				s += "inkscape:window-y=\"-8\"";
				s += char(10);
				s += "inkscape:window-maximized=\"1\" />";
				s += char(10);

				s += "<metadata";
				s += char(10);
				s += "id=\"metadata7\">";

				s += "<rdf:RDF>";
				s += "<cc:Work rdf:about=\"\">";
				s += "<dc:format>image/svg+xml</dc:format>";
				s += "<dc:type rdf:resource=\"http://purl.org/dc/dcmitype/StillImage\" />";
				s += "<dc:title />";
				s += "</cc:Work>";
				s += "</rdf:RDF>";
				s += "</metadata>";
				s += "<g inkscape:label=\"Layer 1\" inkscape:groupmode=\"layer\" id=\"layer1\">";

				s += gObj;

				s += "</g>";
				s += "</svg>";

				std::string path = p;
				path += "\\Gradienty.svg";

				std::fstream file(path.c_str(),std:: ios_base::out);

				file.write(s.c_str(), s.size());

				file.close();
				ShellExecute(NULL,"open",path.c_str(),NULL,NULL,SW_SHOWNORMAL);

			}
			_findclose( hFile );
		}
	}
}

void Document::CreateGradientSvgFile(HWND hWnd, std::vector<std::string> &tPath){

	if(tPath.size()){
		std::string str;
		std::string gDefs;
		std::string gObj;
		UINT n = min((UINT)tPath.size(), 200u);
		for(UINT i = 0; i < n; i++){
			gradient gr;
			//MessageBox(NULL,tGrP[i].path.c_str(),"dsds",MB_OK);
			gr.LoadFile(tPath[i]);
			if(gr.tSegment.size()){
				gDefs	+= gr.GetSvgGradientDefinition(gr.gname);
				gObj	+= gr.GetSvgGradientObject(gr.gname, i, (i % 7) * 100, i / 7 * 100, 100, 100);
			}else{
				std::string error = "B£¥D (ERROR): coœ nie tak z defnicj¹ pliku (something wrong with the definition file)";
				error += tPath[i];
				MessageBox(hWnd,error.c_str(), "B³¹d (error)",MB_OK);
			}
		}

		std::string s;
		s = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n<!-- Created with Inkscape (http://www.inkscape.org/) -->\n\n";
		s += "<svg";
		s += char(10);
		s += "xmlns:dc=\"http://purl.org/dc/elements/1.1/\"";
		s += char(10);
		s += "xmlns:cc=\"http://creativecommons.org/ns#\"";
		s += char(10);
		s += "xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"";
		s += char(10);
		s += "xmlns:svg=\"http://www.w3.org/2000/svg\"";
		s += char(10);
		s += "xmlns=\"http://www.w3.org/2000/svg\"";
		s += char(10);
		s += "xmlns:xlink=\"http://www.w3.org/1999/xlink\"";
		s += char(10);
		s += "xmlns:sodipodi=\"http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd\"";
		s += char(10);
		s += "xmlns:inkscape=\"http://www.inkscape.org/namespaces/inkscape\"";
		s += char(10);
		s += "width=\"744.09448819\"";
		s += char(10);
		s += "height=\"1052.3622047\"";
		s += char(10);
		s += "id=\"svg2\"";
		s += char(10);
		s += "version=\"1.1\"";
		s += char(10);
		s += "inkscape:version=\"0.48.0 r9654\"";
		s += char(10);
		s += "sodipodi:docname=\"Nowy dokument 1\">";
		s += char(10);
		s += "<defs";
		s += char(10);
		s += "id=\"defs4\">";
		s += char(10);

		s += gDefs;

		s += "</defs>";
		s += char(10);

		s += "<sodipodi:namedview";
		s += char(10);
		s += "id=\"base\"";
		s += char(10);
		s += "pagecolor=\"#ffffff\"";
		s += char(10);
		s += "bordercolor=\"#666666\"";
		s += char(10);
		s += "borderopacity=\"1.0\"";
		s += char(10);
		s += "inkscape:pageopacity=\"0.0\"";
		s += char(10);
		s += "inkscape:pageshadow=\"2\"";
		s += char(10);
		s += "inkscape:zoom=\"0.98994949\"";
		s += char(10);
		s += "inkscape:cx=\"210.46193\"";
		s += char(10);
		s += "inkscape:cy=\"780.98887\"";
		s += char(10);
		s += "inkscape:document-units=\"px\"";
		s += char(10);
		s += "inkscape:current-layer=\"layer1\"";
		s += char(10);
		s += "showgrid=\"false\"";
		s += char(10);
		s += "inkscape:window-width=\"1440\"";
		s += char(10);
		s += "inkscape:window-height=\"820\"";
		s += char(10);
		s += "inkscape:window-x=\"-8\"";
		s += char(10);
		s += "inkscape:window-y=\"-8\"";
		s += char(10);
		s += "inkscape:window-maximized=\"1\" />";
		s += char(10);

		s += "<metadata";
		s += char(10);
		s += "id=\"metadata7\">";

		s += "<rdf:RDF>";
		s += "<cc:Work rdf:about=\"\">";
		s += "<dc:format>image/svg+xml</dc:format>";
		s += "<dc:type rdf:resource=\"http://purl.org/dc/dcmitype/StillImage\" />";
		s += "<dc:title />";
		s += "</cc:Work>";
		s += "</rdf:RDF>";
		s += "</metadata>";
		s += "<g inkscape:label=\"Layer 1\" inkscape:groupmode=\"layer\" id=\"layer1\">";

		s += gObj;

		s += "</g>";
		s += "</svg>";

		HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWL_HINSTANCE);
		dlgOpenSaveFileName ofn(hInstance,"SVG (*.svg)\0*.svg\0");
		if(ofn.ShowSaveFileName()){
			std::string path = ofn.GetFileName();

			std::fstream file(path.c_str(),std:: ios_base::out);

			file.write(s.c_str(), s.size());

			file.close();
			ShellExecute(NULL,"open",path.c_str(),NULL,NULL,SW_SHOWNORMAL);
		}

	}
}

void ClearText(std::string &text){
	if(text.size()){
		while(text[0]==' ' && text.size()){
			text.erase(text.begin());
		}
	}
}

void Document::ExportGradientFromGgrToCss(HWND hWnd){
	char buffer[MAX_PATH];

	BROWSEINFO bi;
	bi.hwndOwner=hWnd;
	bi.pidlRoot=0;//pidlRoot;
	bi.pszDisplayName=buffer;
	bi.lpszTitle="Wska¿ folder, w kórym znajduj¹ siê pliki gradientów *.ggr\nSelect the folder where the *.ggr files are.";
	bi.ulFlags=BIF_USENEWUI;
	bi.lpfn=NULL;
	bi.lParam=NULL;
	bi.iImage=0;

	ITEMIDLIST *pidl=SHBrowseForFolder(&bi);

	if(pidl){
		char p[MAX_PATH];
		SHGetPathFromIDList(pidl,p);
		//MessageBox(hWnd,path,buffer,MB_OK);
		struct FILE_AND_PATH{
			std::string path;
			std::string file;
		};
		std::vector<FILE_AND_PATH> tGrP;
		_finddata64i32_t fd;
		intptr_t hFile;
		std::string copy;
		copy = p;
		copy += "\\";
		std::string str = p;
		str += "\\*.ggr";
		hFile = _findfirst(str.c_str(),&fd);

		std::string cssGradient;

		if(hFile == -1L){
		}else{
			do
			{
				std::string copy2;
				copy2 = copy + fd.name;
				FILE_AND_PATH fap;
				fap.path = copy2;
				fap.file = fd.name;
				tGrP.push_back(fap);
			}while(_findnext(hFile,&fd) == 0);

			if(tGrP.size()){
				UINT n = min((UINT)tGrP.size(), 200u);
				std::string html;
				html += "<!DOCTYPE html\nPUBLIC \"-//W3C//DTD XHTML 1.0 Frameset//EN\"\n\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd\">\n<html>\n<head>\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-2\" />\n<meta name=\"Description\" content=\"Podgl¹d gradientów (gradients preview)\" />\n<meta name=\"Keywords\" content=\"\" />\n<meta name=\"Author\" content=\"Krzysztof Zaj¹czkowski\" />\n<link rel = \"stylesheet\" type = \"text/css\" href = \"gradienty.css\"/>\n<title>Gradienty (Gradients)</title>\n</head>\n<body>\n";
				cssGradient += "h2{\ncolor:#fff;\nmargin:0;\npadding:1em;\nborder-bottom:1px solid #888;\n}\n";

				for(UINT i = 0; i < n; i++){
					gradient gr;
					//MessageBox(NULL,tGrP[i].path.c_str(),"dsds",MB_OK);
					gr.LoadFile(tGrP[i].path);
					if(gr.tSegment.size()){
						cssGradient += gr.GetCssGradientString();
						html += gr.GetHtmlGradientDef(tGrP[i].path);
					}else{
						std::string error = "B£¥D (ERROR): coœ nie tak z defnicj¹ pliku (something wrong with the definition file)";
						error += tGrP[i].path;
						MessageBox(hWnd,error.c_str(), "B³¹d (error)",MB_OK);
					}
				}
				html += "</body>\n</html>";

				std::string path = p;
				path += "\\Gradienty.css";
				std::string path2 = p;
				path2 += "\\Gradienty.html";

				std::fstream file(path.c_str(),std:: ios_base::out);

				file.write(cssGradient.c_str(), cssGradient.size());

				file.close();

				std::fstream fileHtml(path2.c_str(), std::ios_base::out);
				fileHtml.write(html.c_str(), html.size());

				fileHtml.close();

				ShellExecute(NULL,"open",path.c_str(),NULL,NULL,SW_SHOWNORMAL);
				ShellExecute(NULL,"open",path2.c_str(),NULL,NULL,SW_SHOWNORMAL);
			}
			_findclose( hFile );
		}
	}
}

void Document::ExportGradientFromGgrToCss(HWND hWnd, std::vector<std::string> &tPath){
	if(tPath.size()){
		HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWL_HINSTANCE);
		dlgOpenSaveFileName ofn(hInstance,"CSS (*.css)\0*.css\0");
		if(ofn.ShowSaveFileName()){
			UINT n = min((UINT)tPath.size(), 200u);
			std::string html = "<!DOCTYPE html\nPUBLIC \"-//W3C//DTD XHTML 1.0 Frameset//EN\"\n\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd\">\n<html>\n<head>\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-2\" />\n<meta name=\"Description\" content=\"Podgl¹d gradientów (gradients preview)\" />\n<meta name=\"Keywords\" content=\"\" />\n<meta name=\"Author\" content=\"Krzysztof Zaj¹czkowski\" />\n<link rel = \"stylesheet\" type = \"text/css\" href = \"";
			html += ofn.GetTitleName();
			html += "\"/>\n<title>Gradienty (Gradients)</title>\n</head>\n<body>\n";
			std::string cssGradient = "h2{\ncolor:#fff;\nmargin:0;\npadding:1em;\nborder-bottom:1px solid #888;\n}\n";

			for(UINT i = 0; i < n; i++){
				gradient gr;
				//MessageBox(NULL,tGrP[i].path.c_str(),"dsds",MB_OK);
				gr.LoadFile(tPath[i]);
				if(gr.tSegment.size()){
					cssGradient += gr.GetCssGradientString();
					html += gr.GetHtmlGradientDef(tPath[i]);
				}else{
					std::string error = "B£¥D (ERROR): coœ nie tak z defnicj¹ pliku (something wrong with the definition file)";
					error += tPath[i];
					MessageBox(hWnd,error.c_str(), "B³¹d (error)",MB_OK);
				}
			}
			
			html += "</body>\n</html>";

			std::string path = ofn.GetFileName();

			std::string path2 = path;

			std::basic_string<char>::size_type i = path.find_last_of('\\',-1);

			//path.erase(path.begin() + i + 1,path.end());

			path2.erase(path2.begin() + (int)path2.size() - 3, path2.end());
			path2 += "html";

			std::fstream file(path.c_str(),std:: ios_base::out);

			file.write(cssGradient.c_str(), cssGradient.size());

			file.close();

			std::fstream fileHtml(path2.c_str(), std::ios_base::out);
			fileHtml.write(html.c_str(), html.size());

			fileHtml.close();

			ShellExecute(NULL,"open",path.c_str(),NULL,NULL,SW_SHOWNORMAL);
			ShellExecute(NULL,"open",path2.c_str(),NULL,NULL,SW_SHOWNORMAL);
		}
	}
}

void Document::ExportGradientFromSvgToGgr(HWND hWnd){
	dlgOpenSaveFileName ofn((HINSTANCE)GetWindowLongPtr(hWnd, GWL_HINSTANCE),"svg (*.svg)\0*.svg\0");
	if(ofn.ShowOpenFileName()){
		std::fstream file(ofn.GetFileName(),std::ios_base::in);

		std::string s;
		bool w = false;

		std::vector<std::string> tText;

		if(!file.fail()){
			do{
				std::getline(file,s);
				if(w){
					//std::cout<<s.c_str()<<std::endl;
					ClearText(s);
					tText.push_back(s);
				}else{
					ClearText(s);
					w = !s.compare("<defs");
					if(w)
						tText.push_back(s);
				}
			}while(!file.fail()?s.compare("</defs>"):0);
		}else{
			  MessageBox(hWnd, "ERROR nr. 1: Spowodowany przez czynnik ludzki: zla scieska do pliku svg", "ERROR", MB_OK);
			  return ;
		}

		file.close();

		if(!tText.size()){
			  MessageBox(hWnd, "ERROR nr. 2: Cos nie tak z plikiem", "ERROR", MB_OK);
			  return ;
		}
		
		c_xml x(tText);
		x.EraseRekordIfNameNotMatch(std::string("linearGradient"));
		x.EraseRekordIfDontHaveChild();
		
		if(!x.xmlT.size()){
			  MessageBox(hWnd, "ERROR nr. 3: Nie odnaleziono deklaracji gradientow", "ERROR", MB_OK);
			  return ;
		}

		char buffer[MAX_PATH];

		BROWSEINFO bi;
		bi.hwndOwner=hWnd;
		bi.pidlRoot=0;//pidlRoot;
		bi.pszDisplayName=buffer;
		bi.lpszTitle="Wska¿ folder, w kórym znajduj¹ siê pliki gradientów *.ggr\nSelect the folder where the *.ggr files are.";
		bi.ulFlags=BIF_USENEWUI;
		bi.lpfn=NULL;
		bi.lParam=NULL;
		bi.iImage=0;

		ITEMIDLIST *pidl=SHBrowseForFolder(&bi);

		if(pidl){
			char p[MAX_PATH];
			SHGetPathFromIDList(pidl,p);

			gradient g;

			for(int i = 0; i < x.xmlT.size(); i++){
				g.GradientFromXml(x, i);
				s = p;
				s += '\\';
				s += g.gname;
				s += ".ggr";
				
				std::fstream file(s.c_str(),std::ios_base::out);

				s.clear();

				s = "GIMP Gradient";
				s += char(10u);
				s += "Name: ";
				s += g.gname;
				s += char(10u);
				char b2[200];
				sprintf(b2,"%i",g.tSegment.size());
				s += b2;
				s += char(10u);
				
				s += g.GradientToString();

				file.write(s.c_str(), s.size());

				file.close();
			}
		}
	}
}

void Document::WmSize(HWND hWnd){
	GetClientRect(hWnd,&wndImSize);

	wndImSize.right		-= RULER_HEIGHT;
	wndImSize.bottom	-= RULER_HEIGHT * 2 + HEIGHT * 2;

	this->vrc.SetRulerSize(RULER_HEIGHT,wndImSize.bottom/* - GetSystemMetrics(SM_CYHSCROLL)*/);
	this->hrc.SetRulerSize(wndImSize.right,RULER_HEIGHT);

	SetWindowPos(imWindow,NULL,RULER_HEIGHT,RULER_HEIGHT * 2 + HEIGHT * 2,wndImSize.right,wndImSize.bottom,SWP_NOZORDER);
	SetWindowPos(grPtWindow, NULL, 0, HEIGHT * 2, wndImSize.right + RULER_HEIGHT, RULER_HEIGHT, SWP_NOZORDER);
	im->gr.SetWidth(wndImSize.right + RULER_HEIGHT);
	DeleteObject(ImageWindowBitmap);
	DeleteObject(grPtBitmap);
	HDC hdc = GetDC(hWnd);
	ImageWindowBitmap = CreateCompatibleBitmap(hdc,wndImSize.right,wndImSize.bottom);
	grPtBitmap = CreateCompatibleBitmap(hdc, wndImSize.right + RULER_HEIGHT, RULER_HEIGHT);
	DeleteDC(hdc);

	//wndImSize.right		-= GetSystemMetrics(SM_CXSIZEFRAME);
	//wndImSize.bottom	-= GetSystemMetrics(SM_CYSIZEFRAME);

	if(im->bmpWidth() <= wndImSize.right){
		im->X((wndImSize.right - im->bmpWidth() - (im->bmpHeight()>wndImSize.bottom?GetSystemMetrics(SM_CYVSCROLL):0)) / 2);
		ShowScrollBar(imWindow,SB_HORZ,false);
	}else{
		im->X(0);
		ShowScrollBar(imWindow,SB_HORZ,true);
		SCROLLINFO si;
		si.cbSize		= sizeof(SCROLLINFO);
		si.fMask		= SIF_PAGE|SIF_POS|SIF_RANGE;
		si.nMax			= int(im->nBmpWidth() - wndImSize.right / im->Scale() + (im->bmpHeight()>wndImSize.bottom?GetSystemMetrics(SM_CYVSCROLL) + 10:10)) /*nPage = 10*/;
		si.nMin			= 0;
		si.nPage		= 10;
		si.nPos			= 0;
		SetScrollInfo(imWindow,SB_HORZ,&si,true);
	}
	if(im->bmpHeight() <= wndImSize.bottom){
		im->Y((wndImSize.bottom - im->bmpHeight() - (im->bmpWidth()>wndImSize.right?GetSystemMetrics(SM_CYHSCROLL):0)) / 2);
		ShowScrollBar(imWindow,SB_VERT,false);
	}else{
		im->Y(0);
		ShowScrollBar(imWindow,SB_VERT,true);
		SCROLLINFO si;
		si.cbSize		= sizeof(SCROLLINFO);
		si.fMask		= SIF_PAGE|SIF_POS|SIF_RANGE;
		si.nMax			= int(im->nBmpHeight() - wndImSize.bottom / im->Scale() + (im->bmpWidth()>wndImSize.right?GetSystemMetrics(SM_CYHSCROLL) + 10:10)) /*nPage = 10*/;
		si.nMin			= 0;
		si.nPage		= 10;
		si.nPos			= 0;
		SetScrollInfo(imWindow,SB_VERT,&si,true);
	}
	vRulerDataClass *vrdc = this->vrc.RClass();
	if(vrdc){
		vrdc->Y(im->Y());
		vrdc->Height(im->bmpHeight());
		vrdc->Scale(im->Scale());
		this->vrc.Invalidate();
	}
	hRulerDataClass *hrdc = this->hrc.RClass();
	if(hrdc){
		hrdc->X(im->X());
		hrdc->Width(im->bmpWidth());	
		hrdc->Scale(im->Scale());
		this->hrc.Invalidate();
	}
}

void Document::WmMouseWheel(HWND hWnd,WPARAM wParam,LPARAM lParam){
	int iAccumDelta = (SHORT)HIWORD(wParam);	// warzne jest rzutowanie na SHORT bo innaczej gubi znak (-)

	if(iAccumDelta >= this->iDeltaPerLine){
		im->Scale(2 * im->Scale());
		this->WmSize(GetParent(this->imWindow));
	}

	if(iAccumDelta <= this->iDeltaPerLine){
		im->Scale(0.5 * im->Scale());
		this->WmSize(GetParent(this->imWindow));
	}

	InvalidateRect(hWnd,NULL,false);
}

Document::~Document(){
	DeleteObject(ImageWindowBitmap);
	DeleteObject(grPtBitmap);
	if(im)
		delete im;
	scWnd.Destroy();
}

void Document::WmLButtonDown(POINT mp){
	scWnd.Destroy();
	im->WindowToImage(mp,wndImSize);
	switch(this->mode){
		case BTM_LINE:
			if((this->btHandClicked && !this->isPtOnLine) || !btHandClicked){
				lmp = mp;
				im->NormalizePoint(lmp);
			}
			break;
		case BTM_POINT:
			{
			}
			break;
		case BTM_CIRCLE:
			{
				lmp = mp;
				im->NormalizePoint(lmp);
			}
			break;
	}
}

void Document::WmLButtonUp(POINT mp,int mk){
	scWnd.Destroy();
	im->WindowToImage(mp,wndImSize);
	switch(mode){
		case BTM_LINE:
			if((this->btHandClicked && !this->isPtOnLine) || !btHandClicked)
			{			
				fmp = mp;
				im->NormalizePoint(fmp);
			}
			break;
		case BTM_POINT:
			{
				if(grPtTable.size() < 100 && selPt == -1){
					im->NormalizePoint(mp);
					grPtTable.push_back(grPoint(mp, mp));
					if((mk & MK_SHIFT) == MK_SHIFT){
						float k = 1.f / grPtTable.size() * (grPtTable.size() - 1);
						for(std::vector<segment>::iterator i = im->gr.tSegment.begin(); i < im->gr.tSegment.end(); i++){
							i->left.pos *= k;
							i->center.pos *= k;
							i->right.pos *= k;
						}

						BYTE *table =  (BYTE*)im->image.GetBits();
						float r = 1.f;
						float g = 1.f;
						float b = 1.f;
						float a = 1.f;
						if(im->image.GetBPP() == 24){
							r = float(table[mp.x * 3 + mp.y * im->image.GetPitch() + 2])/255;
							g = float(table[mp.x * 3 + mp.y * im->image.GetPitch() + 1])/255;
							b = float(table[mp.x * 3 + mp.y * im->image.GetPitch()])/255;
						}else if(im->image.GetBPP() == 32){
							r = float(table[mp.x * 4 + mp.y * im->image.GetPitch() + 2])/255;
							g = float(table[mp.x * 4 + mp.y * im->image.GetPitch() + 1])/255;
							b = float(table[mp.x * 4 + mp.y * im->image.GetPitch()])/255;
							a = float(table[mp.x * 4 + mp.y * im->image.GetPitch() + 3])/255;
						}

						im->gr.tSegment.push_back(segment((im->gr.tSegment.end() - 1)->right, sColor(r, g, b, a, 1.f)));
						im->gr.CreateGradientVector();
						DrawGradient();
					}else{
						im->CreateGr(grPtTable);
						DrawGradient();
					}
				}
				selPt = -1;
			}
			break;
		case BTM_CIRCLE:
			{
			}
			break;
	}
}

void Document::WmDblLeftClk(int mk){
	scWnd.Destroy();
	switch(mode){
		case BTM_LINE:
			{
				if(selPt != -1 && selPt < (int)im->gr.tSegment.size()){
					im->gr.DeletePoint(selPt);
					DrawGradient();
					selPt = -1;
					return ;
				}
				double dist = sqrt((rp - lmp) * (rp - lmp));
				double distC = sqrt((lmp - fmp) * (lmp - fmp));
				float k = float( dist / distC);
				if(k <= 1. && this->btHandClicked && this->isPtOnLine){
					if(im->GetBPP() == 24){
						BYTE *table = (BYTE*)im->image.GetBits();

						float r = float(table[rp.x * 3 + rp.y * im->image.GetPitch() + 2]) / 255.f;
						float g = float(table[rp.x * 3 + rp.y * im->image.GetPitch() + 1]) / 255.f;
						float b = float(table[rp.x * 3 + rp.y * im->image.GetPitch()]) / 255.f;
						float a = 1.f;

						im->AddPointToGradient(r,g,b,a,k);
						DrawGradient();
					}else if(im->GetBPP() == 32){
						BYTE *table = (BYTE*)im->image.GetBits();

						float r = float(table[rp.x * 4 + rp.y * im->image.GetPitch() + 2]) / 255.f;
						float g = float(table[rp.x * 4 + rp.y * im->image.GetPitch() + 1]) / 255.f;
						float b = float(table[rp.x * 4 + rp.y * im->image.GetPitch()]) / 255.f;
						float a = float(table[rp.x * 4 + rp.y * im->image.GetPitch() + 3]) / 255.f;

						im->AddPointToGradient(r,g,b,a,k);
						DrawGradient();
					}
				}
			}
			break;
		case BTM_POINT:
			{
				if(selPt > -1 && grPtTable.size() > 2){
					switch(lr){
						case 1:
							{
								if(selPt != 0 && selPt != grPtTable.size() - 1){
									grPtTable[selPt].left = grPtTable[selPt].right;
									sColor sc = im->gr.tSegment[selPt].left;
									im->gr.tSegment[selPt - 1].SetRightColorRGB(sc.r, sc. g, sc.b, sc.a);
									im->gr.CreateGradientVector();
									DrawGradient();
									InvalidateRect(this->imWindow, NULL, false);
								}
							}
							break;
						case 2:
							{
								if(selPt != 0 && selPt != grPtTable.size() - 1){
									grPtTable[selPt].right = grPtTable[selPt].left;
									sColor sc = im->gr.tSegment[selPt - 1].right;
									im->gr.tSegment[selPt].SetLeftColorRGB(sc.r, sc. g, sc.b, sc.a);
									im->gr.CreateGradientVector();
									DrawGradient();
									InvalidateRect(this->imWindow, NULL, false);
								}
							}
							break;
						case 3:
							{
								if((mk & MK_SHIFT) == MK_SHIFT){
									if(selPt > 0 && selPt < grPtTable.size() - 1){
										grPtTable.erase(grPtTable.begin() + selPt);
										im->gr.DeletePoint(selPt);
										DrawGradient();
										InvalidateRect(this->imWindow, NULL, false);
									}else{
										grPtTable.erase(grPtTable.begin() + selPt);
										if(selPt == 0){
											grPtTable[0].left = grPtTable[0].right;
											im->gr.tSegment.erase(im->gr.tSegment.begin());
											float dx = im->gr.tSegment[0].left.pos;
											float k = 1.f - dx;
											for(std::vector<segment>::iterator i = im->gr.tSegment.begin(); i < im->gr.tSegment.end(); i++){
												i->left.pos		-= dx;
												i->left.pos		/= k;
												i->right.pos	-= dx;
												i->right.pos	/= k;
												i->center.pos	-= dx;
												i->center.pos	/= k;
											}
										}else if(selPt == grPtTable.size()){
											grPtTable[grPtTable.size() - 1].right = grPtTable[grPtTable.size() - 1].left;
											im->gr.tSegment.erase(im->gr.tSegment.end()-1);
											float k = (im->gr.tSegment.end() - 1)->right.pos;
											for(std::vector<segment>::iterator i = im->gr.tSegment.begin(); i < im->gr.tSegment.end(); i++){
												i->left.pos		/= k;
												i->right.pos	/= k;
												i->center.pos	/= k;
											}
										}
										im->gr.CreateGradientVector();
										DrawGradient();
										InvalidateRect(this->imWindow, NULL, false);
									}
								}else{
									grPtTable.erase(grPtTable.begin() + selPt);
									if(selPt == 0)
										grPtTable[0].left = grPtTable[0].right;
									else if(selPt == grPtTable.size() - 1){
										grPtTable[grPtTable.size() - 1].right = grPtTable[grPtTable.size() - 1].left;
									}
									im->CreateGr(grPtTable);
									DrawGradient();
									InvalidateRect(this->imWindow, NULL, false);
								}
							}
							break;
					}
				}
			}
			break;
	}
}

void Document::WmMouseMove(POINT mp, UINT MK){
	im->WindowToImage(mp,wndImSize);
	switch(mode){
		case BTM_LINE:
			{
				rp = GetPointOnLine(fmp, lmp, mp);
				if(!(MK & MK_LBUTTON)){
					this->selPt = this->im->gr.MouseInPt(rp, lmp, fmp - lmp,im->Scale());

					if((rp - mp) * (rp - mp) <= 100 / im->Scale()){
						if(min(lmp.x,fmp.x) > rp.x || max(lmp.x, fmp.x) < rp.x){
							isPtOnLine = false;
						}else
							isPtOnLine = true;
					}else
						isPtOnLine = false;
				}
				
				if((MK & MK_LBUTTON) && ((this->btHandClicked && !this->isPtOnLine) || !btHandClicked)){
					fmp = mp;
					im->NormalizePoint(fmp);
					if(btHandClicked){
						im->SetGradientColors(lmp,fmp-lmp);
					}else
						im->CreateGr(fmp,lmp,ind,this->wndImSize);
				}
			}
			break;
		case BTM_POINT:
			{
				if(!(MK & MK_LBUTTON) && !(MK & MK_RBUTTON)){
					double scale = H_SIZE * H_SIZE / im->Scale();
					selPt = -1;
					lr = 0;
					for(std::vector<grPoint>::iterator i = this->grPtTable.begin(); i < this->grPtTable.end(); i++){
						if((mp - i->left) * (mp - i->left) <= scale){
							selPt = int(i - grPtTable.begin());
							if(i->left != i->right)
								lr = 1;
							else
								lr = 3;
							break;
						}else if((mp - i->right) * (mp - i->right) <= scale){
							selPt = int(i - grPtTable.begin());
							lr = 2;
							break;
						}
					}
				}

				if((MK & MK_LBUTTON) && selPt > -1 && (lr == 1 || lr == 3)){
					im->NormalizePoint(mp);
					if(selPt == 0){
						grPtTable.begin()->left = grPtTable.begin()->right = mp;
					}else if(selPt == grPtTable.size() - 1){
						(grPtTable.end() - 1)->right = (grPtTable.end() - 1)->left = mp;
					}else{
						if((grPtTable.begin() + selPt)->left != ((grPtTable.begin() + selPt)->right)){
							(grPtTable.begin() + selPt)->left = mp;
						}else{
							(grPtTable.begin() + selPt)->left = (grPtTable.begin() + selPt)->right = mp;
						}
					}
					im->SetGradientColor(grPtTable[selPt], selPt);
					this->DrawGradient();
				}else if((MK & MK_RBUTTON) && selPt > -1 && lr > 1){
					im->NormalizePoint(mp);
					if(selPt == 0){
						grPtTable.begin()->left = grPtTable.begin()->right = mp;
					}else if(selPt == grPtTable.size() - 1){
						(grPtTable.end() - 1)->right = (grPtTable.end() - 1)->left = mp;
					}else{
						(grPtTable.begin() + selPt)->right = mp;
					}
					im->SetGradientColor(grPtTable[selPt], selPt);
					this->DrawGradient();
				}
			}
			break;
		case BTM_CIRCLE:
			{
				if(MK & MK_LBUTTON){
					//im->NormalizePoint(mp);
					if(MK & MK_CONTROL){
						fmp = mp;
						int d = min(abs(lmp.x - fmp.x),abs(lmp.y - fmp.y));
						fmp.x = lmp.x + d * (fmp.x - lmp.x < 0 ? -1 : 1);
						fmp.y = lmp.y + d * (fmp.y - lmp.y < 0 ? -1 : 1);
					}else{
						fmp = mp;
					}

					this->CreateGradientByCircle();
				}
			}
	}
}

void Document::Draw(HDC hdc, UINT MK){
	im->Draw(hdc,wndImSize,fmp,lmp, MK, btHandClicked, mode);
	switch(mode){
		case BTM_LINE:
			{
				if(this->btHandClicked && isPtOnLine){
					POINT rp2 = im->ImageToWindow(rp.x, rp.y, wndImSize);
					Ellipse(hdc,rp2.x - H_SIZE, rp2.y -H_SIZE, rp2.x + H_SIZE, rp2.y + H_SIZE);
				}
			}
			break;
		case BTM_POINT:
			{
				SelectObject(hdc, textFont);
				HBRUSH hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
				HPEN hp = CreatePen(PS_SOLID,1,RGB(255,255,255));
				SelectObject(hdc,hp);
				SetROP2(hdc,R2_NOT);
				SelectObject(hdc,hbr);
				POINT left;
				POINT right;
				char c[7];
				SetBkMode(hdc, TRANSPARENT);
				int dy = 5;
				for(std::vector<grPoint>::iterator i = grPtTable.begin(); i < grPtTable.end(); i++){
					left = im->ImageToWindow(i->left.x, i->left.y,this->wndImSize);
					right = im->ImageToWindow(i->right.x, i->right.y,this->wndImSize);
					if(left != right){
						MoveToEx(hdc, left.x, left.y, NULL);
						LineTo(hdc, right.x, right.y);
						Ellipse(hdc, left.x - H_SIZE, left.y - H_SIZE, left.x + H_SIZE, left.y + H_SIZE);
						Ellipse(hdc, right.x - H_SIZE, right.y - H_SIZE, right.x + H_SIZE, right.y + H_SIZE);
						sprintf(c,"%iL",int(i - grPtTable.begin()) + 1);
						TextOut(hdc, left.x + 2 * H_SIZE, left.y + dy,c, (int)strlen(c));
						sprintf(c,"%iR",int(i - grPtTable.begin()) + 1);
						TextOut(hdc, right.x + 2 * H_SIZE, right.y + dy,c, (int)strlen(c));
					}else{
						Ellipse(hdc, right.x - H_SIZE, right.y - H_SIZE, right.x + H_SIZE, right.y + H_SIZE);
						sprintf(c,"%iL/R",int(i - grPtTable.begin()) + 1);
						TextOut(hdc, left.x + 2 * H_SIZE, left.y + dy,c, (int)strlen(c));
					}
				}
				DeleteObject(hbr);
				DeleteObject(hp);
			}
			break;
		case BTM_CIRCLE:
			{
				SelectObject(hdc, textFont);
				HBRUSH hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
				HPEN hp = CreatePen(PS_SOLID,1,RGB(255,255,255));
				SelectObject(hdc,hp);
				SetROP2(hdc,R2_NOT);
				SelectObject(hdc,hbr);
				POINT left;
				POINT right;
				char c[7];
				SetBkMode(hdc, TRANSPARENT);
				int dy = 5;
				left = im->ImageToWindow(fmp.x, fmp.y,this->wndImSize);
				right = im->ImageToWindow(lmp.x, lmp.y,this->wndImSize);
				Ellipse(hdc, right.x - abs(right.x - left.x), right.y - abs(right.y - left.y), right.x + abs(right.x - left.x), right.y + abs(right.y - left.y));
				POINT pt;
				for(std::vector<grPoint>::iterator i = grPtTable.begin(); i < grPtTable.end() - 1; i++){
					pt = im->ImageToWindow(i->left.x,i->left.y,wndImSize);
					Ellipse(hdc,pt.x-5,pt.y-5, pt.x+5,pt.y+5);
				}
				DeleteObject(hbr);
				DeleteObject(hp);
			}
			break;
	}
}

void Document::grPtWmPaint(HDC hdc){
	HDC mem = CreateCompatibleDC(hdc);

	SelectObject(mem, grPtBitmap);

	RECT r;
	SetRect(&r, 0, 0, wndImSize.right + RULER_HEIGHT, HEIGHT);
	SelectObject(mem, pnGray200);

	FillRect(mem, &r, brGray150);

	//if(grPtMouseIn){
	//	r.right = grPtMousePos.x;

	//	FillRect(mem, &r, brGray230);
	//}
	//im->gr.gr.StretchBltB(mem, 0, 0, wndImSize.right + RULER_HEIGHT, HEIGHT);

	im->gr.DrawGrPt(mem, wndImSize.right + RULER_HEIGHT,RULER_HEIGHT,grPtSel, grSgSel);

	BitBlt(hdc, 0, 0, wndImSize.right + RULER_HEIGHT, RULER_HEIGHT, mem, 0, 0, SRCCOPY);

	DeleteDC(mem);
}

void Document::grPtWmMouseMove(int x,int y,UINT MK){
	if(MK == MK_LBUTTON && (grSgSel > -1 || grPtSel > -1)){
		if(grPtSel > -1 && grPtSel < (int)im->gr.tSegment.size() - 1){
			float l = im->gr.tSegment[grPtSel].left.pos;
			float r = im->gr.tSegment[grPtSel + 1].right.pos;
			float pos = float(x) / (wndImSize.right + RULER_HEIGHT);
			pos = min(max(pos,l),r);
			im->gr.tSegment[grPtSel].SetRightPos(pos);
			im->gr.tSegment[grPtSel + 1].SetLeftPos(pos);
			im->gr.CreateGradientVector();
			this->DrawGradient();
		}else if(grSgSel > 0 && grSgSel < (int)im->gr.tSegment.size() - 1){
			float dx = float(x - grPtMousePos.x) / (wndImSize.right + RULER_HEIGHT);
			im->gr.MoveSegment(dx,grSgSel);
			im->gr.CreateGradientVector();
			this->DrawGradient();
		}
	}else{
		im->gr.FindGrPtAndSegmentIndex(grPtSel, grSgSel, x, wndImSize.right + RULER_HEIGHT, H_SIZE);
	}
	grPtMousePos.x = x;
	grPtMousePos.y = y;
	grPtMouseKey = MK;
	InvalidateRect(grPtWindow, NULL, false);
	grPtMouseIn = true;
}

void Document::grPtWmMouseLeave(){
	grPtMouseIn = false;
	grPtSel = -1;
	grSgSel = -1;
	InvalidateRect(grPtWindow, NULL, false);
}

void Document::grPtWmLButtonDown(){
	SetCapture(this->grPtWindow);
	scWnd.Destroy();
}
void Document::grPtWmLButtonUp(){
	ReleaseCapture();
}

void Document::grPtWmRButtonDown(){
	if(grPtSel != -1)
		scWnd.Create(grPtMousePos, this);
}


void Document::CreateScWnd(){
	switch(grPtSel){
		case -3:
			{
				scWnd.scInd = (int)this->im->gr.tSegment.size();
			}
			break;
		case -2:
			{
				scWnd.scInd = 0;
			}
			break;
		default:
			{
				scWnd.scInd = grPtSel + 1;
			}
			break;
	}
	im->gr.GetPointColor(scWnd.scInd, scWnd.left.r, scWnd.left.g, scWnd.left.b, scWnd.left.a, scWnd.right.r, scWnd.right.g, scWnd.right.b, scWnd.right.a);
	scWnd.cleft = cdib(265u, 25u, BYTE(scWnd.left.r * 255.f), BYTE(scWnd.left.g * 255.f), BYTE(scWnd.left.b * 255.f), scWnd.left.a);
	scWnd.cright = cdib(265u, 25u, BYTE(scWnd.right.r * 255.f), BYTE(scWnd.right.g * 255.f), BYTE(scWnd.right.b * 255.f), scWnd.right.a);

	SendMessage(scWnd.scLeftColor,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)scWnd.cleft.hBitmap);
	SendMessage(scWnd.scRightColor,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)scWnd.cright.hBitmap);
}

void Document::RefreshGradient(){
	int state = SendMessage(scWnd.scLeftColor, BM_GETSTATE, NULL, NULL);
	float r, g, b, a;
	r = state == BST_CHECKED ? scWnd.left.r : scWnd.right.r;
	g = state == BST_CHECKED ? scWnd.left.g : scWnd.right.g;
	b = state == BST_CHECKED ? scWnd.left.b : scWnd.right.b;
	a = state == BST_CHECKED ? scWnd.left.a : scWnd.right.a;

	scWnd.gred.SetRGBA_R(g, b, a);

	scWnd.ggreen.SetRGBA_G(r, b, a);

	scWnd.gblue.SetRGBA_B(r, g, a);

	scWnd.galpha.SetRGBA_A(r, g, b);

	float h, s, v;
	h = s = v = 0;

	RGB2HSV(h, s, v, r, g, b);

	scWnd.ghue.SetHSVA_H(s, v, a);

	scWnd.gsaturation.SetHSVA_S(h, v, a);

	scWnd.gvalue.SetHSVA_V(h, s, a);

	HDC hdc = GetDC(scWnd.scWnd);

	int pos = 62;

	pos += 50;

	scWnd.gred.BitBltB(hdc, 14, pos);

	pos += 60;
	
	scWnd.ggreen.BitBltB(hdc, 14, pos);

	pos += 60;
	
	scWnd.gblue.BitBltB(hdc, 14, pos);

	pos += 60;

	scWnd.galpha.BitBltB(hdc, 14, pos);

	pos += 60;

	scWnd.ghue.BitBltB(hdc, 14, pos);

	pos += 60;

	scWnd.gsaturation.BitBltB(hdc, 14, pos);

	pos += 60;

	scWnd.gvalue.BitBltB(hdc, 14, pos);

	ReleaseDC(scWnd.scWnd, hdc);
}

Document::scWindows::scWindows():width(300u), height(520u),cleft(10u,10u), cright(10u,10u),gred(width - 28, 10u), ggreen(width - 28, 10u), gblue(width - 28, 10u), galpha(width - 28, 10u), ghue(width - 28, 10u), gsaturation(width - 28, 10u), gvalue(width - 28, 10u)
{
}

void Document::scWindows::Create(POINT pt, Document* doc){
	this->Destroy();

	ClientToScreen(doc->grPtWindow, &pt);
	pt.x = max(0,min(pt.x - (int)width / 2, GetSystemMetrics(SM_CXFULLSCREEN) - (int)width));
	wndPos = pt;
	HINSTANCE hinst = (HINSTANCE)GetWindowLong(doc->imWindow, GWL_HINSTANCE);
	this->scWnd = CreateWindow("setColorWindow", "Ustaw kolor (set color)", WS_POPUP|WS_CAPTION|WS_SYSMENU, pt.x, pt.y + 5, width + GetSystemMetrics(SM_CXFRAME) - 2, height + GetSystemMetrics(SM_CYSIZE), GetParent(doc->imWindow), NULL, hinst,(LPVOID)doc);

	scLeftColor = CreateWindow("BUTTON", "leftcolor", WS_CHILD|WS_VISIBLE|BS_BITMAP|BS_RADIOBUTTON, 10, 10, 265, 25, scWnd, (HMENU)ID_SC_LEFT_COLOR, hinst, NULL);
	scRightColor = CreateWindow("BUTTON", "rightcolor", WS_CHILD|WS_VISIBLE|BS_BITMAP|BS_RADIOBUTTON, 10, 35, 265, 25, scWnd, (HMENU)ID_SC_RIGHT_COLOR, hinst, NULL);
	scChainButton = CreateWindow("BUTTON", "chain", WS_CHILD|WS_VISIBLE|BS_BITMAP|BS_PUSHLIKE|BS_RADIOBUTTON|BS_FLAT, 275, 10, 15, 50, scWnd, (HMENU)ID_SC_CHAIN, hinst, NULL);

	hChainOpen = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_CH_OPEN));
	hChainClose = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_CH_CLOSE));

	SendMessage(scLeftColor, BM_SETCHECK, BST_CHECKED, 0);

	doc->CreateScWnd();

	if(left == right){
		SendMessage(scChainButton, BM_SETCHECK, BST_UNCHECKED, 0);
		SendMessage(scChainButton,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hChainClose);
	}else{
		SendMessage(scChainButton, BM_SETCHECK, BST_CHECKED, 0);
		SendMessage(scChainButton,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hChainOpen);
	}

	int ypos = 60 + 20;
	scRed.CreateWnd(scWnd, hinst, "Czerwony (red)", ID_SC_RED,10, ypos,width - 20,30);
	scRed.SetValue(double(left.r * 255.f));
	scRed.SetBkTextColor(RGB(255,200,200));
	ypos += 60;

	gred.SetRGBA_R(left.g, left.b, left.a);

	scGreen.CreateWnd(scWnd, hinst, "Zielony (green)", ID_SC_GREEN, 10, ypos, width - 20,30);
	scGreen.SetValue(double(left.g * 255.f));
	scGreen.SetBkTextColor(RGB(200,255,200));
	ypos += 60;

	ggreen.SetRGBA_G(left.r, left.b, left.a);

	scBlue.CreateWnd(scWnd, hinst, "Niebieski (blue)", ID_SC_BLUE, 10, ypos, width - 20,30);
	scBlue.SetValue(double(left.b * 255.f));
	scBlue.SetBkTextColor(RGB(200,200,255));
	ypos += 60;

	gblue.SetRGBA_B(left.r, left.g, left.a);

	scAlpha.CreateWnd(scWnd, hinst, "Alfa (alpha)", ID_SC_ALPHA, 10, ypos, width - 20,30);
	scAlpha.SetValue(double(left.a * 255.f));
	ypos += 60;

	galpha.SetRGBA_A(left.r, left.g, left.b);

	float h, s, v;
	h = s = v = 0;

	RGB2HSV(h, s, v, left.r, left.g, left.b);

	scHue.CreateWnd(scWnd, hinst, "Barwa (hue)", ID_SC_HUE, 10, ypos, width - 20,30);
	scHue.SetMaxValue(360.);
	scHue.SetDevide(360.);
	scHue.SetValue(double(h));
	ypos += 60;

	ghue.SetHSVA_H(s, v, left.a);

	scSaturation.CreateWnd(scWnd, hinst, "Nasycenie (saturation)", ID_SC_SATURATION, 10, ypos, width - 20,30);
	scSaturation.SetMaxValue(100.);
	scSaturation.SetDevide(100.);
	scSaturation.SetValue(double(s * 100.f));
	ypos += 60;

	gsaturation.SetHSVA_S(h, v, left.a);

	scValue.CreateWnd(scWnd, hinst, "Wartoœæ (value)", ID_SC_VALUE, 10, ypos, width - 20,30);
	scValue.SetMaxValue(100.);
	scValue.SetDevide(100.);
	scValue.SetValue(double(v * 100.f));

	gvalue.SetHSVA_V(h, s, left.a);

	ypos += 50;

	scHex = CreateWindow("EDIT",left.ChangeColorToHexStringRGBA().c_str(),WS_CHILD|WS_VISIBLE|WS_BORDER, width / 2 + 20, ypos, width / 2 - 30, 20, scWnd, (HMENU)ID_SC_HEX, hinst, NULL);
	SendMessage(scHex, WM_SETFONT, (WPARAM) textFont, NULL);

	UpdateWindow(scWnd);
	ShowWindow(scWnd, SW_NORMAL);
}

void Document::scWindows::Destroy(){
	DestroyWindow(scWnd);
}

void Document::scWindows::SetControlsValue(float r, float g, float b, float a){
	scRed.SetValue(double(r * 255.f));

	scGreen.SetValue(double(g * 255.f));

	scBlue.SetValue(double(b * 255.f));

	scAlpha.SetValue(double(a * 255.f));

	float h, s, v;
	h = s = v = 0;

	RGB2HSV(h, s, v, r, g, b);

	scHue.SetValue(double(h));

	scSaturation.SetValue(double(s * 100.f));

	scValue.SetValue(double(v * 100.f));
}

#pragma endregion