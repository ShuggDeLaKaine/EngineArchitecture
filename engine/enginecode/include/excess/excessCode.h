/** \file excessCode.h*/
#pragma once
/*
\brief Just an area to contain all the excess code, mainly stuff that has been taken out as I abstract each part.
*/


/*
//need a view, a projection (for camera) and a model matrix.
//two mat4s for the camera.
glm::mat4 view = glm::lookAt(
	glm::vec3(0.0f, 0.0f, 0.0f),	//eye: aka the position; 0.0f, 0.0f, 0.0f, is origin.
	glm::vec3(0.0f, 0.0f, -1.0f),	//centre: aka which way we're looking, convention is to look down the Z axis
	glm::vec3(0.0f, 1.0f, 0.0f)		//up: make up, up (if that makes sense...)
);									//matrix for position and orientation.
glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);	//matrix for how the camera views the world orthographic or perspective. first param field of view, so the camera ratio.
*/

/*
	uint32_t blockNum = 0;								//which block are we using.
	//generate, bind and set UBO for camera.
	UniformBufferLayout cameraLayout = { {"u_projection", ShaderDataType::Mat4 }, {"u_view", ShaderDataType::Mat4 } };
	std::shared_ptr<UniformBuffer> cameraUBO;
	cameraUBO.reset(UniformBuffer::create(cameraLayout));
	*/
	/*
	//now attach to TPShader.
	cameraUBO->attachShaderBlock(TPShader, "b_camera");
	//now send camera data to uniform buffer object.
	cameraUBO->uploadDataToBlock("u_projection", glm::value_ptr(cam3D.m_camera.projection));
	cameraUBO->uploadDataToBlock("u_view", glm::value_ptr(cam3D.m_camera.view));
	*/

	//LIGHTING UBO
	/*
	blockNum++;											//move block number along one.
	glm::vec3 lightPosition(1.0f, 4.0f, 6.0f);			//vec3 for light position.
	glm::vec3 viewPosition(0.0f, 0.0f, 0.0f);			//vec3 for view position.
	glm::vec3 lightColour(1.0f, 1.0f, 1.0f);			//vec3 for light colour.
	uint32_t lightsUBO_ID;								//openGL ID for lights UBO.
	uint32_t lightsDataSize = sizeof(glm::vec4) * 3;	//how big this is; BUT can't use vec3! OpenGL needs either a 2N or 4N, so must use a vec4 here as a base alignment for a vec3 or WILL NOT WORK.
	*/
	/*
	//generate, bind and set UBO for lights.
	glGenBuffers(1, &lightsUBO_ID);														//generate Buffer for light UBO.
	glBindBuffer(GL_UNIFORM_BUFFER, lightsUBO_ID);										//bind buffer for light UBO.
	glBufferData(GL_UNIFORM_BUFFER, lightsDataSize, nullptr, GL_DYNAMIC_DRAW);			//send data and size.
	glBindBufferRange(GL_UNIFORM_BUFFER, blockNum, lightsUBO_ID, 0, lightsDataSize);	//bind the range; to UNI_BUFFER, this block, this ubo, from 0 to data siz (ie all of it).

	//now attach to shaders, as lights just for the TPShader.
	uint32_t blockIndex = glGetUniformBlockIndex(TPShader->getID(), "b_lights");	//first get the block number off the shader.
	glUniformBlockBinding(TPShader->getID(), blockIndex, blockNum);				//link to binding point.
	*/
	/*
	//now send light data to uniform buffer object. These MUST be in the same order as in the shader file.
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(lightPosition));					//uploading light position between 0 and sizeof vec3.
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(viewPosition));		//uploading view position starting at a vec4 (remember vec3 must have base alignment of 4N), then the size of a vec3.
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * 2, sizeof(glm::vec3), glm::value_ptr(lightColour));	//uploading light colour, same as above viewPosition but offsect vec4 * 2, as two vec4s prior.
	*/

/*
cameraUBO->uploadDataToBlock("u_projection", glm::value_ptr(projection));
cameraUBO->uploadDataToBlock("u_view", glm::value_ptr(view));
*/

/*
swu3D["u_view"] = std::pair<ShaderDataType, void *>(ShaderDataType::Mat4, static_cast<void *>(glm::value_ptr(view)));
swu3D["u_projection"] = std::pair<ShaderDataType, void *>(ShaderDataType::Mat4, static_cast<void *>(glm::value_ptr(projection)));
*/

/*
//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
RendererCommons::actionCommand(clearCommand);
*/

/*examples of how you can check the the byte aligned of raw data.
//use uint32_t's though.
int alignVec3 = alignof(glm::vec3);
int alignFCVertex = alignof(FCVertex);
int sizeofVec3 = sizeof(glm::vec3);
int sizeofFCVertex = sizeof(FCVertex);
*/