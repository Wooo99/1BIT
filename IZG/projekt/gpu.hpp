/*!
 * @file
 * @brief This file contains class that represents graphic card.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */
#pragma once

#include <student/fwd.hpp>
#include <vector>
#include <unordered_map>
#include<string.h>
#include<memory>
#include<array>
#include<cassert>

struct Color{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

struct Program
{
	FragmentShader PS;
	VertexShader VS;
	Uniforms ConstantBuffers;
	std::array<AttributeType, maxAttributes> VS2PS;
};
struct InputLayoutDesc
{
	BufferID CurBuffer;
	size_t Offset;
	AttributeType Format;
	uint16_t Stride;
	bool Enabled;	
};
enum class Indexing
{
	NONE = 0,
	UINT8 = 1,
	UINT16 = 2,
	UINT32 = 4,
};
struct Vertexpuller
{
	std::array<InputLayoutDesc, 16> descriptors{ 0 };
	BufferID IAActiveIndexBuffer = 0;
	Indexing indexing = Indexing::NONE;
};


class GPU{
  public:
    GPU();
    virtual ~GPU();

    //buffer object commands
    BufferID  createBuffer           (uint64_t size);
    void      deleteBuffer           (BufferID buffer);
    void      setBufferData          (BufferID buffer,uint64_t offset,uint64_t size,void const* data);
    void      getBufferData          (BufferID buffer,uint64_t offset,uint64_t size,void      * data);
    bool      isBuffer               (BufferID buffer);

    //vertex array object commands (vertex puller)
    ObjectID  createVertexPuller     ();
    void      deleteVertexPuller     (VertexPullerID vao);
    void      setVertexPullerHead    (VertexPullerID vao,uint32_t head,AttributeType type,uint64_t stride,uint64_t offset,BufferID buffer);
    void      setVertexPullerIndexing(VertexPullerID vao,IndexType type,BufferID buffer);
    void      enableVertexPullerHead (VertexPullerID vao,uint32_t head);
    void      disableVertexPullerHead(VertexPullerID vao,uint32_t head);
    void      bindVertexPuller       (VertexPullerID vao);
    void      unbindVertexPuller     ();
    bool      isVertexPuller         (VertexPullerID vao);

    //program object commands
    ProgramID createProgram          ();
    void      deleteProgram          (ProgramID prg);
    void      attachShaders          (ProgramID prg,VertexShader vs,FragmentShader fs);
    void      setVS2FSType           (ProgramID prg,uint32_t attrib,AttributeType type);
    void      useProgram             (ProgramID prg);
    bool      isProgram              (ProgramID prg);
    void      programUniform1f       (ProgramID prg,uint32_t uniformId,float     const&d);
    void      programUniform2f       (ProgramID prg,uint32_t uniformId,glm::vec2 const&d);
    void      programUniform3f       (ProgramID prg,uint32_t uniformId,glm::vec3 const&d);
    void      programUniform4f       (ProgramID prg,uint32_t uniformId,glm::vec4 const&d);
    void      programUniformMatrix4f (ProgramID prg,uint32_t uniformId,glm::mat4 const&d);

    //framebuffer functions
    void      createFramebuffer      (uint32_t width,uint32_t height);
    void      deleteFramebuffer      ();
    void      resizeFramebuffer      (uint32_t width,uint32_t height);
    uint8_t*  getFramebufferColor    ();
    float*    getFramebufferDepth    ();
    uint32_t  getFramebufferWidth    ();
    uint32_t  getFramebufferHeight   ();

    //execution commands
    void      clear                  (float r,float g,float b,float a);
    void      drawTriangles          (uint32_t  nofVertices);

 private:
	std::unordered_map<BufferID, std::vector<uint8_t>> buffers;
	std::unordered_map<VertexPullerID, std::unique_ptr<Vertexpuller>> VertexPullers;
	std::unordered_map<ProgramID, std::unique_ptr<Program>> Programs;
	std::vector<uint8_t> FrameBuffer;
	uint32_t FrameBufferWidth;
	uint32_t FrameBufferHeight;
	std::vector<float> Depth;
	Vertexpuller* UsingVertex = nullptr;
	Program* UsingProgram = nullptr;
};

