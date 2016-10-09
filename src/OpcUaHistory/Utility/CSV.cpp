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

#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaHistory/Utility/CSV.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	CSV::CSV(void)
	: delimiter_(",")
	, fileName_("")
	{
	}

	CSV::~CSV(void)
	{
	}

	void
	CSV::delimiter(const std::string& delimiter)
	{
		delimiter_ = delimiter;
	}

	bool
	CSV::open(const std::string& fileName, Mode mode)
	{
		// FIXME: todo
		return false;
	}

	bool
	CSV::close(void)
	{
		// FIXME: todo
		return false;
	}

	bool
	CSV::readLine(Line& line)
	{
		// FIXME: todo
		return false;
	}

	bool
	CSV::writeLine(Line& line)
	{
		// FIXME: todo
		return false;
	}

}
