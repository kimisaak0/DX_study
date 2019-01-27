#include "DxWrite.h"

namespace Lypi
{
	DxWrite& DxWrite::GetInstance()
	{
		static DxWrite inst;
		return inst;
	}

	DxWrite::DxWrite()
	{
		m_pD2dFactory = nullptr;
		m_pD2dRT = nullptr;
		m_pD2dSCB = nullptr;

		m_pDWriteFactory = nullptr;
		m_pTextFormat = nullptr;

		m_fDpiScaleX = 0;
		m_fDpiScaleY = 0;
		m_FDpiX = 0;
		m_FDpiY = 0;

		//흰색을 기본값으로 함.
		m_D2dTextColor = ColorF(1.0, 1.0, 1.0, 1.0);
	}

	//객체 생성
	HRESULT DxWrite::CreateDeviceIR()
	{
		HRESULT hr;
		//D2D 팩토리 생성
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2dFactory);

		//사실 어차피 96이고 나누면 1이다(..)
		m_pD2dFactory->GetDesktopDpi(&m_FDpiX, &m_FDpiY);
		m_fDpiScaleX = m_FDpiX / (FLOAT)96.0;
		m_fDpiScaleY = m_FDpiY / (FLOAT)96.0;

		//DW 팩토리 생성
		if (SUCCEEDED(hr)) {
			hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&m_pDWriteFactory);
		}

		//텍스트 포맷 생성
		if (SUCCEEDED(hr)) {
			hr = m_pDWriteFactory->CreateTextFormat(
				_T("고딕"),
				NULL,
				DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_ULTRA_EXPANDED,
				15.0f,
				_T("ko-kr"),
				&m_pTextFormat
			);
		}

		return hr;
	}

	HRESULT DxWrite::CreateDeviceR(IDXGISurface1* pSurface)
	{
		HRESULT hr = S_OK;
		//렌더링 옵션을 설정하는 구조체.
		D2D1_RENDER_TARGET_PROPERTIES props;
		props.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
		props.pixelFormat = PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_IGNORE);
		props.dpiX = m_FDpiX;
		props.dpiY = m_FDpiY;
		props.usage = D2D1_RENDER_TARGET_USAGE_NONE;
		props.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

		//D2D렌더타겟 생성
		hr = m_pD2dFactory->CreateDxgiSurfaceRenderTarget(pSurface, &props, &m_pD2dRT);
		if (FAILED(hr)) {
			return hr;
		}

		//정해진 색 사용시 ColorF(ColorF::Coral)
		V_FRETURN(m_pD2dRT->CreateSolidColorBrush(m_D2dTextColor, &m_pD2dSCB));

		return hr;
	}

	//객체 삭제
	void DxWrite::DiscardDeviceIR()
	{
		SAFE_RELEASE(m_pD2dFactory);
		SAFE_RELEASE(m_pDWriteFactory);
		SAFE_RELEASE(m_pTextFormat);
		SAFE_RELEASE(m_pTextLayout);
	}

	void DxWrite::DiscardDeviceR()
	{
		SAFE_RELEASE(m_pD2dRT);
		SAFE_RELEASE(m_pD2dSCB);
	}

	//초기화
	bool DxWrite::Init()
	{
		HRESULT hr;
		//독립적인 객체 생성
		V(CreateDeviceIR());
		return true;
	}

	bool DxWrite::Set(IDXGISurface1* m_pSurface)
	{
		HRESULT hr;
		//종속적인 객체 생성 (화면 크기 변경시 재생성함)
		D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2dFactory);
		V(CreateDeviceR(m_pSurface));
		return true;
	}

	bool DxWrite::Set(HWND hWnd, int iWidth, int iHeight, IDXGISurface1* pSurface)
	{
		if (!Init()) {
			return false;
		}

		CreateDeviceIR();
		CreateDeviceR(pSurface);

		wchar_t wst[] = L"TbasisSample";
		SetText(wst, ColorF(1.0, 1.0, 1.0, 1.0));

		return true;
	}

	//포맷 변경
	void DxWrite::SetAlignment(DWRITE_TEXT_ALIGNMENT textAlignment, DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment)
	{
		//TextFormat 지정
		m_pTextFormat->SetTextAlignment(textAlignment);
		m_pTextFormat->SetParagraphAlignment(paragraphAlignment);
	}

	void DxWrite::SetlayoutRt(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom)
	{
		m_D2rtLayoutRect = RectF(left, top, right, bottom);
	}

	void DxWrite::SetTextPos(const D2D1_MATRIX_3X2_F& transform)
	{
		m_pD2dRT->SetTransform(transform);
	}

	ColorF DxWrite::SetTextColor(ColorF color)
	{
		ColorF oldColor(m_D2dTextColor.r, m_D2dTextColor.g, m_D2dTextColor.b, m_D2dTextColor.a);
		m_D2dTextColor = color;
		m_pD2dSCB->SetColor(m_D2dTextColor);

		return oldColor;
	}

	HRESULT DxWrite::SetText(wchar_t* text, ColorF Color)
	{
		HRESULT hr = S_OK;

		// Store the text for later.
		m_cTextLength = (UINT32)wcslen(text);
		m_wszText.clear();
		m_wszText = text;

		// Release any existing layout object.
		SAFE_RELEASE(m_pTextLayout);

		// Create a text layout using the text format.
		if (SUCCEEDED(hr))
		{
			hr = m_pDWriteFactory->CreateTextLayout(
				m_wszText.c_str(),           // The string to be laid out and formatted.
				m_cTextLength,               // The length of the string.
				m_pTextFormat,               // The text format to apply to the string (contains font information, etc).
				m_D2rtLayoutRect.left,       // The width of the layout box.
				m_D2rtLayoutRect.top,        // The height of the layout box.
				&m_pTextLayout               // The IDWriteTextLayout interface pointer.
			);
		}

		// Declare a typography pointer.
		IDWriteTypography* pTypography = NULL;
		// Create a typography interface object.
		if (SUCCEEDED(hr))
		{
			hr = m_pDWriteFactory->CreateTypography(&pTypography);
		}

		// Set the stylistic set.
		DWRITE_FONT_FEATURE fontFeature = { DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_7,1 };
		if (SUCCEEDED(hr))
		{
			hr = pTypography->AddFontFeature(fontFeature);
		}

		// Set the typography for the entire string.
		DWRITE_TEXT_RANGE textRange = { 0, m_cTextLength };

		if (SUCCEEDED(hr))
		{
			hr = m_pTextLayout->SetTypography(pTypography, textRange);
		}

		// Set the underline for the entire string.
		if (SUCCEEDED(hr))
		{
			hr = m_pTextLayout->SetUnderline(m_fontUnderline, textRange);
		}
		SAFE_RELEASE(pTypography);
		return hr;
	}

	HRESULT DxWrite::SetFont(wchar_t* fontFamily)
	{
		HRESULT hr;

		DWRITE_TEXT_RANGE textRange = { 0, m_cTextLength };

		hr = m_pTextLayout->SetFontFamilyName(fontFamily, textRange);

		if (SUCCEEDED(hr))
		{
			m_wszFontFamily.clear();
			m_wszFontFamily = fontFamily;
		}
		return hr;
	}

	HRESULT DxWrite::SetFontSize(float size)
	{
		HRESULT hr;
		DWRITE_TEXT_RANGE textRange = { 0, m_cTextLength };

		hr = m_pTextLayout->SetFontSize(size, textRange);

		if (SUCCEEDED(hr))
		{
			// Store the font size for later.
			m_fontSize = size;
		}
		return hr;
	}

	HRESULT DxWrite::SetBold(bool bold)
	{
		HRESULT hr;
		DWRITE_TEXT_RANGE textRange = { 0, m_cTextLength };

		if (bold)
		{
			m_fontWeight = DWRITE_FONT_WEIGHT_BOLD;
		}
		else
		{
			m_fontWeight = DWRITE_FONT_WEIGHT_NORMAL;
		}

		// Set the font weight.
		hr = m_pTextLayout->SetFontWeight(m_fontWeight, textRange);
		return hr;
	}

	HRESULT DxWrite::SetItalic(bool italic)
	{
		HRESULT hr;
		DWRITE_TEXT_RANGE textRange = { 0, m_cTextLength };

		if (italic)
		{
			m_fontStyle = DWRITE_FONT_STYLE_ITALIC;
		}
		else
		{
			m_fontStyle = DWRITE_FONT_STYLE_NORMAL;
		}

		// Set the font style.
		hr = m_pTextLayout->SetFontStyle(m_fontStyle, textRange);
		return hr;
	}

	HRESULT DxWrite::SetUnderline(bool underline)
	{
		HRESULT hr;
		DWRITE_TEXT_RANGE textRange = { 0, m_cTextLength };

		m_fontUnderline = underline;

		// Set the underline.
		hr = m_pTextLayout->SetUnderline(m_fontUnderline, textRange);
		return hr;
	}

	//DrawText
	bool DxWrite::Begin()
	{
		//D2D Draw는 BeginDraw과 EndDraw사이에서 이루어져야한다.

		if (m_pD2dRT) {
			m_pD2dRT->BeginDraw();
			m_pD2dRT->SetTransform(IdentityMatrix());
		}

		return true;
	}

	HRESULT DxWrite::Drawtxt(RECT rc, TCHAR* pText, ColorF color)
	{
		if (m_pD2dRT && m_pD2dSCB) {
			D2D1_RECT_F layoutRect = RectF(
				static_cast<FLOAT> (rc.left)    / m_fDpiScaleX,
				static_cast<FLOAT> (rc.top)     / m_fDpiScaleY,
				static_cast<FLOAT> (rc.right)   / m_fDpiScaleX,
				static_cast<FLOAT> (rc.bottom)  / m_fDpiScaleY);

			m_D2rtLayoutRect = layoutRect;

			m_pD2dSCB->SetColor(color);
			m_pD2dRT->DrawTextW(pText, (UINT)wcslen(pText), m_pTextFormat, m_D2rtLayoutRect, m_pD2dSCB);
		}

		return S_OK;
	}

	HRESULT DxWrite::Drawtxt(TCHAR* pText)
	{
		m_pD2dRT->DrawTextW(pText, (UINT)wcslen(pText), m_pTextFormat, m_D2rtLayoutRect, m_pD2dSCB);

		return S_OK;
	}

	bool DxWrite::End()
	{
		if (m_pD2dRT && FAILED(m_pD2dRT->EndDraw())) {
			return false;
		}

		return true;
	}

	//전체 삭제
	bool DxWrite::Release()
	{
		DiscardDeviceIR();
		DiscardDeviceR();

		return true;
	}



	DxWrite::~DxWrite()
	{
		Release();
	}
}