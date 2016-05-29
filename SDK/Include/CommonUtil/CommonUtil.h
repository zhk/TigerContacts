/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			CommonUtil.h
*	Description:
*			The only interface file to other depended packages
*			of package CommonUtil.
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
===============================================================================*/
#pragma	once

#include "PreInclude.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

//Add all exports here...
#	include "Atomic_Op_T.h"
#	include "base64.h"
#	include "CommandLineParser.h"
#	include "Clock.h"
#	include "CommonTemplate.h"
#	include "DataBlock.h"
#	include "DBCFile.h"
#	include "FileConfig.h"
#	include "FileSystem.h"
#	include "GString.h"

#	include "IClockObserver.h"
#	include "Logger.h"
#	include "Mutex.h"
#	include "NatureTime.h"

#	include "OSFileSystem.h"
#	include "Path.h"
#	include "Packet.h"
#	include "PkgFileSystem.h"
#	include "Profiler.h"
#	include "ResourceProvider.h"
#	include "Semaphore.h"
#	include "SharedMemory.h"
#	include "StringHelper.h"
#	include "StaticMemPool.h"
#	include "sigslot.h"
#	include "SharedPtr.h"
#	include "TimeSource.h"
#	include "Thread.h"
#	include "TGXML.h"

#	include "Updatable.h"
#	include "XMLDocHandler.h"
#	include "XMLOutStream.h"

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif