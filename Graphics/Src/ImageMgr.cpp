#include "ImageMgr.h"


Sapphire::ImageMgr::ImageMgr()
{

}

Sapphire::ImageMgr::~ImageMgr()
{
	ImageMap::iterator i, begin = m_imageMap.begin(), end = m_imageMap.end();
	for (i = begin; i != end; ++i)
	{
		m_Images.Dereference(i->second)->Unload();
	}
}

Sapphire::HIMAGE Sapphire::ImageMgr::LoadImage(const char* name)
{
	/*std::map<std::string, HIMAGE> _imgMap;
	HIMAGE h;
	ImageMapInsertRc _rc = _imgMap.insert(std::make_pair(name, h));*/

	ImageMapInsertRc rc = m_imageMap.insert(std::make_pair(name, HIMAGE()));
	if (rc.second)
	{

		Image* pImg = m_Images.Acquire(rc.first->second);
		if (!pImg->Load(rc.first->first))
		{
			DeleteTexture(rc.first->second);
			rc.first->second = HIMAGE();
		}
	}
	return (rc.first->second);
}

Sapphire::HIMAGE Sapphire::ImageMgr::GetImage(const char* name)
{
	ImageMap::iterator it = m_imageMap.find(name);
	if (it != m_imageMap.end())
	{
		return it->second;
	}
	return LoadImage(name);
}

void Sapphire::ImageMgr::DeleteTexture(HIMAGE himg)
{
	Image* pImg = m_Images.Dereference(himg);
	if (pImg != 0)
	{
		std::map<std::string, HIMAGE, istring_less >::iterator it = m_imageMap.find(pImg->getName());
		if (it != m_imageMap.end())
		{
			// 通过索引从Map中删除
			m_imageMap.erase(it);
		}
		// 从数据库中删除
		pImg->Unload();
		m_Images.Release(himg);
	}
}

Sapphire::ImageType Sapphire::ImageMgr::GetImageType(HIMAGE himg) const
{
	return m_Images.Dereference(himg)->getImageType();
}
