#	pragma once

#	include "Factory/Factorable.h"
#	include "Factory/Factory.h"

#	include "Resource.h"
#	include "Reference.h"

#	include "ResourceDeclare.h"



class XmlElement;

namespace Menge
{
	class ResourceVisitor;
	
	struct ResourceFactoryParam
	{
		Menge::String name;
		Menge::String category;
		Menge::String group;
		Menge::String file;
	};

	class ResourceReference
		: public Factorable
		, public Resource
		, public Reference
	{
	public:
		virtual void accept(ResourceVisitor * _visitor) = 0;

	public:
		ResourceReference();
		~ResourceReference();

	public:
		void initialize( const ResourceFactoryParam & _params );

	public:
		void setName( const String& _name );
		const String& getName() const;

		const ResourceFactoryParam& getFactoryParams() const;

	public:
		virtual void loader( XmlElement * _xml );

	protected:
		bool _incrementZero() override;
		void _decrementZero() override;

	protected:
		ResourceFactoryParam m_params;
	};
}
