#include "Appearance.h"

Appearance::Appearance(Geometry geometry, Material material) : _geometry(geometry), _material(material)
{
	_textureRV = nullptr;
}

Appearance::~Appearance()
{
	if (_textureRV)
		delete _textureRV;
	_textureRV = nullptr;
}
