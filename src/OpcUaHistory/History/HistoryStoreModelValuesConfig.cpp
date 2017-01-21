/*
   Copyright 2017 Kai Huebl (kai@huebl-sgh.de)

   Lizenziert gemäß Apache Licence Version 2.0 (die „Lizenz“); Nutzung dieser
   Datei nur in Übereinstimmung mit der Lizenz erlaubt.
   Eine Kopie der Lizenz erhalten Sie auf http://www.apache.org/licenses/LICENSE-2.0.

   Sofern nicht gemäß geltendem Recht vorgeschrieben oder schriftlich vereinbart,
   erfolgt die Bereitstellung der im Rahmen der Lizenz verbreiteten Software OHNE
   GEWÄHR ODER VORBEHALTE – ganz gleich, ob ausdrücklich oder stillschweigend.

   Informationen über die jeweiligen Bedingungen für Genehmigungen und Einschränkungen
   im Rahmen der Lizenz finden Sie in der Lizenz.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include <boost/algorithm/string.hpp>
#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaHistory/History/HistoryStoreModelValuesConfig.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	HistoryStoreModelValuesConfig::HistoryStoreModelValuesConfig(void)
	: configFileName_("")
	, namespaceUris_()
	{
	}

	HistoryStoreModelValuesConfig::~HistoryStoreModelValuesConfig(void)
	{
	}

	bool
	HistoryStoreModelValuesConfig::decode(const std::string& configFileName, Config& config)
	{
		configFileName_ = configFileName;

		// get Values element
		boost::optional<Config> valuesConfig = config.getChild("Values");
		if (!valuesConfig) {
			Log(Error, "element missing in config file")
				.parameter("Parameter", "HistoryStoreModel.Values")
				.parameter("ConfigFileName", configFileName_);
			return false;
		}

		// decode NamespaceUris element
		if (!decodeNamespaceUris(*valuesConfig)) {
			return false;
		}

		// decode Values element
		if (!decodeValues(*valuesConfig)) {
			return false;
		}

		return true;
	}

	bool
	HistoryStoreModelValuesConfig::decodeNamespaceUris(Config& config)
	{
		// get NamespaceUris element
		boost::optional<Config> namespaceUris = config.getChild("NamespaceUris");
		if (!namespaceUris) {
			Log(Error, "element missing in config file")
				.parameter("Parameter", "HistoryStoreModel.Values.NamespaceUris")
				.parameter("ConfigFileName", configFileName_);
			return false;
		}

		// get Uri elements
		config.getValues("Uri", namespaceUris_);
		if (namespaceUris_.size() == 0) {
			Log(Error, "element missing in config file")
				.parameter("Parameter", "HistoryStoreModel.Values.NamespaceUris.Uri");
			return false;
		}

		return true;
	}

	bool
	HistoryStoreModelValuesConfig::decodeValues(Config& config)
	{
		// FIXME: todo
		return true;
	}

#if 0
	<Values>
		<NamespaceUris>
		    <Uri>http://ASNeG-Demo.de/Test-Server-Lib/</Uri>
			<Uri>http://ASNeG.de/HistoryServer/</Uri>
	    </NamespaceUris>

		<Value Name="SByteValue">
        	<Server Operation="HRead"  NodeId="ns=1;i=200"/>
        	<Server Operation="HWrite" NodeId="ns=1;i=200"/>
        	<Server Operation="Mon"    NodeId="ns=1;i=200"/>
        	<Client Operation="Mon"    NodeId="ns=2;i=200" MonItem="Client.Subst.MonItem"/>
		</Value
#endif

}
