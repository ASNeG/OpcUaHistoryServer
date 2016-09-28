/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)

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

#ifndef __OpcUaHistory_ClientSubscription_h__
#define __OpcUaHistory_ClientSubscription_h__

#include <boost/shared_ptr.hpp>
#include "OpcUaStackCore/Utility/IOThread.h"
#include <map>
#include <set>
#include <vector>
#include <stdint.h>

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class ClientSubscription
	{
	  public:
		typedef boost::shared_ptr<ClientSubscription> SPtr;
		typedef std::map<std::string, ClientSubscription::SPtr> Map;
		typedef std::set<ClientSubscription::SPtr> Set;

		typedef enum {
			S_Active,
			S_Deactive,
		} State;

		ClientSubscription(void);
		~ClientSubscription(void);

		void ioThread(IOThread::SPtr& ioThread);
		IOThread::SPtr& ioThread(void);

		void state(State state);
		State state(void);

	  private:
		IOThread::SPtr ioThread_;

		// runtime parameters
		State state_;
	};

}

#endif
