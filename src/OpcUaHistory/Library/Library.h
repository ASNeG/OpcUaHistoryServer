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

#ifndef __OpcUaHistory_Library_h__
#define __OpcUaHistory_Library_h__

#include "OpcUaStackCore/Base/ConfigXmlManager.h"
#include "OpcUaStackCore/Utility/IOThread.h"
#include "OpcUaStackServer/Application/ApplicationIf.h"
#include "OpcUaHistory/History/HistoryStore.h"
#include "OpcUaHistory/OpcUaClient/HistoryClientManager.h"
#include "OpcUaHistory/OpcUaServer/HistoryServerManager.h"

using namespace OpcUaStackCore;
using namespace OpcUaStackServer;

namespace OpcUaHistory
{

	class Library
	: public ApplicationIf
	{
	  public:
		Library(void);
		virtual ~Library(void);

		//- ApplicationIf -----------------------------------------------------
		virtual bool startup(void);
		virtual bool shutdown(void);
		//- ApplicationIf -----------------------------------------------------

	  private:
		IOThread::SPtr ioThread_;
		ConfigXmlManager configXmlManager_;
		HistoryStore historyStore_;
		HistoryClientManager historyClientManager_;
		HistoryServerManager historyServerManager_;
	};

}

#endif
