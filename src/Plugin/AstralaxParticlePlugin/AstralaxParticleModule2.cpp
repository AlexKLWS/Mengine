#include "AstralaxParticleModule2.h"

#include "AstralaxInterface.h"

#include "Interface/PrototypeServiceInterface.h"
#include "Interface/StringizeInterface.h"
#include "Interface/ScriptSystemInterface.h"

#include "ResourceAstralax.h"
#include "AstralaxEmitter.h"

#include "Environment/Python/PythonAnimatableEventReceiver.h"
#include "Environment/Python/PythonScriptWrapper.h"

#include "Kernel/NodePrototypeGenerator.h"
#include "Kernel/ResourcePrototypeGenerator.h"
#include "Kernel/Logger.h"

#include "pybind/pybind.hpp"

//////////////////////////////////////////////////////////////////////////
namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    namespace Detail
    {
        ////////////////////////////////////////////////////////////////////////////        
        static PyObject * s_ParticleEmitter2_setEventListener( pybind::kernel_interface * _kernel, AstralaxEmitter * _node, PyObject * _args, PyObject * _kwds )
        {
            (void)_args;

            if( _kwds == nullptr )
            {
                return _kernel->ret_none();
            }

            pybind::dict py_kwds( _kernel, _kwds );
            Helper::registerAnimatableEventReceiver<>( py_kwds, _node );

#ifndef NDEBUG
            if( py_kwds.empty() == false )
            {
                for( pybind::dict::iterator
                    it = py_kwds.begin(),
                    it_end = py_kwds.end();
                    it != it_end;
                    ++it )
                {
                    String k = it.key();

                    LOGGER_ERROR( "invalid kwds '%s'\n"
                        , k.c_str()
                    );
                }

                throw;
            }
#endif

            return _kernel->ret_none();
        }
    }
    //////////////////////////////////////////////////////////////////////////
    AstralaxParticleModule2::AstralaxParticleModule2()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    AstralaxParticleModule2::~AstralaxParticleModule2()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool AstralaxParticleModule2::_initialize()
    {
        LOGGER_INFO( "Initializing Particle System ..." );

        pybind::kernel_interface * kernel = SCRIPT_SERVICE()
            ->getKernel();

        pybind::set_kernel( kernel );

        pybind::interface_<AstralaxEmitter, pybind::bases<Node, Eventable, Animatable> >( kernel, "ParticleEmitter2", false )
            .def( "setResourceParticle", &AstralaxEmitter::setResourceParticle )
            .def( "getResourceParticle", &AstralaxEmitter::getResourceParticle )

            .def( "setEmitterTranslateWithParticle", &AstralaxEmitter::setEmitterTranslateWithParticle )
            .def( "setEmitterPositionRelative", &AstralaxEmitter::setEmitterPositionRelative )
            .def( "setEmitterCameraRelative", &AstralaxEmitter::setEmitterCameraRelative )
            .def( "setEmitterPositionProviderOriginOffset", &AstralaxEmitter::setEmitterPositionProviderOriginOffset )

            .def( "changeEmitterImage", &AstralaxEmitter::changeEmitterImage )
            .def( "removeEmitterImage", &AstralaxEmitter::removeEmitterImage )
            .def( "changeEmitterPolygon", &AstralaxEmitter::changeEmitterPolygon )
            .def( "removeEmitterPolygon", &AstralaxEmitter::removeEmitterPolygon )

            .def( "getDuration", &AstralaxEmitter::getDuration )

            .def( "setEmitterRandomMode", &AstralaxEmitter::setEmitterRandomMode )
            .def( "getEmitterRandomMode", &AstralaxEmitter::getEmitterRandomMode )
            .def_static_native_kernel( "setEventListener", &Detail::s_ParticleEmitter2_setEventListener )
            ;

        if( SCRIPT_SERVICE()
            ->setWrapper( STRINGIZE_STRING_LOCAL( "ParticleEmitter2" ), new PythonScriptWrapper<AstralaxEmitter>() ) == false )
        {
            return false;
        }

        if( PROTOTYPE_SERVICE()
            ->addPrototype( STRINGIZE_STRING_LOCAL( "Node" ), STRINGIZE_STRING_LOCAL( "ParticleEmitter2" ), new FactorableUnique<NodePrototypeGenerator<AstralaxEmitter, 128> > ) == false )
        {
            return false;
        }

        if( SCRIPT_SERVICE()
            ->setWrapper( STRINGIZE_STRING_LOCAL( "ResourceParticle" ), new PythonScriptWrapper<ResourceAstralax>() ) == false )
        {
            return false;
        }

        if( PROTOTYPE_SERVICE()
            ->addPrototype( STRINGIZE_STRING_LOCAL( "Resource" ), STRINGIZE_STRING_LOCAL( "ResourceParticle" ), new FactorableUnique<ResourcePrototypeGenerator<ResourceAstralax, 128> > ) == false )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void AstralaxParticleModule2::_finalize()
    {
        SCRIPT_SERVICE()
            ->removeWrapper( STRINGIZE_STRING_LOCAL( "ParticleEmitter2" ) );

        PROTOTYPE_SERVICE()
            ->removePrototype( STRINGIZE_STRING_LOCAL( "Node" ), STRINGIZE_STRING_LOCAL( "ParticleEmitter2" ) );

        SCRIPT_SERVICE()
            ->removeWrapper( STRINGIZE_STRING_LOCAL( "ResourceParticle" ) );

        PROTOTYPE_SERVICE()
            ->removePrototype( STRINGIZE_STRING_LOCAL( "Resource" ), STRINGIZE_STRING_LOCAL( "ResourceParticle" ) );

        pybind::kernel_interface * kernel = pybind::get_kernel();

        kernel->remove_scope<AstralaxEmitter>();
    }
    //////////////////////////////////////////////////////////////////////////
    void AstralaxParticleModule2::_destroy()
    {
    }
}