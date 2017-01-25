/*
   Copyright 2015-2017 Kai Huebl (kai@huebl-sgh.de)

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

#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaHistory/History/HistoryStore.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	HistoryStore::HistoryStore(void)
	: fileHistoryStore_()
	, historyStoreModelConfig_()
	{
	}

	HistoryStore::~HistoryStore(void)
	{
	}

    bool
    HistoryStore::startup(std::string& configFile, ConfigXmlManager& configXmlManager)
    {
    	// parse history model configuration
    	if (!historyStoreModelConfig_.decode(configFile, configXmlManager)) {
    		return false;
    	}

    	// check configuratiom
    	if (historyStoreModelConfig_.fileHistoryStoreConfig().activate()) {
    		return fileHistoryStore_.startup(&historyStoreModelConfig_.fileHistoryStoreConfig());
    	}

    	return false;
    }

    HistoryStoreIf*
    HistoryStore::historyStoreIf(void)
    {
    	return &fileHistoryStore_;
    }

    bool
    HistoryStore::shutdown(void)
    {
    	return fileHistoryStore_.shutdown();
    }

	void
	HistoryStore::clientNamespaces(NamespaceElement::Vec& namespaceElementVec)
	{
		HistoryStoreModelValuesConfig::NamespaceUris& namespaceUris = historyStoreModelConfig_.historyStoreModelValuesConfig().namespaceUris();
		HistoryStoreModelValuesConfig::NamespaceTypes& namespaceTypes = historyStoreModelConfig_.historyStoreModelValuesConfig().namespaceTypes();

		for (uint32_t idx=0; idx<namespaceUris.size(); idx++) {
			if ( namespaceTypes[idx] == HistoryStoreModelValuesConfig::Client ||
				 namespaceTypes[idx] == HistoryStoreModelValuesConfig::ClientServer ) {
				NamespaceElement namespaceElement;
				namespaceElement.namespaceIndex_ = idx+1;
				namespaceElement.namespaceName_ = namespaceUris[idx];
				namespaceElementVec.push_back(namespaceElement);
			}
		}
	}

	void
	HistoryStore::clientVariables(VariableElement::Vec& variableElementVec)
	{
	}

	void
	HistoryStore::serverNamespaces(NamespaceElement::Vec& namespaceElementVec)
	{
		HistoryStoreModelValuesConfig::NamespaceUris& namespaceUris = historyStoreModelConfig_.historyStoreModelValuesConfig().namespaceUris();
		HistoryStoreModelValuesConfig::NamespaceTypes& namespaceTypes = historyStoreModelConfig_.historyStoreModelValuesConfig().namespaceTypes();

		for (uint32_t idx=0; idx<namespaceUris.size(); idx++) {
			if ( namespaceTypes[idx] == HistoryStoreModelValuesConfig::Server ||
				 namespaceTypes[idx] == HistoryStoreModelValuesConfig::ClientServer ) {
				NamespaceElement namespaceElement;
				namespaceElement.namespaceIndex_ = idx+1;
				namespaceElement.namespaceName_ = namespaceUris[idx];
				namespaceElementVec.push_back(namespaceElement);
			}
		}
	}

	void
	HistoryStore::serverVariables(VariableElement::Vec& variableElementVec)
	{
	}

}
