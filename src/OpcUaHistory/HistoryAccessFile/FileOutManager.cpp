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
	, fileOutList_()
	, fileOutMap_()
	{
	}

	FileOutManager::~FileOutManager(void)
	{
		// cleanup files
		while (fileOutMap_.begin() != fileOutMap_.end())
		{
			FileOut* fileOut = fileOutMap_.begin()->second;
			fileOut->erase();
			fileOutMap_.erase(fileOutMap_.begin());
		}
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

	FileAccess::Result
	FileOutManager::write(
		const std::string& name,
		OpcUaDataValue& dataValue
	)
	{
		FileOut* fileOut = nullptr;

		// find or create file output entry
		FileOut::Map::iterator it;
		it = fileOutMap_.find(name);
		if (it == fileOutMap_.end()) {
			fileOut = createFileOutEntry(name);
		}
		else {
			fileOut = it->second;
		}
		if (fileOut == nullptr) {
			return FileAccess::FileNotOpen;
		}

		// write data value to file
		FileAccess::Result result = fileOut->write(dataValue);
		if (result != FileAccess::Success) {
			removeFileOutEntry(fileOut);
			return result;
		}

		updateFileOutEntry(fileOut);
		return FileAccess::Success;
	}

	FileOut*
	FileOutManager::createFileOutEntry(const std::string& name)
	{
		// FIXME: todo
		return nullptr;
	}

	void
	FileOutManager::removeFileOutEntry(FileOut* fileOut)
	{
		// FIXME: todo
	}

	void
	FileOutManager::updateFileOutEntry(FileOut* fileOut)
	{
		fileOut->remove();
		fileOutList_.pushBefore(fileOut);
	}

}
