/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			Path.h
*	Description:
*			
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
===============================================================================*/

#pragma once

#include "PreInclude.h"

namespace Cactus
{
	/**
	A string represents a directory.
	*/
	class COMMONUTIL_API Path : public Object
	{
	public:

		/** Default constructor get a Path represents the current working directory.
		*/
		Path();

		/**	Construct a Path use a file's path.
		* @param pathname			Can be both absolute and relative.
		* @param bForceDirectory	If it is false, if the last section of pathname has a '.',
									it will be treated as a file, otherwise a directory.
		*/
		Path( const Cactus::String& pathname, bool bForceDirectory = true );

		/**	It is a directory or filename.
		* @return	true for directory
		*/
		bool					IsDirectory() const { return _bIsDirectory; }

		/**	Catch some string to the current Path. Current Path must be a directory.
		* @pathname					Some path like string, if it is an absolute path, s_exPathWrongParameter will throw.
		* @param bForceDirectory	If it is false, if the last section of pathname has a '.',
									it will be treated as a file, otherwise a directory.
		* @bIncludeLast				Whether catch the last section of the pathname.
		* @return					New full path string. If the current Path is a file, s_exPathTypeMismatch will throw.
		*/
		Cactus::String			Catch(const Cactus::String& pathname, bool bForceDirectory = true, bool bIncludeLast = true);

		/** Form a new path string base the current Path. Like Catch but does not modify the current Path.
		*/
		Cactus::String			FixedUp(const Cactus::String& pathname, bool bForceDirectory = true, bool bIncludeLast = true) const;

		/**	Change file ext for a file path. If the current Path is a directory, s_exPathTypeMismatch will throw.
		*/
		void					ChangeExtension( const Cactus::String& newExt );

		/**	Get the relative path string for 'filename' to the current Path.
		* @filename		Some path like string.
		* @return		Return a relative path string. If the current Path is a file, s_exPathTypeMismatch will throw.
		*/
		Cactus::String			Relative( const Cactus::String& filename ) const;

		/** Get the relative path string for 'path' to the current Path.
		* @path			A Path.
		* @return		Return a relative path string. If the current Path is a file, s_exPathTypeMismatch will throw.
		*/
		Cactus::String			Relative( const Cactus::Path& path ) const;

		/** Get the relative path to the current working directory. Null string for no solution.
		*/
		Cactus::String			RelativeToWorkingDir() const;
	
		/**	Returns the directory information. Validate both for a directory or filename.
		*/
		Cactus::String			GetDirectoryName() const;

		/** Return a file extension. If the current Path is a directory, s_exPathTypeMismatch will throw.
		*/
		Cactus::String			GetFileExtension() const;

		/**	Returns the file name and extension. If the current Path is a directory, s_exPathTypeMismatch will throw.
		*/
		Cactus::String			GetFileName() const;
		
		/**	Returns the file name. If the current Path is a directory, s_exPathTypeMismatch will throw.
		*/
		Cactus::String			GetFileTitle() const;

		/**	Get the string for the Path. Validate both for a directory or filename.
		*/
		Cactus::String			GetFullString() const;

		/**	Get the abbreviate string for the Path. Validate both for a directory or filename.
		* @param sz		Desired output string length.
		*/
		Cactus::String			GetAbbreviateString(size_t sz) const;

		/** Gets the root directory information.
		*/
		Cactus::String			GetPathRoot() const;

		/**	Gets a validate file name in current path.
		*/
		Cactus::String			GetRandomFileName() const;

	public:

		/**	Fix '\'to '/'.
		*/
		static Cactus::String	FixSeparator(const String& str, char chDesiredSep = '/');

		/** Split Path into sections.
		*/
		static size_t			Split(StringVector& out,
									const Cactus::String& path,
									bool include_last = true
									);

		/** Return a relative string.
		*/
		static Cactus::String	RelativeToDir(const Cactus::String& path,
									const Cactus::String& base_directory
									);

	private:
		StringVector	_Paths;
		bool			_bIsDirectory;

		/** Is the string a file name?
		*/
		static bool				_IsFileName(const Cactus::String& str);

		/** Is the string an absolute path?
		*/
		static bool				_IsAbsolutePath(const Cactus::String& str);

		/** Connect StringVector into a path string.
		*/
		static Cactus::String	_Connect(const Cactus::StringVector& sects, bool bDir = true, bool bIncludeLast = true);

		/** Construct a Path from the pathname.
		* @param pathname	pathname should be an absolute path. Can not be null.
		*/
		void					_Construct(const Cactus::String& pathname, bool bForceDirectory = true);

		// "The current Path is not a directory nor filename as you required!";
		static std::logic_error	s_exPathTypeMismatch;

		// "Wrong parameter for the current Path type!";
		static std::logic_error	s_exPathWrongParameter;
	};
}
