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

#ifndef __OpcUaHistory_CSV_h__
#define __OpcUaHistory_CSV_h__

#include <boost/shared_ptr.hpp>
#include <vector>

namespace OpcUaHistory
{

	class CSV
	{
	  public:
		typedef boost::shared_ptr<CSV> SPtr;
		typedef std::vector<std::string> Line;

		typedef enum
		{
			Read,
			Write
		} Mode;

		typedef enum
		{
			Ok,
			Error,
			EndOfFile
		} State;

		CSV(void);
		~CSV(void);

		void delimiter(const std::string& delimiter);
		bool open(const std::string& fileName, Mode mode);
		bool close(void);
		bool readLine(Line& line);
		bool writeLine(Line& line);

	  private:
		std::string delimiter_;
		std::string fileName_;

	};

}

#endif
