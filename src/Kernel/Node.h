#	pragma once

#	include "Kernel/Identity.h"
#	include "Kernel/Scriptable.h"
#	include "Kernel/Eventable.h"
#	include "Kernel/Resource.h"
#	include "Kernel/GlobalHandleAdapter.h"
#	include "Kernel/Updatable.h"
#	include "Kernel/Renderable.h"
#	include "Kernel/BoundingBox.h"
#	include "Kernel/Transformation3D.h"
#	include "Kernel/Colorable.h"
#	include "Kernel/Affectorable.h"
#	include "Kernel/Visitable.h"

#	include "Core/ValueInterpolator.h"

#	include "Factory/Factorable.h"

#	include "Core/Viewport.h"

#   include "stdex/intrusive_slug_list_size.h"
#   include "stdex/intrusive_slug.h"

namespace Menge
{		
	class Layer;
	class Scene;

	struct RenderMaterial;

	class ResourceReference;

	class VisitorResource
	{
	public:
		virtual void visitResource( ResourceReference * _resource ) = 0;
	};

	class Node;

    typedef stdex::intrusive_slug_list_size<Node> TListNodeChild;
	typedef stdex::intrusive_slug<TListNodeChild> TSlugChild;

	class Node
        : public stdex::intrusive_slug_linked<Node>
		, public Factorable
		, public Identity
		, public Resource
		, public GlobalHandleAdapter
		, public Updatable
		, public Renderable
		, public BoundingBox
		, public Transformation3D
		, public Colorable
		, public Affectorable
        , public Visitable
        , public Eventable
		, public Scriptable
	{
        DECLARE_VISITABLE();

	public:
		Node();
		~Node();

    public:
        void setServiceProvider( ServiceProviderInterface * _serviceProvider );
        ServiceProviderInterface * getServiceProvider() const;

    protected:
        virtual void _setServiceProvider( ServiceProviderInterface * _serviceProvider );

    protected:
        ServiceProviderInterface * m_serviceProvider;

	public:
		virtual void setLayer( Layer * _layer );
		Layer * getLayer() const;

	public:
		virtual Scene * getScene();

	protected:
		Layer * m_layer;

	public:
		void render( const RenderViewportInterface * _viewport, const RenderCameraInterface * _camera, unsigned int _debugMask ) override;
		inline bool isRenderable() const;
		
	public:
		void setRenderViewport( const RenderViewportInterface * _viewport );
		const RenderViewportInterface * getRenderViewport() const;

	public:
		void setRenderCamera( const RenderCameraInterface * _camera );
		const RenderCameraInterface * getRenderCamera() const;

	protected:
		void _hide( bool _value ) override;
			
//#	ifndef MENGE_MASTER_RELEASE
	protected:
		void _debugRender( const RenderViewportInterface * _viewport, const RenderCameraInterface* _camera, unsigned int _debugMask ) override;
//#	endif

	protected:
		const RenderViewportInterface * m_renderViewport;
		const RenderCameraInterface * m_renderCamera;

	protected:
		void renderChild_( const RenderViewportInterface * _viewport, const RenderCameraInterface * _camera, unsigned int _debugMask );

	protected:
		bool isEnableGlobalHandle() const override;
        Eventable * getGlobalHandleEventable() override;
        Scriptable * getGlobalHandleScriptable() override;

	public:
		void getScreenPosition( const RenderCameraInterface * _camera, mt::vec2f & _position );
		
	public:
		const ColourValue & getWorldColor() const;
		void calcTotalColor( ColourValue & _color ) const;
        bool isSolidColor() const;

	protected:
		void _invalidateColor() override;

	protected:
		void setParent_( Node * _node );

	public:
		inline Node * getParent() const;
		inline bool hasParent() const;

		bool addChildren( Node * _node );
		bool addChildrenFront( Node* _node );
		bool addChildrenAfter( Node* _node, Node * _after );
		void removeChildren( Node * _node );        
		void removeAllChild();
		void removeFromParent();
		
		void destroyAllChild();

		TListNodeChild & getChild();

		Node * findChildren( const ConstString & _name, bool _recursion ) const;
		bool hasChildren( const ConstString & _name, bool _recursive ) const;
		bool emptyChild() const;

    protected:
        void removeChildren_( Node * _node );

	protected:
		virtual bool _hasChildren( const ConstString & _name, bool _recursive ) const;
		virtual Node * _findChildren( const ConstString & _name, bool _recursion ) const;

	protected:
		virtual void _changeParent( Node * _oldParent, Node * _newParent );
		virtual void _addChildren( Node * _node );
		virtual void _removeChildren( Node * _node );

	protected:
		Node * m_parent;

		TListNodeChild m_child;

	private:
		bool addChildren_( TListNodeChild::iterator _insert, Node * _node );

		void insertChildren_( TListNodeChild::iterator _insert, Node * _node );
		void eraseChildren_( Node * _node );

	public:
		void visitChild( Visitor * _visitor );

	public:
		void destroy() override;

	protected:
		void _destroy() override;
	
	public:
		bool activate();
		void deactivate();

	public:
		inline bool isActivate() const;

	protected:
		virtual bool _activate();
		virtual void _afterActivate();

		virtual void _deactivate();
		virtual void _afterDeactivate();

	protected:
		void _invalidateWorldMatrix() override;
		//void _invalidateBoundingBox() override;

	protected:
		bool _checkVisibility( const Viewport & _viewport ) override;

	public:
		bool compile() override;
		void release() override;

	protected:
		void _recompile() override;
        void _uncompile() override;

	public:
		bool enable();
		void disable();

	public:
		inline bool isEnable() const;

	public:
		void freeze( bool _value );
		inline bool isFreeze() const;

	protected:
		virtual void _freeze( bool _value );

	public:
		void update( float _current, float _timing ) override;

	protected:
		void _update( float _current, float _timing ) override;

	protected:
		void updateChild_( float _current, float _timing );
		
	protected:
		PyObject * _embedded() override;
		void _embedding( PyObject * _embed ) override;

	public:
		PyObject * setEventListener( PyObject * _args, PyObject * _kwds );
		void removeEventListener();

	protected:
		virtual void _setEventListener( PyObject * _listener );

	public:
		virtual void visitResource( VisitorResource * _visitor );

	protected:
		bool m_active;
		bool m_enable;

		bool m_freeze;

	protected:
		bool m_rendering;

	protected:
		void updateRendering_();

	protected:
		void getBoundingBox( mt::box2f & _boundingBox );

	protected:
		void _updateBoundingBox( mt::box2f& _boundingBox ) override;

	protected:
		size_t m_cameraRevision;

	public:
		void setShallowGrave(); //Shallow Grave prevents any damage from killing the targetted hero. 
		void removeShallowGrave();

		bool isShallowGrave() const;

	protected:
		int m_shallowGrave;
	};
	//////////////////////////////////////////////////////////////////////////
	inline bool Node::isActivate() const
	{
		return m_active;
	}
	//////////////////////////////////////////////////////////////////////////
	inline bool Node::isFreeze() const
	{
		return m_freeze;
	}
	//////////////////////////////////////////////////////////////////////////
	inline bool Node::isEnable() const
	{
		return m_enable;
	}
	//////////////////////////////////////////////////////////////////////////
	inline Node * Node::getParent() const
	{
		return m_parent;
	}
	//////////////////////////////////////////////////////////////////////////
	inline bool Node::hasParent() const
	{
		return m_parent != nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	inline bool Node::isRenderable() const
	{
		return m_rendering;
	}
}

