#include "Sentiment_OGL4Renderer.h"

//------------------------------

OGL4RenderUtility::OGL4Shader::~OGL4Shader()
{
    glDeleteShader(m_ShaderID);
}

//------------------------------

IRenderUtility::USAGE OGL4RenderUtility::OGL4HardwareBuffer::GetUsageType() const
{
    // stub
    return USAGE_STATIC;
}

void OGL4RenderUtility::OGL4ResourceGroup::AttachVertexBuffer(const std::shared_ptr<IHardwareBuffer>& buf, unsigned int slot, const std::string& semantic)
{
    // stub
}

void OGL4RenderUtility::OGL4ResourceGroup::AttachIndexBuffer(const std::shared_ptr<IHardwareBuffer>& buf, unsigned int slot, const std::string& semantic)
{
    // stub
}

void OGL4RenderUtility::OGL4ResourceGroup::AttachInputLayout(const std::shared_ptr<IInputLayout>& buf)
{
    // stub
}

void OGL4RenderUtility::OGL4ResourceGroup::AttachShaderResource(const std::string& shader, std::shared_ptr<IShaderResource>& rec)
{
    // stub
}

void OGL4RenderUtility::OGL4ResourceGroup::DetachVertexBuffer(const std::shared_ptr<IHardwareBuffer> & vertBuf)
{
    // stub
}

void OGL4RenderUtility::OGL4ResourceGroup::DetachIndexBuffer()
{
    // stub
}

void OGL4RenderUtility::OGL4ResourceGroup::DetachInputLayout()
{
    // stub
}

void OGL4RenderUtility::OGL4ResourceGroup::DetachShaderResource(const std::string& shader, std::shared_ptr<IShaderResource>& rec)
{
    // stub
}

// ------------------------------------

void OGL4RenderUtility::OGL4FrameBuffer::AttachRenderTarget(const std::shared_ptr<IRenderTarget> & buffer)
{
    // stub
}

void OGL4RenderUtility::OGL4FrameBuffer::AttachDepthBuffer(const std::shared_ptr<IDepthBuffer> & depthBuffer)
{
    // stub
}

void OGL4RenderUtility::OGL4FrameBuffer::DetachRenderTarget(const std::shared_ptr<IRenderTarget> & targ)
{
    // stub
}

void OGL4RenderUtility::OGL4FrameBuffer::DetachRenderTarget(unsigned int i)
{
    // stub
}

void OGL4RenderUtility::OGL4FrameBuffer::DetachDepthBuffer()
{
    // stub
}

//-------------------------------------------

std::shared_ptr<IRenderUtility::IHardwareBuffer> OGL4RenderUtility::CreateHardwareBuffer(const std::string& typeName, IHardwareBuffer::INIT_DESC& desc) const
{
    std::shared_ptr<IHardwareBuffer> newBuffer(nullptr) ;

    if(typeName == "Vertex")
    {
        unsigned int vertArrayId;
        unsigned int vertBufferId;

        glGenVertexArrays(1, &vertArrayId);
        glBindVertexArray(vertArrayId);
        glGenBuffers(1, &vertBufferId);

        glBindBuffer(GL_ARRAY_BUFFER, vertBufferId);
        glBufferData(GL_ARRAY_BUFFER, desc.NumElements * desc.ElementSize, desc.Data, GL_STATIC_DRAW);

        for(unsigned int i = 0; i < desc.NumAttribs; i++)
            glEnableVertexAttribArray(i);

        for(unsigned int i = 0; i < desc.NumAttribs; i++)
        {
            glBindBuffer(GL_ARRAY_BUFFER, vertBufferId);
            glVertexAttribPointer(i, desc.AttribSize[i], GL_FLOAT, false, desc.ElementSize, (unsigned char*) NULL + (desc.AttribOffset[i] * sizeof(float)));
        }

        newBuffer.reset(new OGL4VertexBuffer(vertArrayId, vertBufferId));
    }
    else if(typeName == "Index")
    {
        unsigned int indexBufferId;

        glGenBuffers(1, & indexBufferId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, desc.NumElements * sizeof(unsigned int), desc.Data, GL_STATIC_DRAW);

        newBuffer.reset(new OGL4IndexBuffer(indexBufferId));
    }

    return newBuffer;
}

std::shared_ptr<IRenderUtility::IShader> OGL4RenderUtility::CreateShader(const std::string& typeName, IShader::INIT_DESC& desc) const
{
    std::shared_ptr<IShader> newShader(nullptr);

    if(typeName == "Vertex")
    {
        int vertShader;
        int status;
        std::shared_ptr<const char> file;

        file = LoadShaderSourceFile(desc.fileName);
        if(file != nullptr)
            return newShader;

        vertShader = glCreateShader(GL_VERTEX_SHADER);

        const char* meh = file.get();

        glShaderSource(vertShader, 1, &meh, NULL);

        glCompileShader(vertShader);

        glGetShaderiv(vertShader, GL_COMPILE_STATUS, &status);
        if(status != 1)
        {
            OutputShaderErrorMessage(vertShader, desc.fileName);
            return newShader;
        }

        newShader.reset(new OGL4Shader(vertShader));
    }
    else if(typeName == "Pixel")
    {
        int pixelShader;
        int status;
        std::shared_ptr<const char> file;

        file = LoadShaderSourceFile(desc.fileName);
        if(file != nullptr)
            return newShader;

        pixelShader = glCreateShader(GL_FRAGMENT_SHADER);

        const char* meh = file.get();

        glGetShaderiv(pixelShader, GL_COMPILE_STATUS, &status);
        if(status != 1)
        {
            OutputShaderErrorMessage(pixelShader, desc.fileName);
            return newShader;
        }

        newShader.reset(new OGL4Shader(pixelShader));
    }

    return newShader;
}

std::shared_ptr<IRenderUtility::IShaderResource> OGL4RenderUtility::CreateShaderResource(const std::string& typeName, IShaderResource::INIT_DESC& desc) const
{
    // stub
    return std::shared_ptr<IShaderResource>(nullptr);
}

std::shared_ptr<IRenderUtility::IRenderTarget> OGL4RenderUtility::CreateRenderTarget(const std::string& typeName, IRenderTarget::INIT_DESC& desc) const
{
    // stub
    return std::shared_ptr<IRenderTarget>(nullptr);
}

std::shared_ptr<IRenderUtility::IRenderContext> OGL4RenderUtility::CreateRenderContext(IRenderContext::INIT_DESC& desc) const
{
    #ifdef _WIN32
    int attributeListInt[19];
    int attributeList[5];
    PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
    unsigned int formatCount;
    int pixelFormat[1];
    HGLRC renderContext;

    HDC deviceContext = GetDC((HWND) desc.window->get_handle());
    if(!deviceContext)
    {
        throw std::runtime_error("failed to get the HDC");
    }

    attributeListInt[0] = WGL_SUPPORT_OPENGL_ARB;
	attributeListInt[1] = TRUE;

	attributeListInt[2] = WGL_DRAW_TO_WINDOW_ARB;
	attributeListInt[3] = TRUE;

	attributeListInt[4] = WGL_ACCELERATION_ARB;
	attributeListInt[5] = WGL_FULL_ACCELERATION_ARB;

	attributeListInt[6] = WGL_COLOR_BITS_ARB;
	attributeListInt[7] = 24;

	attributeListInt[8] = WGL_DEPTH_BITS_ARB;
	attributeListInt[9] = desc.depthBits;

    attributeListInt[10] = WGL_DOUBLE_BUFFER_ARB;
	attributeListInt[11] = TRUE;

	attributeListInt[12] = WGL_SWAP_METHOD_ARB;
	attributeListInt[13] = WGL_SWAP_EXCHANGE_ARB;

	attributeListInt[14] = WGL_PIXEL_TYPE_ARB;
	attributeListInt[15] = WGL_TYPE_RGBA_ARB;

	attributeListInt[16] = WGL_STENCIL_BITS_ARB;
	attributeListInt[17] = desc.stencilBits;

    attributeListInt[18] = 0;

    if(wglChoosePixelFormatARB(deviceContext, attributeListInt, NULL, 1, pixelFormat, &formatCount) != 1)
    {
        std::string error;
        error = "couldn't choose a pixel format: error " + GetLastError();
        error += GetLastError();
        throw std::runtime_error(error);
    }

    if(SetPixelFormat(deviceContext, pixelFormat[0], &pixelFormatDescriptor) != 1)
    {
        std::string error;
        error = "couldn't set a pixel format: error ";
        throw std::runtime_error(error);
    }

    attributeList[0] = WGL_CONTEXT_MAJOR_VERSION_ARB;
	attributeList[1] = 4;
	attributeList[2] = WGL_CONTEXT_MINOR_VERSION_ARB;
	attributeList[3] = 0;

	attributeList[4] = 0;

	renderContext = wglCreateContextAttribsARB(deviceContext, 0, attributeList);
	if(renderContext == NULL)
	    throw std::runtime_error("could not create context attributes");

	if(wglMakeCurrent(deviceContext, renderContext) != 1)
	    throw std::runtime_error("could not make the context current");

    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);

    glFrontFace(GL_CW);

    glEnable(GL_CULL_FACE);

    glCullFace(GL_BACK);

    return std::shared_ptr<OGL4RenderContext>(new OGL4RenderContext((HWND) desc.window->get_handle(), deviceContext, renderContext));
    #endif

}

std::shared_ptr<IRenderUtility::IResourceGroup> OGL4RenderUtility::CreateResourceGroup(IResourceGroup::INIT_DESC& desc) const
{
    // stub
    return std::shared_ptr<IResourceGroup>(nullptr);
}

std::shared_ptr<IRenderUtility::IShaderProgram> OGL4RenderUtility::CreateShaderProgram(IShaderProgram::INIT_DESC& desc) const
{
    int shaderProgram;

    shaderProgram = glCreateProgram();

    for(auto& it : desc.shaders)
    {
        OGL4Shader* ptr = static_cast<OGL4Shader*>(it.get());

        glAttachShader(shaderProgram, ptr->m_ShaderID);
    }

    glLinkProgram(shaderProgram);

    for(auto& it: desc.shaders)
    {
        OGL4Shader* ptr = static_cast<OGL4Shader*>(it.get());

        glDetachShader(shaderProgram, ptr->m_ShaderID);
    }

    return std::shared_ptr<IShaderProgram>(new OGL4ShaderProgram(shaderProgram));
}

std::shared_ptr<IRenderUtility::IFrameBuffer> OGL4RenderUtility::CreateFrameBuffer(IFrameBuffer::INIT_DESC& desc) const
{
    // stub
    return std::shared_ptr<IFrameBuffer>(nullptr);
}

//___________________________________________

std::shared_ptr<const char> OGL4RenderUtility::LoadShaderSourceFile(const std::string& filename)
{
    std::ifstream fin;
	int fileSize;
	char input;
	char* buffer;


	// Open the shader source file.
	fin.open(filename);

	// If it could not open the file then exit.
	if(fin.fail())
	{
		return 0;
	}

	// Initialize the size of the file.
	fileSize = 0;

	// Read the first element of the file.
	fin.get(input);

	// Count the number of elements in the text file.
	while(!fin.eof())
	{
		fileSize++;
		fin.get(input);
	}

	// Close the file for now.
	fin.close();

	// Initialize the buffer to read the shader source file into.
	buffer = new char[fileSize+1];
	if(!buffer)
	{
		return 0;
	}

	// Open the shader source file again.
	fin.open(filename);

	// Read the shader text file into the buffer as a block.
	fin.read(buffer, fileSize);

	// Close the file.
	fin.close();

	// Null terminate the buffer.
	buffer[fileSize] = '\0';

	return std::shared_ptr<const char>(buffer, [](char* p){delete [] p;} );
}

void OGL4RenderUtility::OutputShaderErrorMessage(unsigned int shaderId, const std::string& filename)
{
    int logSize;
    char* infoLog;
    std::ofstream fout;

    // Get the size of the string containing the information log for the failed shader compilation message.
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);

	// Increment the size by one to handle also the null terminator.
	logSize++;

	// Create a char buffer to hold the info log.
	infoLog = new char[logSize];
	if(!infoLog)
	{
		return;
	}

	// Now retrieve the info log.
	glGetShaderInfoLog(shaderId, logSize, NULL, infoLog);

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for(int i=0; i<logSize; i++)
	{
		fout << infoLog[i];
	}

	// Close the file.
	fout.close();

	return;
}

//-------------------------------------------

void OGL4Renderer::Begin(float red, float green, float blue, float alpha)
{
    glClearColor(red, green, blue, alpha);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void* OGL4Renderer::Map(std::shared_ptr<IRenderUtility::IMappable>& buffer, IRenderUtility::MAP_TYPE)
{
    // stub
    return nullptr;
}

void OGL4Renderer::Unmap(std::shared_ptr<IRenderUtility::IMappable>& buffer)
{
    // stub
}

void OGL4Renderer::SetRenderContext(std::shared_ptr<IRenderUtility::IRenderContext>& context)
{
    OGL4RenderUtility::OGL4RenderContext * oglContext = static_cast<OGL4RenderUtility::OGL4RenderContext*>(context.get());

#ifdef _WIN32
    if(wglMakeCurrent(oglContext->m_DeviceContext, oglContext->m_RenderContext) != 1)
        throw std::runtime_error("failed to set the context");

    m_CurrentContext = std::static_pointer_cast<OGL4RenderUtility::OGL4RenderContext>(context);
#elif _UNIX
#endif

}

void OGL4Renderer::SetResoureGroup(std::shared_ptr<IRenderUtility::IResourceGroup>& resources)
{
    OGL4RenderUtility::IResourceGroup* ptr = static_cast<OGL4RenderUtility::OGL4ResourceGroup*>(resources.get());


}

void OGL4Renderer::SetShaderProgram(std::shared_ptr<IRenderUtility::IShaderProgram>& prog)
{
    // stub
}

void OGL4Renderer::SetFrameBuffer(std::shared_ptr<IRenderUtility::IFrameBuffer>& frameBuf)
{
    // stub
}

void OGL4Renderer::Draw(unsigned int vertCount, unsigned int offset)
{
    // stub
}

void OGL4Renderer::DrawIndexed(unsigned int indexCount, unsigned int offset)
{
    // stub
}

void OGL4Renderer::End()
{
#ifdef _WIN32
    std::shared_ptr<OGL4RenderUtility::OGL4RenderContext> context;
    context = m_CurrentContext.lock();

    if(context != nullptr)
    {
        SwapBuffers(context->m_DeviceContext);
    }

#elif _UNIX
#endif
}


void CreateRendererAndUtility(std::shared_ptr<IRenderer> & renderObj, std::shared_ptr<IRenderUtility> & utilityObj)
{
#ifdef _WIN32
    HDC deviceContext;
    HGLRC renderContext;
    PIXELFORMATDESCRIPTOR pixelFormat;

    deviceContext = GetDC(NULL);
    if(!deviceContext)
        throw std::runtime_error("hit at getDC");

    if(SetPixelFormat(deviceContext, 1, &pixelFormat) != 1)
        throw std::runtime_error("hit at pixelformat");

    renderContext = wglCreateContext(deviceContext);
    if(!renderContext)
        throw std::runtime_error("hit at wglCreateContext");

    if(wglMakeCurrent(deviceContext, renderContext) != 1)
        throw std::runtime_error("hit at wglMakeCurrent");
#endif

    if (GLEW_OK != glewInit())
    {
        throw std::runtime_error("hit at glewInit()");
    }

    if(!GLEW_VERSION_4_0)
    {
        throw std::runtime_error("hit at glew version check");
    }

#ifdef _WIN32
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(renderContext);
    renderContext = NULL;

    ReleaseDC(NULL, deviceContext);
    deviceContext = 0;
#endif

    renderObj.reset(new OGL4Renderer);

    utilityObj.reset(new OGL4RenderUtility);
}
