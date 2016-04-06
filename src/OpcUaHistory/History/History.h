/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#ifndef __OpcUaHistory_History_h__
#define __OpcUaHistory_History_h__

#include <istream>
#include <map>
#include <stdint.h>

namespace OpcUaHistory
{

	class History
	{
	  public:
		History(void);
		~History(void);

		bool startup(void);
		bool shutdown(void);

	  private:
	};

}

#endif
