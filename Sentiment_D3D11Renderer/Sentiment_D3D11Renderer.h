#ifndef SENTIMENT_D3D11RENDERER_H
#define SENTIMENT_D3D11RENDERER_H

#include <map>

// direct 3d includes
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <D3DX11.h>
#include <d3dx10math.h>
#include "..\Root\Engine\Graphics\IRenderer.h"
//#include "..\Root\Utility\Factory\Factory.h"


class D3D11RenderUtility :
    public IRenderUtility
{

public:

    class D3D11HardwareBuffer :
        public IRenderUtility::IHardwareBuffer
    {
    public:
        D3D11HardwareBuffer(INIT_DESC& desc) :
            IHardwareBuffer(desc)
            {}

    };

    class D3D11Shader :
        public IRenderUtility::IShader
    {
        // STUB
    };

    class D3D11RenderTarget :
        public IRenderUtility::IRenderTarget
    {
        // STUB
    };

    class InputLayout:
        public IRenderUtility::IInputLayout
    {
        // STUB
    };

    class DepthBuffer :
        public IRenderUtility::IDepthBuffer
    {
        // STUB
    };

    class VertexBuffer :
        public D3D11HardwareBuffer
    {
    };

    class IndexBuffer :
        public D3D11HardwareBuffers
    {
    };

    class ConstantBuffer :
        public D3D11HardwareBuffers
    {
    };

    class Texture1D :
        public D3D11

public:
    // ***** COMPONENT CONSTRUCTORS ***** //
    std::shared_ptr<IHardwareBuffer> CreateHardwareBuffer(const std::string& typeName, IHardwareBuffer::INIT_DESC& desc) const;

    std::shared_ptr<IShader> CreateShader(const std::string& typeName, IShader::INIT_DESC& desc) const;

    std::shared_ptr<IShaderResource> CreateShaderResource(const std::string& typeName, IShaderResource::INIT_DESC& desc) const;

    std::shared_ptr<IRenderTarget> CreateRenderTarget(const std::string& typeName, IRenderTarget::INIT_DESC& desc) const;

    // ***** CONTAINER CONSTRUCTORS ***** //
    std::shared_ptr<IResourceGroup> CreateResourceGroup(IResourceGroup::INIT_DESC& desc) const;

    std::shared_ptr<IShaderProgram> CreateShaderProgram(IShaderProgram::INIT_DESC& desc) const;

    std::shared_ptr<IFrameBuffer> CreateFrameBuffer(IFrameBuffer::INIT_DESC& desc) const;
};

class D3D11Renderer :
    public IRenderer
{

    void Begin(float red, float green, float blue, float alpha);

    void * Map(std::shared_ptr<IRenderUtility::IMappable>& buffer);

    void Unmap(std::shared_ptr<IRenderUtility::IMappable>& buffer);

    void SetResourceGrouop(std::shared_ptr<IRenderUtility::IResourceGroup>& group);

    void SetShaderProgram(std::shared_ptr<IRenderUtility::IShader>& shaderProg);

    void SetFrameBuffer(std::shared_ptr<IRenderUtility::IFrameBuffer>& buffer);

    void Draw(unsigned int vertCount, unsigned int offset);

    void DrawIndexed(unsigned int indexCount, unsigned int offset);

    void End();
};

#endif
