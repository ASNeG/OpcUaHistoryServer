/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaHistory/HistoryAccessFile/FileOutManager.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	FileOutManager::FileOutManager(void)
	: basePath_("./")
	, maxNumberFilesPerDirectory_(1000)
	, maxFileSize_(1000000)
	, maxOpenFiles_(100)
	{
	}

	FileOutManager::~FileOutManager(void)
	{
	}

	void
	FileOutManager::basePath(const std::string& basePath)
	{
		basePath_ = basePath;
	}

	std::string&
	FileOutManager::basePath(void)
	{
		return basePath_;
	}

	void
	FileOutManager::maxNumberFilesPerDirectory(uint32_t maxNumberFilesPerDirectory)
	{
		maxNumberFilesPerDirectory_ = maxNumberFilesPerDirectory;
	}

	uint32_t
	FileOutManager::maxNumberFilesPerDirectory(void)
	{
		return maxNumberFilesPerDirectory_;
	}

	void
	FileOutManager::maxFileSize(uint32_t maxFileSize)
	{
		maxFileSize_ = maxFileSize;
	}

	uint32_t
	FileOutManager::maxFileSize(void)
	{
		return maxFileSize_;
	}

	void
	FileOutManager::maxOpenFiles(uint32_t maxOpenFiles)
	{
		maxOpenFiles_ = maxOpenFiles;
	}

	uint32_t
	FileOutManager::maxOpenFiles(void)
	{
		return maxOpenFiles_;
	}

	void
	FileOutManager::write(
		const std::string& name,
		OpcUaDataValue& dataValue
	)
	{
	}

}
