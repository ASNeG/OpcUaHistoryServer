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

#ifndef __OpcUaHistory_HistoryStoreModelValuesConfig_h__
#define __OpcUaHistory_HistoryStoreModelValuesConfig_h__

#include "OpcUaStackCore/Base/Config.h"
#include "OpcUaHistory/History/OpcUaReferenceConfig.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

    class HistoryStoreModelValueConfig
    {
      public:
    	typedef boost::shared_ptr<HistoryStoreModelValueConfig> SPtr;
    	typedef std::vector<HistoryStoreModelValueConfig::SPtr> Vec;

    	HistoryStoreModelValueConfig(void);
    	~HistoryStoreModelValueConfig(void);

    	void configFileName(const std::string& configFileName);
    	void elementPrefix(const std::string& elementPrefix);
    	std::string& name(void);
    	OpcUaReferenceConfig::Vec& serverVec(void);
    	OpcUaReferenceConfig::Vec& clientVec(void);

    	bool decode(Config& config);

      private:
    	std::string configFileName_;
    	std::string elementPrefix_;
    	std::string name_;
    	OpcUaReferenceConfig::Vec serverVec_;
    	OpcUaReferenceConfig::Vec clientVec_;
    };


	class HistoryStoreModelValuesConfig
	{
	  public:
		typedef std::vector<std::string> NamespaceUris;

		HistoryStoreModelValuesConfig(void);
		~HistoryStoreModelValuesConfig(void);

    	void configFileName(const std::string& configFileName);
    	void elementPrefix(const std::string& elementPrefix);
		NamespaceUris& namespaceUris(void);
		HistoryStoreModelValueConfig::Vec& valueVec(void);

		bool decode(Config& config);

	  private:
		bool decodeNamespaceUris(Config& config);
		bool decodeValues(Config& config);

		std::string configFileName_;
		std::string elementPrefix_;
		NamespaceUris namespaceUris_;
		HistoryStoreModelValueConfig::Vec valueVec_;

	};

}

#endif
