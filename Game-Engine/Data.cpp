#include "Data.h"

#include "Shader.h"
#include "Texture.h"

namespace loom
{
	void Shader::load() { id = GetSMgr()->create(files); };
	void Texture::load() { id = GetTMgr()->create(file, type); };
};
