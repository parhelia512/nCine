#ifndef CLASS_NCINE_MATERIAL
#define CLASS_NCINE_MATERIAL

#include "GLShaderUniforms.h"
#include "GLShaderUniformBlocks.h"
#include "GLShaderAttributes.h"

namespace ncine {

class GLShaderProgram;
class GLTexture;
class Texture;
class GLUniformCache;
class GLAttribute;

/// The class containing material data for a drawable node
class Material
{
  public:
	/// One of the predefined shader programs
	enum class ShaderProgramType
	{
		/// Shader program for Sprite classes
		SPRITE = 0,
		/// Shader program for MeshSprite classes
		MESH_SPRITE,
		/// Shader program for TextNode classes with grayscale font texture
		TEXTNODE_GRAY,
		/// Shader program for TextNode classes with color font texture
		TEXTNODE_COLOR,
		/// Shader program for ProfilePlotter
		COLOR,
		/// Shader program for a batch of Sprite classes
		BATCHED_SPRITES,
		/// Shader program for a batch of MeshSprite classes
		BATCHED_MESH_SPRITES,
		/// A custom shader program
		CUSTOM
	};

	/// Default constructor
	Material();
	Material(GLShaderProgram *program, GLTexture *texture);

	inline bool isTransparent() const { return isTransparent_; }
	inline void setTransparent(bool isTransparent) { isTransparent_ = isTransparent; }

	inline ShaderProgramType shaderProgramType() const { return shaderProgramType_; }
	inline const GLShaderProgram *shaderProgram() const { return shaderProgram_; }
	void setShaderProgramType(ShaderProgramType shaderProgramType);
	void setUniformsDataPointer(GLubyte *dataPointer);
	/// Wrapper around `GLShaderUniforms::uniform()`
	inline GLUniformCache *uniform(const char *name) { return shaderUniforms_.uniform(name); }
	/// Wrapper around `GLShaderUniformBlocks::uniformBlock()`
	inline GLUniformBlockCache *uniformBlock(const char *name) { return shaderUniformBlocks_.uniformBlock(name); }
	/// Wrapper around `GLShaderAttributes::attribute()`
	inline GLVertexFormat::Attribute *attribute(const char *name) { return shaderAttributes_.attribute(name); }
	inline const GLTexture *texture() const { return texture_; }
	inline void setTexture(const GLTexture *texture) { texture_ = texture; }
	void setTexture(const Texture &texture);

  private:
	bool isTransparent_;

	ShaderProgramType shaderProgramType_;
	GLShaderProgram *shaderProgram_;
	GLShaderUniforms shaderUniforms_;
	GLShaderUniformBlocks shaderUniformBlocks_;
	GLShaderAttributes shaderAttributes_;
	const GLTexture *texture_;

	/// Memory buffer with uniform values to be sent to the GPU
	nctl::UniquePtr<GLubyte []> uniformsHostBuffer_;

	void bind();
	void setShaderProgram(GLShaderProgram *program);
	/// Wrapper around `GLShaderUniforms::commitUniforms()`
	inline void commitUniforms() { shaderUniforms_.commitUniforms(); }
	/// Wrapper around `GLShaderUniformBlocks::commitUniformBlocks()`
	inline void commitUniformBlocks() { shaderUniformBlocks_.commitUniformBlocks(); }
	/// Wrapper around `GLShaderAttributes::defineVertexPointers()`
	inline void defineVertexFormat(const GLBufferObject *vbo) { if (vbo) shaderAttributes_.defineVertexFormat(vbo); }
	unsigned int sortKey();

	friend class RenderCommand;
};

}

#endif
