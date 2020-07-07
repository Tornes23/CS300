#include "SkyBox.h"

SkyBox::SkyBox(std::string cubePath) : mShader("./src/Shader/programs/SkyBox.vs", "./src/Shader/programs/SkyBox.fs"), mCubeMap(cubePath), mModel()
{

}

CubeMap SkyBox::GetCubeMap() const
{
	return mCubeMap;
}

Model SkyBox::GetModel() const
{
	return mModel;
}

ShaderProgram SkyBox::GetShader() const
{
	return mShader;
}
