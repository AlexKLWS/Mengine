#	pragma once

#	include "Interface/HttpSystemInterface.h"

#	include "Core/ServiceBase.h"

#	include "Factory/Factory.h"

#	include "Kernel/ThreadTask.h"

namespace Menge
{
	class MockupHttpSystem
		: public ServiceBase<HttpSystemInterface>
		, public HttpReceiver
	{
	public:
		MockupHttpSystem();

	protected:
		bool _initialize() override;
		void _finalize() override;

	protected:
		HttpRequestID getMessage( const String & _url, HttpReceiver * _receiver ) override;
		HttpRequestID postMessage( const String & _url, const TMapParams & _params, HttpReceiver * _receiver ) override;
		HttpRequestID downloadAsset( const String & _url, const ConstString & _category, const FilePath & _path, HttpReceiver * _receiver ) override;

	protected:
		bool cancelRequest( HttpRequestID _id ) override;

	private:
		void onDownloadAssetComplete( HttpRequestID _id, const OutputStreamInterfacePtr & _stream, uint32_t _code, bool _successful ) override;
		void onPostMessageComplete( HttpRequestID _id, const String & _response, uint32_t _code, bool _successful ) override;
		void onGetMessageComplete( HttpRequestID _id, const String & _response, uint32_t _code, bool _successful ) override;

	protected:
		uint32_t m_enumeratorReceivers;

		struct HttpReceiverDesc
		{
			uint32_t id;
			ThreadTaskPtr task;
			HttpReceiver * receiver;
		};

		typedef stdex::vector<HttpReceiverDesc> TVectorHttpReceiverDesc;
		TVectorHttpReceiverDesc m_receiverDescs;

		FactoryPtr m_factoryTaskDummy;

	};
}