/*!
 * @file
 * @brief This file contains implementation of gpu
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <student/gpu.hpp>



/// \addtogroup gpu_init
/// @{

/**
 * @brief Constructor of GPU
 */
GPU::GPU(){
  /// \todo Zde můžete alokovat/inicializovat potřebné proměnné grafické karty
}

/**
 * @brief Destructor of GPU
 */
GPU::~GPU(){
  /// \todo Zde můžete dealokovat/deinicializovat grafickou kartu
}

/// @}

/** \addtogroup buffer_tasks 01. Implementace obslužných funkcí pro buffery
 * @{
 */

/**
 * @brief This function allocates buffer on GPU.
 *
 * @param size size in bytes of new buffer on GPU.
 *
 * @return unique identificator of the buffer
 */
BufferID GPU::createBuffer(uint64_t size) { 
  	auto buffer = std::vector<uint8_t>(size_t(size));
  	BufferID pointer = reinterpret_cast<BufferID>(buffer.data());
  	buffers.emplace(pointer, std::move(buffer));
  	return pointer;
}

/**
 * @brief This function frees allocated buffer on GPU.
 *
 * @param buffer buffer identificator
 */
void GPU::deleteBuffer(BufferID buffer) {
 if(buffers.find(buffer) != buffers.end())
	{
 		if(auto bufferit = buffers.find(buffer); bufferit != buffers.end())
  		{
  			buffers.erase(buffer);
  		}
	}
}

/**
 * @brief This function uploads data to selected buffer on the GPU
 *
 * @param buffer buffer identificator
 * @param offset specifies the offset into the buffer's data
 * @param size specifies the size of buffer that will be uploaded
 * @param data specifies a pointer to new data
 */
void GPU::setBufferData(BufferID buffer, uint64_t offset, uint64_t size, void const* data) {
	if(buffers.find(buffer) != buffers.end()){
		if(auto temp = buffers.find(buffer); temp != buffers.end()){
			std::copy((uint8_t*)data,(uint8_t*)data + size,(*temp).second.begin() + offset);
		}
	}
}

/**
 * @brief This function downloads data from GPU.
 *
 * @param buffer specfies buffer
 * @param offset specifies the offset into the buffer from which data will be returned, measured in bytes. 
 * @param size specifies data size that will be copied
 * @param data specifies a pointer to the location where buffer data is returned. 
 */
void GPU::getBufferData(BufferID buffer,
                        uint64_t offset,
                        uint64_t size,
                        void*    data)
{
if(buffers.find(buffer) != buffers.end())
	{
		if(auto temp = buffers.find(buffer); temp != buffers.end())
		{	
			auto buff = (*temp).second.begin() + offset;
			std::copy(buff,buff + size, (uint8_t*)data);
		}
	}
}

/**
 * @brief This function tests if buffer exists
 *
 * @param buffer selected buffer id
 *
 * @return true if buffer points to existing buffer on the GPU.
 */
bool GPU::isBuffer(BufferID buffer) { 
  if(buffers.find(buffer)==buffers.end()){
	  return false;
  }
  else
	  return true;
  return false; 
}

/// @}

/**
 * \addtogroup vertexpuller_tasks 02. Implementace obslužných funkcí pro vertex puller
 * @{
 */

/**
 * @brief This function creates new vertex puller settings on the GPU,
 *
 * @return unique vertex puller identificator
 */
ObjectID GPU::createVertexPuller     (){
	auto vertex = std::make_unique<Vertexpuller>();
	VertexPullerID ID = reinterpret_cast<VertexPullerID>(vertex.get());
	VertexPullers.emplace(ID, std::move(vertex));
 	return ID;
}
void     GPU::deleteVertexPuller     (VertexPullerID vao)
{
	if(VertexPullers.find(vao) != VertexPullers.end())
	{
		if(auto alive = VertexPullers.find(vao); alive != VertexPullers.end())
		{
			VertexPullers.erase(alive);
		}
	}
}

void     GPU::setVertexPullerHead    (VertexPullerID vao,uint32_t head,AttributeType type,uint64_t stride,uint64_t offset,BufferID buffer)
{
	if( VertexPullers.find(vao) != VertexPullers.end() ){
		if(auto MyVertex = VertexPullers.find(vao); MyVertex != VertexPullers.end()){
			auto &heads = MyVertex->second->descriptors;
			heads.at(head) = { buffer, offset, type, uint16_t(stride), false};
		}
  	}
}

void     GPU::setVertexPullerIndexing(VertexPullerID vao,IndexType type,BufferID buffer){
	if( VertexPullers.find(vao) != VertexPullers.end()){
		if(auto MyVertex = VertexPullers.find(vao); MyVertex != VertexPullers.end()){
			(*MyVertex).second->indexing = Indexing(type);
			(*MyVertex).second->IAActiveIndexBuffer = buffer;
		}
  	}
}

void     GPU::enableVertexPullerHead (VertexPullerID vao,uint32_t head){
	if( VertexPullers.find(vao) != VertexPullers.end()){
		if(auto MyVertex = VertexPullers.find(vao); MyVertex != VertexPullers.end()){
			auto& heads = MyVertex->second->descriptors;
			heads.at(head).Enabled = true;
  		}
	}
}
void     GPU::disableVertexPullerHead(VertexPullerID vao,uint32_t head){
	if(VertexPullers.find(vao) != VertexPullers.end()){
		if(auto MyVertex = VertexPullers.find(vao); MyVertex != VertexPullers.end()){
			auto heads = (*MyVertex).second->descriptors;
			heads.at(head).Enabled = false;
  		}
	}
}
void     GPU::bindVertexPuller       (VertexPullerID vao){
	if(VertexPullers.find(vao) != VertexPullers.end()){
		if(auto MyVertex = VertexPullers.find(vao); MyVertex != VertexPullers.end()){
			UsingVertex = MyVertex->second.get();
 		}	 
	}
}

void     GPU::unbindVertexPuller     (){
	UsingVertex=nullptr;
}

bool     GPU::isVertexPuller         (VertexPullerID vao){
	if( VertexPullers.find(vao) != VertexPullers.end())  return true;
  	else return false;
}

/// @}

/** \addtogroup program_tasks 03. Implementace obslužných funkcí pro shader programy
 * @{
 */

/**
 * @brief This function creates new shader program.
 *
 * @return shader program id
 */
ProgramID        GPU::createProgram         (){
	auto program = std::make_unique<Program>();
	ProgramID ID = reinterpret_cast<ProgramID>(program.get());
	Programs.emplace(ID, std::move(program));
 	return ID;
}
void             GPU::deleteProgram         (ProgramID prg){
	if(Programs.find(prg) != Programs.end()){
		if(auto Program = Programs.find(prg); Program != Programs.end()){
			Programs.erase(Program);
		}
	}
 }
void             GPU::attachShaders         (ProgramID prg,VertexShader vs,FragmentShader fs){
	if(Programs.find(prg) != Programs.end()){
		if(auto Program = Programs.find(prg); Program != Programs.end()){
			Program->second->PS = fs;
			Program->second->VS = vs;
		}
	}
}

void             GPU::setVS2FSType          (ProgramID prg,uint32_t attrib,AttributeType type){
	if(Programs.find(prg) != Programs.end()){
		if(auto Program = Programs.find(prg); Program != Programs.end()){
			Program->second->VS2PS.at(attrib) = type;
		}
	}
}

void             GPU::useProgram            (ProgramID prg){
	if(Programs.find(prg) != Programs.end()){
		if(auto Program = Programs.find(prg); Program != Programs.end()){
			UsingProgram = Program->second.get();
		}
	}
}

bool             GPU::isProgram             (ProgramID prg){
	if(Programs.find(prg) != Programs.end()) return true;
	return false;
}

void             GPU::programUniform1f      (ProgramID prg,uint32_t uniformId,float     const&d){
	if(Programs.find(prg) != Programs.end()){
		if(auto Program = Programs.find(prg); Program != Programs.end()){
			Program->second->ConstantBuffers.uniform[uniformId].v1 = d;
		}
	}
}

void             GPU::programUniform2f      (ProgramID prg,uint32_t uniformId,glm::vec2 const&d){
	if(Programs.find(prg) != Programs.end()){
		if(auto Program = Programs.find(prg); Program != Programs.end()){
			Program->second->ConstantBuffers.uniform[uniformId].v2 = d;
		}
	}
}

void             GPU::programUniform3f      (ProgramID prg,uint32_t uniformId,glm::vec3 const&d){
	if(Programs.find(prg) != Programs.end()){
		if(auto Program = Programs.find(prg); Program != Programs.end()){
			Program->second->ConstantBuffers.uniform[uniformId].v3 = d;
		}
	}
}

void             GPU::programUniform4f      (ProgramID prg,uint32_t uniformId,glm::vec4 const&d){
	if(Programs.find(prg) != Programs.end()){
		if(auto Program = Programs.find(prg); Program != Programs.end()){
			Program->second->ConstantBuffers.uniform[uniformId].v4 = d;
		}
	}
}

void             GPU::programUniformMatrix4f(ProgramID prg,uint32_t uniformId,glm::mat4 const&d){
	if(Programs.find(prg) != Programs.end()){
		if(auto Program = Programs.find(prg); Program != Programs.end()){
			Program->second->ConstantBuffers.uniform[uniformId].m4 = d;
		}
	}
}

/// @}





/** \addtogroup framebuffer_tasks 04. Implementace obslužných funkcí pro framebuffer
 * @{
 */

/**
 * @brief This function creates framebuffer on GPU.
 *
 * @param width width of framebuffer
 * @param height height of framebuffer
 */
void GPU::createFramebuffer      (uint32_t width,uint32_t height){
	size_t sz = (size_t) width * height;
	FrameBufferWidth = width;
	FrameBufferHeight = height;
	Depth.resize(sz);
	FrameBuffer.resize(sz * 4);
}
void GPU::deleteFramebuffer      (){
	FrameBuffer.resize(0);
	Depth.resize(0);
}

void     GPU::resizeFramebuffer(uint32_t width,uint32_t height){
	FrameBufferWidth = width;
	FrameBufferHeight = height;
 	size_t sz = (size_t) width * height;
	FrameBuffer.resize(sz * 4);
	Depth.resize(sz);
}
uint8_t* GPU::getFramebufferColor  (){
	return &FrameBuffer[0];
}

float* GPU::getFramebufferDepth    (){
	return &Depth[0];
}

uint32_t GPU::getFramebufferWidth (){
	return FrameBufferWidth;
}

uint32_t GPU::getFramebufferHeight(){
 	return FrameBufferHeight;
}

/// @}

/** \addtogroup draw_tasks 05. Implementace vykreslovacích funkcí
 * Bližší informace jsou uvedeny na hlavní stránce dokumentace.
 * @{
 */

/**
 * @brief This functino clears framebuffer.
 *
 * @param r red channel
 * @param g green channel
 * @param b blue channel
 * @param a alpha channel
 */
void            GPU::clear                 (float r,float g,float b,float a){
	if( r >= 0 && g >= 0 && b >= 0 && a >= 0){
		size_t sz = (size_t) FrameBufferWidth * FrameBufferHeight;
		for(int i = 0; i < (sz*4); i++){
			FrameBuffer[i] = r;
			i++;
			FrameBuffer[i] = g;
			i++;
			FrameBuffer[i] = b;
			i++;
			FrameBuffer[i] = a ;
		}
		for(int i = 0; i < sz; i++){
			Depth[i] = 2;
		}
	}
}

void            GPU::drawTriangles         (uint32_t  nofVertices){
	if(nofVertices >= 0){
	switch(UsingVertex->indexing){
		case Indexing::UINT8:{
			InVertex Shader;
			uint32_t MyVertexID = 0;
			auto& MyVertex = *UsingVertex;
			auto& MyProgram = *UsingProgram;
			auto& Buffer = buffers.at(MyVertex.IAActiveIndexBuffer);
			std::vector<uint8_t> IndexArray = reinterpret_cast<std::vector<uint8_t>&>(Buffer);
			auto position = IndexArray.begin();
			std::vector<OutVertex> Output;
			while(position != IndexArray.end())
			{
	        		MyVertexID = uint32_t(*position);
				uint8_t count = 0;
	        		while(count < maxAttributes){
					auto &d = MyVertex.descriptors[count];
					if(d.Enabled)//
						switch(d.Format)//
						{
						case AttributeType::FLOAT://
						{
							auto addres = d.CurBuffer;
							size_t size = (size_t) MyVertexID * d.Stride + d.Offset;
							Shader.attributes[count].v1 = (float&)(buffers.at(addres)[size]); break;//

						}
						case AttributeType::VEC2://
						{
							auto addres = d.CurBuffer;
							size_t size = (size_t) MyVertexID * d.Stride + d.Offset;
							Shader.attributes[count].v2 = (glm::vec2&)(buffers.at(addres)[size]); break;//

						}
						case AttributeType::VEC3://
						{	
							auto addres = d.CurBuffer;
							size_t size = (size_t) MyVertexID * d.Stride + d.Offset;
							Shader.attributes[count].v3 = (glm::vec3&)(buffers.at(addres)[size]); break;//

						}
						case AttributeType::VEC4://
						{
							auto addres = d.CurBuffer;
							size_t size = (size_t) MyVertexID * d.Stride + d.Offset;
							Shader.attributes[count].v4 = (glm::vec4&)(buffers.at(addres)[size]); break;//
						}
						default: break; //same
						}
					Shader.gl_VertexID = MyVertexID;
					count++;;
				}
				position = position + 1;
				auto& lol = Output.emplace_back();
				MyProgram.VS(lol, Shader, MyProgram.ConstantBuffers); 
			}	
	 	return;	
	 	}	
		case Indexing::UINT16:{
			uint32_t MyVertexID = 0;
			auto& MyVertex = *UsingVertex;
			auto& MyProgram = *UsingProgram;
			auto& Buffer = buffers.at(MyVertex.IAActiveIndexBuffer);
			std::vector<uint16_t> IndexArray = reinterpret_cast<std::vector<uint16_t>&>(Buffer);
			auto position = IndexArray.begin();
			std::vector<OutVertex> Output;
			InVertex Shader;
			while(position != IndexArray.end())
			{
	        		MyVertexID = uint32_t(*position);
				uint8_t count = 0;
	        		while(count < maxAttributes){
					auto &d = MyVertex.descriptors[count];
					if(d.Enabled)//
						switch(d.Format)//
						{
						case AttributeType::FLOAT://
						{
							auto addres = d.CurBuffer;
							size_t size = (size_t) MyVertexID * d.Stride + d.Offset;
							Shader.attributes[count].v1 = (float&)(buffers.at(addres)[size]); break;//

						}
						case AttributeType::VEC2://
						{
							auto addres = d.CurBuffer;
							size_t size = (size_t) MyVertexID * d.Stride + d.Offset;
							Shader.attributes[count].v2 = (glm::vec2&)(buffers.at(addres)[size]); break;//

						}
						case AttributeType::VEC3://
						{	
							auto addres = d.CurBuffer;
							size_t size = (size_t) MyVertexID * d.Stride + d.Offset;
							Shader.attributes[count].v3 = (glm::vec3&)(buffers.at(addres)[size]); break;//

						}
						case AttributeType::VEC4://
						{
							auto addres = d.CurBuffer;
							size_t size = (size_t) MyVertexID * d.Stride + d.Offset;
							Shader.attributes[count].v4 = (glm::vec4&)(buffers.at(addres)[size]); break;//
						}
						default: break; //same
						}
					Shader.gl_VertexID = MyVertexID;
					count++;
				}
				position = position + 1;
				auto& lol = Output.emplace_back();
				MyProgram.VS(lol, Shader, MyProgram.ConstantBuffers);
			}	
	 	return;	
	 	}	
		case Indexing::UINT32:{
			uint32_t MyVertexID = 0;
			auto& MyVertex = *UsingVertex;
			auto& MyProgram = *UsingProgram;
			auto& Buffer = buffers.at(MyVertex.IAActiveIndexBuffer);
			std::vector<uint32_t> IndexArray = reinterpret_cast<std::vector<uint32_t>&>(Buffer);
			auto position = IndexArray.begin();
			std::vector<OutVertex> Output;
			InVertex Shader;
			while(position != IndexArray.end())
			{
	        		MyVertexID = uint32_t(*position);
				uint8_t  count = 0;
				while( count < maxAttributes){
					auto &d = MyVertex.descriptors[count];
					if(d.Enabled)//
						switch(d.Format)//
						{
						case AttributeType::FLOAT://
						{
							auto addres = d.CurBuffer;
							size_t size = (size_t) MyVertexID * d.Stride + d.Offset;
							Shader.attributes[count].v1 = (float&)(buffers.at(addres)[size]); break;//

						}
						case AttributeType::VEC2://
						{
							auto addres = d.CurBuffer;
							size_t size = (size_t) MyVertexID * d.Stride + d.Offset;
							Shader.attributes[count].v2 = (glm::vec2&)(buffers.at(addres)[size]); break;//

						}
						case AttributeType::VEC3://
						{	
							auto addres = d.CurBuffer;
							size_t size = (size_t) MyVertexID * d.Stride + d.Offset;
							Shader.attributes[count].v3 = (glm::vec3&)(buffers.at(addres)[size]); break;//

						}
						case AttributeType::VEC4://
						{
							auto addres = d.CurBuffer;
							size_t size = (size_t) MyVertexID * d.Stride + d.Offset;
							Shader.attributes[count].v4 = (glm::vec4&)(buffers.at(addres)[size]); break;//
						}
						default: break; //same
						}
					Shader.gl_VertexID = MyVertexID;
					count++;
				}
				position = position + 1;
				auto& lol = Output.emplace_back();
				MyProgram.VS(lol, Shader, MyProgram.ConstantBuffers);
			}	
	 	return;	
	 	}
		default:{
			InVertex Shader;
			auto& MyVertex = *UsingVertex;
			auto& MyProgram = *UsingProgram;
			std::vector<OutVertex> Output { size_t (nofVertices)};
			uint32_t MyVertexID = 0;
			for(auto& Vertex : Output){
	        		Shader.gl_VertexID = MyVertexID;
				uint8_t count = 0;
	        		while(count < maxAttributes){
					auto &d = MyVertex.descriptors[count];
					if(d.Enabled)
						switch(d.Format)//
						{
						case AttributeType::FLOAT://
						{
							auto addres = d.CurBuffer;
							size_t size = (size_t) MyVertexID * d.Stride + d.Offset;
							Shader.attributes[count].v1 = (float&)(buffers.at(addres)[size]); break;//

						}
						case AttributeType::VEC2://
						{
							auto addres = d.CurBuffer;
							size_t size = (size_t) MyVertexID * d.Stride + d.Offset;
							Shader.attributes[count].v2 = (glm::vec2&)(buffers.at(addres)[size]); break;//

						}
						case AttributeType::VEC3://
						{	
							auto addres = d.CurBuffer;
							size_t size = (size_t) MyVertexID * d.Stride + d.Offset;
							Shader.attributes[count].v3 = (glm::vec3&)(buffers.at(addres)[size]); break;//

						}
						case AttributeType::VEC4://
						{
							auto addres = d.CurBuffer;
							size_t size = (size_t) MyVertexID * d.Stride + d.Offset;
							Shader.attributes[count].v4 = (glm::vec4&)(buffers.at(addres)[size]); break;//
						}
						default: break; //same
						}
				count++;
				}

			MyProgram.VS(Vertex, Shader, MyProgram.ConstantBuffers);
			MyVertexID++;
		}	
	 return;	
	 }	
}
}
}
/// @}
