#include "RenderBuffersManager.h"
#include "RenderStatistics.h"
#include "GLDebug.h"

namespace ncine {

///////////////////////////////////////////////////////////
// CONSTRUCTORS and DESTRUCTOR
///////////////////////////////////////////////////////////

RenderBuffersManager::RenderBuffersManager(unsigned long vboMaxSize)
	: buffers_(4)
{
	BufferSpecifications &vboSpecs = specs_[BufferTypes::ARRAY];
	vboSpecs.type = BufferTypes::ARRAY;
	vboSpecs.target = GL_ARRAY_BUFFER;
	vboSpecs.mapFlags = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_UNSYNCHRONIZED_BIT;
	vboSpecs.usageFlags = GL_STREAM_DRAW;
	vboSpecs.maxSize = vboMaxSize;
	vboSpecs.alignment = sizeof(GLfloat);

	const IGfxCapabilities &gfxCaps = theServiceLocator().gfxCapabilities();
	const int maxUniformBlockSize = gfxCaps.value(IGfxCapabilities::GLIntValues::MAX_UNIFORM_BLOCK_SIZE);
	const int offsetAlignment = gfxCaps.value(IGfxCapabilities::GLIntValues::UNIFORM_BUFFER_OFFSET_ALIGNMENT);

	BufferSpecifications &uboSpecs = specs_[BufferTypes::UNIFORM];
	uboSpecs.type = BufferTypes::UNIFORM;
	uboSpecs.target = GL_UNIFORM_BUFFER;
	uboSpecs.mapFlags = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_UNSYNCHRONIZED_BIT;
	uboSpecs.usageFlags = GL_STREAM_DRAW;
	uboSpecs.maxSize = static_cast<unsigned int>(maxUniformBlockSize);
	uboSpecs.alignment = static_cast<unsigned int>(offsetAlignment);

	// Create the first buffer for each type right away
	for (unsigned int i = 0; i < BufferTypes::COUNT; i++)
		createBuffer(specs_[i]);
}

///////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////

const RenderBuffersManager::Parameters RenderBuffersManager::acquireMemory(BufferTypes::Enum type, unsigned long bytes, unsigned int alignment)
{
	FATAL_ASSERT(bytes <= specs_[type].maxSize);

	// Accepting a custom alignment only if it is a multiple of the specification one
	if (alignment % specs_[type].alignment != 0)
		alignment = specs_[type].alignment;

	Parameters params;

	for (ManagedBuffer &buffer : buffers_)
	{
		if (buffer.type == type)
		{
			const unsigned long offset = buffer.size - buffer.freeSpace;
			const unsigned int alignAmount = (alignment - offset % alignment) % alignment;

			if (buffer.freeSpace >= bytes + alignAmount)
			{
				params.object = buffer.object.get();
				params.offset = offset + alignAmount;
				params.size = bytes;
				buffer.freeSpace -= bytes + alignAmount;
				params.mapBase = buffer.mapBase;
				break;
			}
		}
	}

	if (params.object == nullptr)
	{
		createBuffer(specs_[type]);
		params.object = buffers_.back().object.get();
		params.offset = 0;
		params.size = bytes;
		buffers_.back().freeSpace -= bytes;
		params.mapBase = buffers_.back().mapBase;
	}

	return params;
}

///////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
///////////////////////////////////////////////////////////

void RenderBuffersManager::flushUnmap()
{
	GLDebug::ScopedGroup scoped("RenderBuffersManager::flushUnmap()");

	for (ManagedBuffer &buffer : buffers_)
	{
		RenderStatistics::gatherStatistics(buffer);
		const unsigned long usedSize = buffer.size - buffer.freeSpace;
		FATAL_ASSERT(usedSize <= specs_[buffer.type].maxSize);
		buffer.freeSpace = buffer.size;

		if (specs_[buffer.type].mapFlags == 0)
		{
			if (usedSize > 0)
			{
				buffer.object->bufferSubData(0, usedSize, buffer.hostBuffer.get());
				buffer.object->bufferData(buffer.size, nullptr, specs_[buffer.type].usageFlags);
			}
		}
		else
		{
			if (usedSize > 0)
				buffer.object->flushMappedBufferRange(0, usedSize);
			buffer.object->unmap();
		}

		buffer.mapBase = nullptr;
	}
}

void RenderBuffersManager::remap()
{
	GLDebug::ScopedGroup scoped("RenderBuffersManager::remap()");

	for (ManagedBuffer &buffer : buffers_)
	{
		ASSERT(buffer.freeSpace == buffer.size);
		ASSERT(buffer.mapBase == nullptr);

		if (specs_[buffer.type].mapFlags == 0)
			buffer.mapBase = buffer.hostBuffer.get();
		else
			buffer.mapBase = static_cast<GLubyte *>(buffer.object->mapBufferRange(0, buffer.size, specs_[buffer.type].mapFlags));
	}
}

void RenderBuffersManager::createBuffer(const BufferSpecifications &specs)
{
	GLDebug::ScopedGroup scoped("RenderBuffersManager::createBuffer()");

	ManagedBuffer managedBuffer;
	managedBuffer.type = specs.type;
	managedBuffer.size = specs.maxSize;
	managedBuffer.object = nctl::makeUnique<GLBufferObject>(specs.target);
	managedBuffer.object->bufferData(managedBuffer.size, nullptr, specs.usageFlags);
	managedBuffer.freeSpace = managedBuffer.size;

	if (specs.mapFlags == 0)
	{
		managedBuffer.hostBuffer = nctl::makeUnique<GLubyte []>(specs.maxSize);
		managedBuffer.mapBase = managedBuffer.hostBuffer.get();
	}
	else
		managedBuffer.mapBase = static_cast<GLubyte *>(managedBuffer.object->mapBufferRange(0, managedBuffer.size, specs.mapFlags));

	buffers_.pushBack(nctl::move(managedBuffer));
}

}
