#	pragma once

#	include "Interface/ParticleSystemInterface.h"

#	include "AstralaxEmitterContainer.h"

#   ifndef MENGINE_UNSUPPORT_PRAGMA_WARNING
#	pragma warning(push, 0) 
#	endif 

#	include "magic.h"

#   ifndef MENGINE_UNSUPPORT_PRAGMA_WARNING
#	pragma warning(pop) 
#	endif

#	include <string>
#	include <vector>

namespace Menge
{
	class AstralaxEmitter 
		: public ParticleEmitterInterface
	{
	public:
		AstralaxEmitter();
		~AstralaxEmitter();

    public:
        bool initialize( ServiceProviderInterface * _serviceProvider, const AstralaxEmitterContainerPtr & _container, HM_EMITTER _id, const ConstString & _containerName, const ConstString & _emitterName );
		void finalize();

	public:
		const ConstString & getContainerName() const override;
		const ConstString & getEmitterName() const;

	public:
		const AstralaxEmitterContainerPtr & getContainer() const;

	public:
		void play() override;
		void stop() override;
		void pause() override;
		void restart() override;
		bool update( float _timing, bool & _stop ) override;
		void setLeftBorder( float _leftBorder ) override;
		void seek( float _timming ) override;
		void setLoop( bool _loop ) override;
		bool getLoop() const override;
		
		float getLeftBorder() const override;
		float getRightBorder() const override;
		float getDuration() const override;
		
		void setRandomMode( bool _randomMode ) override;
		bool getRandomMode() const override;

		bool isBackground() const override;

	public:
		void interrupt() override;

	public:
		bool is3d() const override;
		bool getCamera( ParticleCamera & _camera ) const override;

	public:
		bool flushParticles( ParticleMesh * _meshes, size_t _meshLimit, ParticleVertices * _particles, size_t _particlesLimit, ParticleEmitterRenderFlush & _flush ) override;

	public:
		//void getBoundingBox( int & left, int & top, int & right, int & bottom )  const override;
		void getBoundingBox( mt::box2f& _box ) const override;
		//void getBoundingBox( Rect & _rect )  const override;
		void setEmitterTranslateWithParticle( bool _value ) override;
		bool isIntensive() const override;

	public:
		bool changeEmitterImage( int _width, int _height, unsigned char* _data, int _bytes ) override;
		bool changeEmitterModel( float * _points, int _count ) override;
		
	public:
		void setPosition( const mt::vec3f & _pos ) override;
		void getPosition( mt::vec3f & _pos ) override;

        void getBasePosition( mt::vec3f & _pos ) override;

		void setScale( float _scale ) override;

		void setAngle( float _radians ) override;

        bool getBackgroundBox( mt::box2f & _box ) override;

	public:
		HM_EMITTER getId() const;

		bool inInterval() const;
		bool createFirstRenderedParticlesList( MAGIC_RENDERING * _rendering );

    public:
		float getUpdateTemp() const;

    protected:
        bool setupBasePosition_();
		
	protected:
        ServiceProviderInterface * m_serviceProvider; 
		AstralaxEmitterContainerPtr m_container;

		HM_EMITTER m_emitterId;
        mt::vec3f m_basePosition;	

		ConstString m_containerName;
		ConstString m_emitterName;

		double m_tempScale;
		double m_leftBorder;
		double m_rightBorder;
		
        double m_rate;
		double m_total_rate;

        double m_time;
		
		float m_angle;
		//float m_posX, m_posY; 

		float m_factor[20];
		int m_typesCount;

		bool m_start;
		bool m_looped;

        bool m_firstUpdate;
		bool m_background;
	};
}