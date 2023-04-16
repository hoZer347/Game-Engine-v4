#include "Data.h"

#include "Loom.h"

#include "Shader.h"
#include "Texture.h"

namespace loom
{
	void Shader::load() { id = GetSMgr()->create(files); };
	void Texture::load() { id = GetTMgr()->create(file, type); };
};
