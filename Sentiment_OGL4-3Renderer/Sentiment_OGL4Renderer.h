#ifndef SENTIMENT_OGL4RENDERER_H
#define SENTIMENT_OGL4RENDERER_H

#include ".\Root\Engine\Graphics\IRenderer.h"
#include <fstream>

#ifdef BUILD_DLL
#define LIBLINK extern "C" __declspec(dllexport)
#else
#define LIBLINK extern "C" __declspec(dllimport)
#endif

#ifdef _WIN32
#include <windows.h>
#include "glew.h"
#include "wglew.h"
#include "gl\gl.h"
#endif

class OGL4RenderUtility :
    public IRenderUtility
{
public:

    class OGL4HardwareBuffer :
        public IHardwareBuffer
    {
    public:
        IRenderUtility::USAGE GetUsageType() const;

    };

    class OGL4ShaderResource :
        public IShaderResource
    {
        // stub
    };

    class OGL4Shader :
        public IShader
    {
    public:
        OGL4Shader(int shaderID) :
            m_ShaderID(shaderID)
            {}

        ~OGL4Shader();

        void cache() {}

        void uncache() {}

        int m_ShaderID;
    };

    class OGL4RenderTarget :
        public IRenderTarget
    {
        // stub
    };

    class OGL4InputLayout :
        public IInputLayout
    {
        // stub
    };

    class OGL4DepthBuffer :
        public IDepthBuffer
    {

    };

    class OGL4RenderContext :
        public IRenderUtility::IRenderContext
    {
    public:
#ifdef _WIN32
        OGL4RenderContext(HWND windowHandle, HDC dcontext, HGLRC rcontext) :
            m_WindowHandle(windowHandle),
            m_DeviceContext(dcontext),
            m_RenderContext(rcontext)
            {}

        ~OGL4RenderContext()
        {
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(m_RenderContext);
            m_RenderContext = NULL;

            ReleaseDC(m_WindowHandle, m_DeviceContext);
            m_DeviceContext = 0;
        }

        HWND m_WindowHandle;
        HDC m_DeviceContext;
        HGLRC m_RenderContext;
#elif _UNIX
#endif
    };

    class OGL4VertexBuffer :
        public OGL4HardwareBuffer
    {
    public:
        OGL4VertexBuffer(unsigned int vertexArrayID, unsigned int vertexBufferID) :
            m_VertexArrayID(vertexArrayID),
            m_VertexBufferID(vertexBufferID)
            {}

        unsigned int m_VertexArrayID , m_VertexBufferID;
    };

    class OGL4IndexBuffer :
        public OGL4HardwareBuffer
    {
    public:
        OGL4IndexBuffer(unsigned int indexBufferID) :
            m_IndexBufferID(indexBufferID)
            {}

        unsigned int m_IndexBufferID;
    };

public:
    class OGL4ResourceGroup :
        public IResourceGroup
    {

    public:
        void AttachVertexBuffer(const std::shared_ptr<IHardwareBuffer> & buf, unsigned int slot, const std::string& semantic);

        void AttachIndexBuffer(const std::shared_ptr<IHardwareBuffer>& buf, unsigned int slot, const std::string& semantic);

        // attaches an input layout for use with the shader
        void AttachInputLayout(const std::shared_ptr<IInputLayout> & buf);

        void AttachShaderResource(const std::string& shader, std::shared_ptr<IShaderResource>& rec);

        // detaches a vertex buffer from the resource group if it exists
        void DetachVertexBuffer(const std::shared_ptr<IHardwareBuffer> & vertBuf);

        // detaches an index buffer from the resource group if it exists
        void DetachIndexBuffer();

        void DetachInputLayout();

        void DetachShaderResource(const std::string& shader, std::shared_ptr<IShaderResource>& rec);

    };

    class OGL4ShaderProgram :
        public IShaderProgram
    {
    public:
        OGL4ShaderProgram(int programID) :
            m_ProgramID(programID)
            {}

        int m_ProgramID;
    };

    class OGL4FrameBuffer :
        public IShaderProgram
    {
        // attaches a render target at the index specified. must be 1 - 7. If there is a
        // target already in place, the input replaces it.
        void AttachRenderTarget(const std::shared_ptr<IRenderTarget> & buffer);

        // removes old depth buffer and attaches the one provided
        void AttachDepthBuffer(const std::shared_ptr<IDepthBuffer> & depthBuffer);

        // detaches render target at provided pointer
        void DetachRenderTarget(const std::shared_ptr<IRenderTarget> & targ);

        // detaches render target at provided index
        void DetachRenderTarget(unsigned int i);

        // detaches the depth buffer if one is attached
        void DetachDepthBuffer();
    };


public:
    std::shared_ptr<IHardwareBuffer> CreateHardwareBuffer(const std::string& typeName, IHardwareBuffer::INIT_DESC& desc) const;

    std::shared_ptr<IShader> CreateShader(const std::string& typeName, IShader::INIT_DESC& desc) const;

    std::shared_ptr<IShaderResource> CreateShaderResource(const std::string& typeName, IShaderResource::INIT_DESC& desc) const;

    std::shared_ptr<IRenderTarget> CreateRenderTarget(const std::string& typeName, IRenderTarget::INIT_DESC& desc) const;

    std::shared_ptr<IRenderContext> CreateRenderContext(IRenderContext::INIT_DESC& desc) const;

    std::shared_ptr<IResourceGroup> CreateResourceGroup(IResourceGroup::INIT_DESC& desc) const;

    std::shared_ptr<IShaderProgram> CreateShaderProgram(IShaderProgram::INIT_DESC& desc) const;

    std::shared_ptr<IFrameBuffer> CreateFrameBuffer(IFrameBuffer::INIT_DESC& desc) const;

private:
    static std::shared_ptr<const char> LoadShaderSourceFile(const std::string& name);

    static void OutputShaderErrorMessage(unsigned int shaderId, const std::string& filename);
};

class OGL4Renderer :
    public IRenderer
{
public:
    void Begin(float red, float green, float blue, float alpha);

    void* Map(std::shared_ptr<IRenderUtility::IMappable>& buffer, IRenderUtility::MAP_TYPE);

    void Unmap(std::shared_ptr<IRenderUtility::IMappable>& buffer);

    void SetRenderContext(std::shared_ptr<IRenderUtility::IRenderContext>& context);

    void SetResoureGroup(std::shared_ptr<IRenderUtility::IResourceGroup>& resources);

    void SetShaderProgram(std::shared_ptr<IRenderUtility::IShaderProgram>& prog);

    void SetFrameBuffer(std::shared_ptr<IRenderUtility::IFrameBuffer>& frameBuf);

    void Draw(unsigned int vertCount, unsigned int offset);

    void DrawIndexed(unsigned int indexCount, unsigned int offset);

    void End();

private:
    std::weak_ptr<OGL4RenderUtility::OGL4RenderContext> m_CurrentContext;
};


LIBLINK void CreateRendererAndUtility(std::shared_ptr<IRenderer> & renderObj, std::shared_ptr<IRenderUtility> & utilityObj);


#endif
