#ifndef __FONT__H__
#define  __FONT__H__

#include <Graphics.h>
#include <handle.h>

namespace Sapphire
{
	enum FONT_TYPE
	{
		FONT_NONE = 0,
		FONT_FREETYPE,
		FONT_BITMAP,
		MAX_FONT_TYPES
	};
	

	class Font
	{
	public:
		Font();
		virtual ~Font();

		virtual void Load(std::string fontPath);

		std::string getFontName() const { return m_fontName; }
		void setFontName(std::string val) { m_fontName = val; }

	protected:
		
		std::string m_fontName;
		FONT_TYPE m_type;

	};

	typedef Handle <Font> HFONT;
}

#endif // !__FONT__H__

