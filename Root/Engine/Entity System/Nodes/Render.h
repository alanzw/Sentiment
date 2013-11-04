

#include "Root/Engine/Entity System/Entity_Engine.h"
#include "Root/Engine/Graphics/IRenderer.h"
#include "Root/Engine/Entity System/Components/Mesh.h"

class Render :
    public node<Render>
{
public:

    void SetParameters();

private:
    std::shared_ptr<IRenderUtility::IResourceGroup> m_Group;
};
