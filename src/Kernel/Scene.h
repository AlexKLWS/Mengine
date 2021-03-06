#pragma once

#include "Kernel/Entity.h"
#include "Kernel/Scriptable.h"
#include "Kernel/Layer.h"
#include "Kernel/DummyPicker.h"

#include "math/vec3.h"
#include "math/vec4.h"

namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    enum SceneEventFlag
    {
        EVENT_SCENE_APP_MOUSE_LEAVE = __EVENT_ENTITY_LAST__,
        EVENT_SCENE_APP_MOUSE_ENTER,
        EVENT_SCENE_FOCUS,
    };
    //////////////////////////////////////////////////////////////////////////
    class SceneEventReceiver
        : public EntityEventReceiver
    {
    public:
        virtual bool onSceneAppMouseLeave( const EntityBehaviorInterfacePtr & _behavior ) = 0;
        virtual bool onSceneAppMouseEnter( const EntityBehaviorInterfacePtr & _behavior ) = 0;
        virtual bool onSceneAppFocus( const EntityBehaviorInterfacePtr & _behavior, bool _focus ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<SceneEventReceiver> SceneEventReceiverPtr;
    //////////////////////////////////////////////////////////////////////////
    class Scene
        : public Entity
        , public DummyPicker
    {
        DECLARE_VISITABLE( Entity );
        DECLARE_EVENTABLE_TYPE( SceneEventReceiver );
        DECLARE_PICKER();

    public:
        Scene();
        ~Scene() override;

    public:
        void onAppMouseLeave();
        void onAppMouseEnter();

    public:
        void onFocus( bool _focus );

    protected:
        const RenderViewportInterfacePtr & getPickerViewport() const override;
        const RenderCameraInterfacePtr & getPickerCamera() const override;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusiveNodePtr<Scene> ScenePtr;
    //////////////////////////////////////////////////////////////////////////
}
