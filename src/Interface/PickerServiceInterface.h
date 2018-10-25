#pragma once

#include "Interface/InputSystemInterface.h"

#include "Kernel/IntrusivePtr.h"

#include "Config/Vector.h"

#include "math/vec2.h"

namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class Arrow> ArrowPtr;
    typedef IntrusivePtr<class Scene, class Node> ScenePtr;
    typedef IntrusivePtr<class Scriptable> ScriptablePtr;
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class PickerTrapInterface> PickerTrapInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    struct PickerTrapState
    {
        PickerTrapInterfacePtr trap;

        uint32_t id;
        bool picked;
        bool pressed;
        bool handle;
        bool dead;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class RenderViewportInterface> RenderViewportInterfacePtr;
    typedef IntrusivePtr<class RenderCameraInterface> RenderCameraInterfacePtr;
    typedef IntrusivePtr<class RenderScissorInterface> RenderScissorInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    class PickerTrapInterface
        : public InputHandlerInterface
    {
    public:
        virtual bool pick( const mt::vec2f& _point, const RenderViewportInterfacePtr & _viewport, const RenderCameraInterfacePtr & _camera, const Resolution & _contentResolution, const ArrowPtr & _arrow ) = 0;

    public:
        virtual bool onHandleMouseEnter( float _x, float _y ) = 0;
        virtual void onHandleMouseLeave() = 0;
        virtual void onHandleMouseOverDestroy() = 0;

    public:
        virtual PickerTrapState * propagatePickerTrapState() const = 0;
        virtual ScriptablePtr propagatePickerScriptable() = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<PickerTrapInterface> PickerTrapInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
    typedef Vector<PickerTrapInterfacePtr> VectorPickerTraps;
    //////////////////////////////////////////////////////////////////////////
    class PickerServiceInterface
        : public ServiceInterface
        , public InputHandlerInterface
    {
		SERVICE_DECLARE("PickerService");

    public:
        virtual void setBlock( bool _value ) = 0;
        virtual void setHandleValue( bool _value ) = 0;

    public:
        virtual void setArrow( const ArrowPtr & _arrow ) = 0;
        virtual void setScene( const ScenePtr & _scene ) = 0;

    public:
        virtual void setRenderViewport( const RenderViewportInterfacePtr & _viewport ) = 0;
        virtual void setRenderCamera( const RenderCameraInterfacePtr & _camera ) = 0;
        virtual void setRenderScissor( const RenderScissorInterfacePtr & _scissor ) = 0;

    public:
        virtual void update() = 0;
        virtual void clear() = 0;

    public:
        virtual bool pickTrap( const mt::vec2f& _point, VectorPickerTraps & _traps ) = 0;

    public:
        virtual PickerTrapState * regTrap( const PickerTrapInterfacePtr & _trap ) = 0;
        virtual void unregTrap( PickerTrapState * _id ) = 0;

        virtual void updateTraps() = 0;
        virtual void invalidateTraps() = 0;

    public:
        virtual uint32_t getPickerTrapCount() const = 0;

    public:
        virtual void handleMouseEnter( const InputMousePositionEvent & _event ) = 0;
        virtual void handleMouseLeave( const InputMousePositionEvent & _event ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<PickerServiceInterface> PickerServiceInterfacePtr;
	//////////////////////////////////////////////////////////////////////////
#define PICKER_SERVICE()\
    ((Mengine::PickerServiceInterface *)SERVICE_GET(Mengine::PickerServiceInterface))
}