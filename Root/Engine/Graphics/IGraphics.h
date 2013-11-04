// Programmer: Wyatt Kennedy
// Date: 3/15/2013
// File: SYNC_Graphics.h
// This object derives from IGraphics

#ifndef SYNC_GRAPHICS_H
#define SYNC_GRAPHICS_H

#include <map>
#include "SYNC_Math.h"
#include "SYNC_IGraphics.h"
#include "SYNC_IRenderer.h"

namespace SYNC
{
	class Graphics :
		public IGraphics
	{
		struct INIT_DESC
		{


		};

	public:
		/** 
		 * Function declaration
		 * @name - CreateModel(const std::string& filename)
		 */
		std::shared_ptr<IModel> CreateModel(const std::string& mesh, 
			                                const std::string& textures);



	private:
		std::map<std::string, IRenderUtility::IVertexBuffer> m_ModelCache;
		std::shared_ptr<IRenderUtility> m_RenderUtility;
		std::shared_ptr<IRenderer> m_Renderer;
		SYNC::Vector3 m_ViewPort;
	};

	void CreateGraphicsManager(IGraphics::INIT_DESC & desc);
}

#endif