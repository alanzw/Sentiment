// Programmer: Wyatt Kennedy
// Date: 1/12/2013
// File: IRenderer.h
// This is an abstract std::shared_ptr that is used to represent a graphics renderer
// to be implemented by any api

#ifndef IRENDERER_H
#define IRENDERER_H

// Sentiment includes
#include "Root\Utility\Math\SENTIMENT_Math.h"
#include "Root\Engine\GUI\IGui.h"

// stl includes
#include <memory>
#include <vector>
#include <string>
#include <map>

class IRenderUtility
{
public:
//***********************************************************************************
//	 enum declarations
//***********************************************************************************
    // describes intended use of ShaderResource
    enum USAGE
    {
        // set once from cpu, has only gpu read/write access
        USAGE_STATIC = 0,
        // set constantly from cpu, has cpu write and gpu read access
        USAGE_DYNAMIC = 1,
        // has gpu read only access. Truly constant after initialization
        USAGE_IMMUTABLE = 2,
        // has full cpu and gpu access, use with caution / can be SLOW
        USAGE_STAGING = 3
    };

    // used in draw commands to describe how to draw vertex data
    enum PRIMITIVE_TYPE
    {
        // default value, unset
        PRIMITIVE_UNDEFINED = 0,
        // draws as a list of points with no connective polygons
        PRIMITIVE_POINTLIST = 1,
        // draw as in pairs forming lines. ex. V1->V2  V3->V4
        PRIMITIVE_LINELIST = 2,
        // takes list vert and connect to next vert. ex V1->V2->V3
        PRIMITIVE_LINESTRIP = 3,
        // takes verts in triples and forms triangles. ex. tri(v1, v2, v3) tri (v4, v5, v6)
        PRIMITIVE_TRIANGLELIST = 4,
        // takes last to verts and forms a triangle with it. ex. tri(v1, v2, v3) tri(v2, v3, v4)
        PRIMITIVE_TRIANGLESTRIP = 5
    };

    // used in to describe pixel formats for various buffers that hold pixel data
    enum COLOR_FORMAT
    {
      // r unsigned int normalized
      COLOR_R8_UNORM = 0,
      COLOR_R16_UNORM,

      // rg unsigned int normalized
      COLOR_RG8_UNORM,
      COLOR_RG16_UNORM,

      // rgba unsigned int normalized
      COLOR_RGBA8_UNORM,
      COLOR_RGBA16_UNORM,

      // r signed int normalized
      COLOR_R8_SNORM,
      COLOR_R16_SNORM,

      // rg signed int normalized
      COLOR_RG8_SNORM,
      COLOR_RG16_SNORM,

      // rgba signed int normalized
      COLOR_RGBA8_SNORM,
      COLOR_RGBA16_SNORM,

      // r unsigned int
      COLOR_R8_UINT,
      COLOR_R16_UINT,
      COLOR_R32_UINT,

      // rg unsigned int
      COLOR_RG8_UINT,
      COLOR_RG16_UINT,
      COLOR_RG32_UINT,

      // rgb unsigned int
      COLOR_RGB32_UINT,

      // rgba unsigned int
      COLOR_RGBA8_UINT,
      COLOR_RGBA16_UINT,
      COLOR_RGBA32_UINT,

      // r signed int
      COLOR_R8_SINT,
      COLOR_R16_SINT,
      COLOR_R32_SINT,

      // rg signed int
      COLOR_RG8_SINT,
      COLOR_RG16_SINT,
      COLOR_RG32_SINT,

      // rgb signed int
      COLOR_RGB32_SINT,

      // rgba signed int
      COLOR_RGBA8_SINT,
      COLOR_RGBA16_SINT,
      COLOR_RGBA32_SINT,

      // r float
      COLOR_R16_FLOAT,
      COLOR_R32_FLOAT,

      // rg float
      COLOR_RG16_FLOAT,
      COLOR_RG32_FLOAT,

      // rgb float
      COLOR_RGB32_FLOAT,

      // rgba float
      COLOR_RGBA16_FLOAT,
      COLOR_RGBA32_FLOAT,

      // special formats
      COLOR_RGB10A2_UNORM,
      COLOR_RGB10A2_UINT,
      COLOR_RG11B10_FLOAT,
      COLOR_RGB9_EXP5,
      COLOR_SRGB8_UNORM,

      // compressed format
      COLOR_COMPRESSED_R,
      COLOR_COMPRESSED_RG,
      COLOR_COMPRESSED_RGBA,
      COLOR_COMPRESSED_SRGBA,
    };

    // format of data specifically in depth buffers
    enum DEPTH_FORMAT
    {
        COLOR_D16_UNORM,
        COLOR_D32_FLOAT,
        COLOR_D24_UNORM_S8_UINT,
        COLOR_D32_FLOAT_S8X24_UINT,
    };

    // informs what type of comparison should be used in specific operations
    enum COMPARISON_FUNC
    {
        COMPARE_NEVER,
        COMPARE_ALWAYS,
        COMPARE_EQUAL,
        COMPARE_NOT_EQUAL,
        COMPARE_LESS,
        COMPARE_LESS_EQUAL,
        COMPARE_GREATER,
        COMPARE_GREATER_EQUAL
    };

    // tells the renderer how to map a buffer of data
    enum MAP_TYPE
    {
        MAP_READ,
        MAP_WRITE,
        MAP_READ_WRITE
    };

    // describes a sampler
    struct SAMPLE_DESC
    {
        unsigned int count;
        unsigned int quality;
    };

//***********************************************************************************
// Abstract Component:
//	  There are two main component types in the pipeline. Shaders and the resources
//	  they utilize.
//
//			- Hardware Buffers are collections of data that a shader iterates through
//			  during execution
//			- Shader Resources are made available during shader execution to sample
//			  from or use, normally persistant to each run of the shader until changed
//			- a Mappable Resource is capable of being used
//			- a Shader is a program that takes data and uses the graphics coprocessor
//		      to perform operations on each element in the data
//			- a Render Target is any type of resource that can recieve the output of
//			  of the shader program
//
//	**WARNING** an object can be both a shader resource and render target at the same
//				time but CANNOT be both in the same active pipeline. For example: a
//				a 2D texture cannot be attached to both a ResourceGroup and FrameBuffer
//				who are both set in the pipeline. You now travel into the realm of
//				the unknown.
//***********************************************************************************

//------------------------------------------------------------------------------------------
// IRenderUtility::IMappable declaration
//------------------------------------------------------------------------------------------
    class IMappable
    {
    public:
        // virtual destructor for derived classes
        virtual ~IMappable() {}
    };

//------------------------------------------------------------------------------------------
// IRenderUtility::IHardwareBuffer declaration implements IRenderUtility::IMappable
//------------------------------------------------------------------------------------------
    class IHardwareBuffer :
        public virtual IMappable
    {
    public:
        struct INIT_DESC
        {
            USAGE Usage;
            void * Data;
            unsigned int NumElements;
            unsigned int ElementSize;
            unsigned int NumAttribs;
            std::vector<std::string> AttribName;
            std::vector<unsigned int> AttribSize;
            std::vector<unsigned int> AttribOffset;
        };

    public:
        // virtual destructor for derived classes
        virtual ~IHardwareBuffer() {}

        // obtains the resource usage hint
        virtual IRenderUtility::USAGE GetUsageType() const = 0;
    };

//------------------------------------------------------------------------------------------
// IRenderUtility::IShaderResource declaration implements IRenderUtility::IMappable
//------------------------------------------------------------------------------------------
    class IShaderResource :
        public virtual IMappable
    {
    public:
        struct INIT_DESC
        {
            USAGE Usage;
        };

    public:
        // virtual destructor for derived classes
        virtual ~IShaderResource() {}
    };

//------------------------------------------------------------------------------------------
// IRenderUtility::IShader declaration
//------------------------------------------------------------------------------------------
    class IShader
    {
    public:
        struct INIT_DESC
        {
            std::string fileName;
            std::string entryPoint;
        };

    public:
        // virtual destructor for derived classes
        virtual ~IShader() {}

        virtual void cache() = 0;

        virtual void uncache() = 0;
    };

//------------------------------------------------------------------------------------------
// IRenderUtility::IRenderTarget declaration
//------------------------------------------------------------------------------------------
    class IRenderTarget
    {
    public:
        struct INIT_DESC
        {
        };

    public:
        // virtual destructor for derived classes
        virtual ~IRenderTarget() {}
    };

//***********************************************************************************
// Components:
//	  a component is an individual collection of data or a step in the pipeline.
//			- an Input layout describes to a shader the layout of the vertex data set
//			  It's layout index must match the name of the shader.
//			- a vertex buffer is a shader resource that contains only 3D vertex data
//			- an index buffer is a shader resource that lists the order in which to
//		      calculate vertices in a vertex buffer
//			- a constant buffer is a shader resource which contains any sort of
//			  structure data you might need in the shader. Can be edited later.
//			- a 1D texture is a shader resource that contains pixel data with a given
//			  width.
//			- a 2D texture is a shader resource that contains pixel data with a given
//		      width and height. Like a 2D array.
//			- a 3D texture is a shader resource that contains pixel data with a given
//			  width, height, and depth. like a 3D array
//			- a depth buffer is a shader resource that contains depth data per pixel
//			- a vertex shader is a shader program that takes vertices from 3d space
//	    	  and puts them in world/view/projection space
//			- a fragment shader is a shader program that takes the fragments generated
//		      from rasterization and determines the color needed at each screen coordinate.
//*************************************************************************************

//------------------------------------------------------------------------------------------
// IRenderUtility::IInputLayout declaration
//------------------------------------------------------------------------------------------
    class IInputLayout
    {
    public:
        struct LayoutElement
        {
            std::string semantic;
            COLOR_FORMAT format;
            unsigned int offset;
        };

        struct INIT_DESC
        {
            std::vector<LayoutElement> elements;
        };

    public:
        // virtual destructor for derived classes
        virtual ~IInputLayout() {}
    };

//------------------------------------------------------------------------------------------
// IRenderUtility::IDepthBuffer declaration
//------------------------------------------------------------------------------------------
    class IDepthBuffer
    {
    public:
        enum STENCIL_OP
        {
            STENCIL_OP_KEEP,
            STENCIL_OP_ZERO,
            STENCIL_OP_REPLACE,
            STENCIL_OP_INCR_SAT,
            STENCIL_OP_DECR_SAT,
            STENCIL_OP_INVERT,
            STENCIL_OP_INCR,
            STENCIL_OP_DECR
        };

        struct INIT_DESC
        {
            bool depthEnable;
            USAGE usage;
            SAMPLE_DESC sampleDesc;
            DEPTH_FORMAT depthFormat;
            COMPARISON_FUNC depthFunc;
            bool writeMask;
            int width;
            int height;

            struct
            {
                bool stencilEnable;
                char stencilMask;
                COMPARISON_FUNC stencilFunc;

                struct OperationDesc
                {
                    STENCIL_OP StencilFail;
                    STENCIL_OP StencilDepthFail;
                    STENCIL_OP stencilPass;
                } frontface, backface;

            } StencilDesc;
        };

    public:
        // virtual destructor for derived classes
        virtual ~IDepthBuffer() {}
    };

    class IRenderContext
    {
    public:
        struct INIT_DESC
        {
            std::shared_ptr<IGui::IWindow> window;
            unsigned int depthBits;
            unsigned int stencilBits;
            unsigned int antialiasLevel;
        };

    };

//*************************************************************************************
//  Container std::shared_ptr:
//		Containers represent the 3 main components of the complete render cycle. The
//		data to manipuate, the programs to manipulate them, and the buffers to receive
//		the new output. These claim no ownership over components attached, and you must
//		be careful not to destruct them while they are active,
//
//			- a Resource Group is a collection of related buffers and layouts to
//			  be utilized by the shader program. The same buffer can be attached to
//			  different groups, allowing a wide range of configurations.
//			- a Shader Program is a collection of shaders from the top to the bottom
//			  of the pipeline. The same shader can be attached to different programs,
//			  allowing a wide range of configurations.
//			- a Frame Buffer is a collection of buffers that are used as render targets
//			  that act as the collective output of the shader program. The same buffer
//			  can be attached to different Frame Buffers, allowing a wide range of
//			  configurations.
//
//   **WARNING**
//	 a component may not be attached to both an ACTIVE Resource Group and Frame
//	 Buffer at the same time.
//*************************************************************************************

//------------------------------------------------------------------------------------------
// IRenderUtility::IResourceGroup declaration
//------------------------------------------------------------------------------------------
    class IResourceGroup
    {
    public:
        struct INIT_DESC
        {
        };

    public:
        // virtual destructor for derived classes
        virtual ~IResourceGroup() {}

        // attaches a vertex buffer to the resource group
        virtual void AttachBuffer(const std::shared_ptr<IHardwareBuffer> & buf) = 0;

        // attaches an input layout for use with the shader
        virtual void AttachInputLayout(const std::shared_ptr<IInputLayout> & buf) = 0;

        virtual void AttachShaderResource(const std::string& shader, std::shared_ptr<IShaderResource>& rec) = 0;


        virtual void DetachBuffer(const std::shared_ptr<IHardwareBuffer>& buf) = 0;

        virtual void DetachInputLayout() = 0;

        virtual void DetachShaderResource(const std::string& shader, std::shared_ptr<IShaderResource>& rec) = 0;

    };

//------------------------------------------------------------------------------------------
// IRenderUtility::IShaderProgram declaration
//------------------------------------------------------------------------------------------
    class IShaderProgram
    {
    public:
        struct INIT_DESC
        {
            std::vector<std::shared_ptr<IShader>> shaders;
        };

    public:
        // virtual destructor for derived classes
        virtual ~IShaderProgram() {}
    };

//------------------------------------------------------------------------------------------
// IRenderUtility::IFrameBuffer declaration
//------------------------------------------------------------------------------------------
    class IFrameBuffer
    {
    public:
        struct Target
        {
        };

        struct INIT_DESC
        {
        };

    public:
        // virtual desturctor for derived classes
        virtual ~IFrameBuffer() {}

        // attaches a render target at the index specified. must be 1 - 7. If there is a
        // target already in place, the input replaces it.
        virtual void AttachRenderTarget(const Target & buffer) = 0;

        // removes old depth buffer and attaches the one provided
        virtual void AttachDepthBuffer(const std::shared_ptr<IDepthBuffer> & depthBuffer) = 0;

        // detaches render target at provided pointer
        virtual void DetachRenderTarget(const std::shared_ptr<IRenderTarget> & targ) = 0;

        // detaches render target at provided index
        virtual void DetachRenderTarget(unsigned int i) = 0;

        // detaches the depth buffer if one is attached
        virtual void DetachDepthBuffer() = 0;
    };


//*************************************************************************************
//  Virtual Object Constructors:
//	    These are constructors that will create components shaders and objects which
//		are attached to containers, and finally set in the pipeline by the renderer.
//*************************************************************************************
public:
    // virtual destructor for derived classes
    virtual ~IRenderUtility() {}

    // **** COMPONENTS **** //

    virtual std::shared_ptr<IHardwareBuffer> CreateHardwareBuffer(const std::string& typeName, IHardwareBuffer::INIT_DESC& desc) const = 0;

    virtual std::shared_ptr<IShader> CreateShader(const std::string& typeName, IShader::INIT_DESC& desc) const = 0;

    virtual std::shared_ptr<IShaderResource> CreateShaderResource(const std::string& typeName, IShaderResource::INIT_DESC& desc) const = 0;

    virtual std::shared_ptr<IRenderTarget> CreateRenderTarget(const std::string& typeName, IRenderTarget::INIT_DESC& desc) const = 0;

    virtual std::shared_ptr<IRenderContext> CreateRenderContext(IRenderContext::INIT_DESC& desc) const = 0;

    // **** CONTAINERS **** //

    // creates a resource group to to contain vertex, index, layout, etc handles to be bound
    // to the pipeline together
    virtual std::shared_ptr<IResourceGroup> CreateResourceGroup(IResourceGroup::INIT_DESC & desc) const = 0;

    // creates a pipeline program to attach shaders to
    virtual std::shared_ptr<IShaderProgram> CreateShaderProgram(IShaderProgram::INIT_DESC & desc) const = 0;

    // creates a frame buffer object to attach textures and a depth buffer to for render output
    virtual std::shared_ptr<IFrameBuffer> CreateFrameBuffer(IFrameBuffer::INIT_DESC & desc) const = 0;
};

class IRenderer
{
public:
    // virtual destructor for derived classes
    virtual ~IRenderer() {}

    // to be called before the frame is commenced
    virtual void Begin( float red, float green, float blue, float alpha) = 0;

    // must be used before writing data to the buffer
    virtual void * Map( std::shared_ptr<IRenderUtility::IMappable> & buffer, IRenderUtility::MAP_TYPE) = 0;

    // must be used after writing data to the buffer
    virtual void Unmap( std::shared_ptr<IRenderUtility::IMappable> & buffer) = 0;

    virtual void SetRenderContext(std::shared_ptr<IRenderUtility::IRenderContext>& context) = 0;

    // used to set the group of resources to pushed through the shader program
    virtual void SetResoureGroup(std::shared_ptr<IRenderUtility::IResourceGroup>& resources) = 0;

    // used to set the programmable pipeline program object to use for draw calls
    virtual void SetShaderProgram( std::shared_ptr<IRenderUtility::IShaderProgram> & program) = 0;

    // sets a couple different render targets
    virtual void SetFrameBuffer(std::shared_ptr<IRenderUtility::IFrameBuffer> & frameBuf) = 0;

    // pushes data set in resource group through the set shader program and outputs to the
    // attached frame buffer
    virtual void Draw(unsigned int vertCount, unsigned int offset) = 0;

    // pushes data set in resource gorup through the set shader programs by index rather than
    // per vertex, and then outputs to the frame buffer
    virtual void DrawIndexed(unsigned int indexCount, unsigned int offset) = 0;

    // to be called after the frame is completed
    virtual void End() = 0;
};

typedef void (*RENDERER_CONSTRUCTOR)(std::shared_ptr<IRenderer> & renderObj, std::shared_ptr<IRenderUtility> & utilityObj);

#endif
