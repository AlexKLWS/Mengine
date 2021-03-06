#include "EntityPrototypeGenerator.h"

#include "Interface/StringizeServiceInterface.h"
#include "Interface/PrototypeServiceInterface.h"
#include "Interface/ScriptServiceInterface.h"

#include "Environment/Python/PythonEventReceiver.h"

#include "PythonEntityBehavior.h"

#include "Kernel/FactoryPool.h"
#include "Kernel/AssertionFactory.h"
#include "Kernel/AssertionMemoryPanic.h"
#include "Kernel/Logger.h"
#include "Kernel/Document.h"

#include "pybind/pybind.hpp"

namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    EntityPrototypeGenerator::EntityPrototypeGenerator()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    EntityPrototypeGenerator::~EntityPrototypeGenerator()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    void EntityPrototypeGenerator::setGenerator( const pybind::object & _generator )
    {
        m_generator = _generator;
    }
    //////////////////////////////////////////////////////////////////////////
    const pybind::object & EntityPrototypeGenerator::getGenerator() const
    {
        return m_generator;
    }
    //////////////////////////////////////////////////////////////////////////
    bool EntityPrototypeGenerator::initialize()
    {
        if( FactoryPrototypeGenerator::initialize() == false )
        {
            return false;
        }

        if( m_generator.is_callable() == false )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void EntityPrototypeGenerator::finalize()
    {
        FactoryPrototypeGenerator::finalize();

        m_eventable = nullptr;
        m_generator = nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    FactoryPtr EntityPrototypeGenerator::_initializeFactory()
    {
        FactoryPtr factory = Helper::makeFactoryPool<PythonEntityBehavior, 128>();

        return factory;
    }
    //////////////////////////////////////////////////////////////////////////
    const pybind::object & EntityPrototypeGenerator::getPythonType()
    {
        if( m_type.is_invalid() == false )
        {
            return m_type;
        }

        pybind::object py_type = m_generator.call( m_prototype );

        EventablePtr eventable = SCRIPT_SERVICE()
            ->eventableEntity( py_type );

        MENGINE_ASSERTION_MEMORY_PANIC( eventable, pybind::object::get_invalid(), "type '%s' prototype '%s' invalid eventable entity"
            , m_category.c_str()
            , m_prototype.c_str()
        );

        m_eventable = eventable;

        m_type = py_type;

        return m_type;
    }
    //////////////////////////////////////////////////////////////////////////
    FactorablePointer EntityPrototypeGenerator::generate( const Char * _doc )
    {
        const pybind::object & py_type = this->getPythonType();

        if( py_type.is_invalid() == true )
        {
            return nullptr;
        }

        pybind::object py_entity = py_type.call();

        if( py_entity.is_invalid() == true )
        {
            LOGGER_ERROR( "can't create object '%s' '%s' (invalid create) doc '%s'"
                , m_category.c_str()
                , m_prototype.c_str()
                , _doc
            );

            return nullptr;
        }

        EntityPtr entity = PROTOTYPE_SERVICE()
            ->generatePrototype( STRINGIZE_STRING_LOCAL( "Node" ), m_category, _doc );

        MENGINE_ASSERTION_MEMORY_PANIC( entity, nullptr, "can't generate '%s' '%s' doc '%s'"
            , m_category.c_str()
            , m_prototype.c_str()
            , _doc
        );

        entity->setPrototype( m_prototype );

        entity->setBehaviorEventable( m_eventable );

        const FactoryPtr & factory = this->getFactory();

        PythonEntityBehaviorPtr behavior = factory->createObject( _doc );
        behavior->setScriptObject( py_entity );

        entity->setBehavior( behavior );

        entity->onCreate();

        return entity;
    }
}