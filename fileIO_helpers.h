#ifndef FILEIO_HELPERS_H__
#define FILEIO_HELPERS_H__


// Copyright (C) 2017 Kyaw Kyaw Htike @ Ali Abdul Ghafur. All rights reserved.


#include <windows.h> // for searching files in directories
#include <locale> // for wstring_convert, 
#include <codecvt> // for codecvt_utf8
#include <string>
#include <vector>

// this is just a helper function for future reference which is not even used in this file.
std::wstring string_to_wstring(const std::string& str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;
	return converterX.from_bytes(str);

	//note: all the code above can also done simply as a one liner:
	//return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);
}

// this is just a helper function for future reference which is not even used in this file.
std::string wstring_to_string(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;
	return converterX.to_bytes(wstr);

	//note: all of code above can also done simply as a one liner:
	//return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(wstr);
}


/*

======================================================
(1) dir_ff function
======================================================

search for file and directory names in a given search path.
Note: this function will only work on windows since it is using windows API to look for files and folders.

search_path can be a folder: 
e.g: std::string search_path = "C:\\Strawberry\\";

Note: it must end with \\

It can also be written as:
e.g: std::string search_path = "C:/Strawberry/";

search_path can also be specific file extensions:
e.g: std::string search_path = "D:\\Datasets\\images\\*.png";

The function outputs two vectors (std::vector<std::string>): for file names and folder names.

Full usage example for dir_ff function: 

int main(int argc, char* argv[])
{	
	
	vector<string> fnames;
	vector<string> foldernames;
	
	//dir_ff("D:/Datasets/images/*.png", fnames, foldernames);
	dir_ff("D:\\Datasets\\images\\*.png", fnames, foldernames);
	//dir_ff("C:\\Strawberry\\*.txt", fnames, foldernames);
	
	cout << "num of files found = " << fnames.size() << endl;
	cout << "num of folders found = " << foldernames.size() << endl;

	cout << "============ Files ==============" << endl;
	for (int i = 0; i < fnames.size(); i++)
		cout << fnames[i] << endl;

	cout << "============ Folders ==============" << endl;
	for (int i = 0; i < foldernames.size(); i++)
		cout << foldernames[i] << endl;
	

	return 0;
}


Example usage:

Imagine that in a given folder "C:/Users/Kyaw/Desktop/test_fold", there are the following 2 image files, 3 text files, 1 cpp file and 1 folder as follows:

cpan
DISTRIBUTIONS.txt
I00000.png
I00001.png
image processing in opencv.cpp
README.txt
relocation.txt

The following code:

	vector<string> fnames;
	vector<string> foldernames;
	dir_ff("C:/Users/Kyaw/Desktop/test_fold/", fnames, foldernames);

	cout << "num of files found = " << fnames.size() << endl;
	cout << "num of folders found = " << foldernames.size() << endl;

	cout << "============ Files ==============" << endl;
	for (int i = 0; i < fnames.size(); i++)
		cout << fnames[i] << endl;

	cout << "============ Folders ==============" << endl;
	for (int i = 0; i < foldernames.size(); i++)
		cout << foldernames[i] << endl;
	
will output:

num of files found = 0
num of folders found = 0
============ Files ==============
============ Folders ==============
Press any key to continue . . .

This is to be expected as the format is wrong. It should be:

	vector<string> fnames;
	vector<string> foldernames;
	dir_ff("C:/Users/Kyaw/Desktop/test_fold/*.*", fnames, foldernames);

	cout << "num of files found = " << fnames.size() << endl;
	cout << "num of folders found = " << foldernames.size() << endl;

	cout << "============ Files ==============" << endl;
	for (int i = 0; i < fnames.size(); i++)
		cout << fnames[i] << endl;

	cout << "============ Folders ==============" << endl;
	for (int i = 0; i < foldernames.size(); i++)
		cout << foldernames[i] << endl;
	

which will output:

num of files found = 6
num of folders found = 3
============ Files ==============
DISTRIBUTIONS.txt
I00000.png
I00001.png
image processing in opencv.cpp
README.txt
relocation.txt
============ Folders ==============
.
..
cpan
Press any key to continue . . .

Escaped Backslash can also be used:

dir_ff("C:\\Users\\Kyaw\\Desktop\\test_fold\\*.*", fnames, foldernames);

which will output the same output as above.	

Only files with a specific extension can be searched as shown below:

	vector<string> fnames;
	vector<string> foldernames;
	dir_ff("C:\\Users\\Kyaw\\Desktop\\test_fold\\*.png", fnames, foldernames);

	cout << "num of files found = " << fnames.size() << endl;
	cout << "num of folders found = " << foldernames.size() << endl;

	cout << "============ Files ==============" << endl;
	for (int i = 0; i < fnames.size(); i++)
		cout << fnames[i] << endl;

	cout << "============ Folders ==============" << endl;
	for (int i = 0; i < foldernames.size(); i++)
		cout << foldernames[i] << endl;
	
which will output:

num of files found = 2
num of folders found = 0
============ Files ==============
I00000.png
I00001.png
============ Folders ==============
Press any key to continue . . .


======================================================
(2) dir_fnames function
======================================================
This function is a convenient wrapper over dir_ff so that files with multiple specific extensions can be looked for. Moreover, it will return many useful other things such as fullpaths and file names with extensions. This will very useful in reading/saving batches of files including images. Therefore, this function has the same functionality as a matlab function "dir_cell" that I've written before.

Get the filenames with specific extensions, their full paths, file names with extensions removed in a given directory. 

Usage example:

Imagine that in a given folder "C:/Users/Kyaw/Desktop/test_fold", there are the following 2 image files, 3 text files, 1 cpp file and 1 folder as follows:

cpan
DISTRIBUTIONS.txt
I00000.png
I00001.png
image processing in opencv.cpp
README.txt
relocation.txt

The following code can be written:

string dir_given = "C:/Users/Kyaw/Desktop/test_fold/";
//string dir_given = "C:\\Users\\Kyaw\\Desktop\\test_fold\\"; 
std::vector<std::string> str_exts;
str_exts.push_back("*.png"); //png files
str_exts.push_back("*.txt"); // txt files
std::vector<std::string> fnames;
std::vector<std::string> fnames_fullpath;
std::vector<std::string> fnames_noext;
dir_fnames(dir_given, str_exts, fnames_fullpath, fnames, fnames_noext);

for (int i = 0; i < fnames.size(); i++)
{
	cout << fnames[i] << endl;
	cout << fnames_fullpath[i] << endl;
	cout << fnames_noext[i] << endl;
	cout << "====================" << endl;
}

==================

This will output:

I00000.png
C:/Users/Kyaw/Desktop/test_fold/I00000.png
I00000
====================
I00001.png
C:/Users/Kyaw/Desktop/test_fold/I00001.png
I00001
====================
DISTRIBUTIONS.txt
C:/Users/Kyaw/Desktop/test_fold/DISTRIBUTIONS.txt
DISTRIBUTIONS
====================
README.txt
C:/Users/Kyaw/Desktop/test_fold/README.txt
README
====================
relocation.txt
C:/Users/Kyaw/Desktop/test_fold/relocation.txt
relocation
====================
Press any key to continue . . .


The following code:

string dir_given = "C:/Users/Kyaw/Desktop/test_fold/";
//string dir_given = "C:\\Users\\Kyaw\\Desktop\\test_fold\\"; 
std::vector<std::string> str_exts;
str_exts.push_back("*.png");
std::vector<std::string> fnames;
std::vector<std::string> fnames_fullpath;
std::vector<std::string> fnames_noext;
dir_fnames(dir_given, str_exts, fnames_fullpath, fnames, fnames_noext);

for (int i = 0; i < fnames.size(); i++)
{
	cout << fnames[i] << endl;
	cout << fnames_fullpath[i] << endl;
	cout << fnames_noext[i] << endl;
	cout << "====================" << endl;
}

=========================

... will output (since only looking for png files:

I00000.png
C:/Users/Kyaw/Desktop/test_fold/I00000.png
I00000
====================
I00001.png
C:/Users/Kyaw/Desktop/test_fold/I00001.png
I00001
====================
Press any key to continue . . .


The following code:

string dir_given = "C:/Users/Kyaw/Desktop/test_fold/";
//string dir_given = "C:\\Users\\Kyaw\\Desktop\\test_fold\\"; 
std::vector<std::string> str_exts;
str_exts.push_back("*.png");
std::vector<std::string> fnames;
std::vector<std::string> fnames_fullpath;
std::vector<std::string> fnames_noext;
dir_fnames(dir_given, str_exts, fnames_fullpath, fnames, fnames_noext);

for (int i = 0; i < fnames.size(); i++)
{
	cout << fnames[i] << endl;
	cout << fnames_fullpath[i] << endl;
	cout << fnames_noext[i] << endl;
	cout << "====================" << endl;
}

=========================

... will output:

I00000.png
C:/Users/Kyaw/Desktop/test_fold/I00000.png
I00000
====================
I00001.png
C:/Users/Kyaw/Desktop/test_fold/I00001.png
I00001
====================
DISTRIBUTIONS.txt
C:/Users/Kyaw/Desktop/test_fold/DISTRIBUTIONS.txt
DISTRIBUTIONS
====================
README.txt
C:/Users/Kyaw/Desktop/test_fold/README.txt
README
====================
relocation.txt
C:/Users/Kyaw/Desktop/test_fold/relocation.txt
relocation
====================
image processing in opencv.cpp
C:/Users/Kyaw/Desktop/test_fold/image processing in opencv.cpp
image processing in opencv
====================
Press any key to continue . . .


*/


void dir_ff(std::string search_path, std::vector<std::string> &files, std::vector<std::string> &folders)
{
	
	files.clear();
	folders.clear();

	//setup wstring to string converter
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;

	std::wstring search_path_w = converter.from_bytes(search_path);
	std::wstring str_temp;
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path_w.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				files.push_back(converter.to_bytes(fd.cFileName));			
			else
				folders.push_back(converter.to_bytes(fd.cFileName));

		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
}


void dir_fnames(std::string dir_given, std::vector<std::string> str_exts, std::vector<std::string> &fnames_fullpath, std::vector<std::string> &fnames, std::vector<std::string> &fnames_noExt)
{	

	fnames.clear();
	fnames_fullpath.clear();
	fnames_noExt.clear();	

	for(int i=0; i<str_exts.size(); i++)
	{		
		std::string search_path_cur = dir_given + str_exts[i];
		std::vector<std::string> fnames_cur;
		std::vector<std::string> folders; // don't care
		dir_ff(search_path_cur, fnames_cur, folders);
		
		std::vector<std::string>fnames_fullpath_cur = fnames_cur;
		std::vector<std::string>fnames_noExt_cur = fnames_cur;
		
		for(int j=0; j<fnames_cur.size(); j++)
		{
			fnames_fullpath_cur[j] = dir_given + fnames_fullpath_cur[j];
			fnames_noExt_cur[j] = fnames_noExt_cur[j].substr(0, fnames_noExt_cur[j].size()-str_exts[i].size()+1);
		}		
		
		fnames.insert(fnames.end(), fnames_cur.begin(), fnames_cur.end());
		fnames_fullpath.insert(fnames_fullpath.end(), fnames_fullpath_cur.begin(), fnames_fullpath_cur.end());
		fnames_noExt.insert(fnames_noExt.end(), fnames_noExt_cur.begin(), fnames_noExt_cur.end());
		
	}

	
	
		
}


// just some convenient overloads if the user wants only certain outputs

// if user wants only fnames_fullpath and fnames outputs
void dir_fnames(std::string dir_given, std::vector<std::string> str_exts, std::vector<std::string> &fnames_fullpath, std::vector<std::string> &fnames)
{
	std::vector<std::string> fnames_noExt; // will ignore this
	dir_fnames(dir_given, str_exts, fnames_fullpath, fnames, fnames_noExt);
}

// if user wants only fnames_fullpath output
void dir_fnames(std::string dir_given, std::vector<std::string> str_exts, std::vector<std::string> &fnames_fullpath)
{
	std::vector<std::string> fnames; // will ignore this
	std::vector<std::string> fnames_noExt; // will ignore this
	dir_fnames(dir_given, str_exts, fnames_fullpath, fnames, fnames_noExt);
}

// convenient overloads if the user wants only a single extension (a string input)

// note the std::string input type for str_ext
void dir_fnames(std::string dir_given, std::string str_ext, std::vector<std::string> &fnames_fullpath, std::vector<std::string> &fnames, std::vector<std::string> &fnames_noExt)
{
	std::vector<std::string>str_exts; str_exts.push_back(str_ext); // make a vector of 1 elmement
	dir_fnames(dir_given, str_exts, fnames_fullpath, fnames, fnames_noExt);
}

// note the std::string input type for str_ext
void dir_fnames(std::string dir_given, std::string str_ext, std::vector<std::string> &fnames_fullpath, std::vector<std::string> &fnames)
{
	std::vector<std::string>str_exts; str_exts.push_back(str_ext); // make a vector of 1 elmement
	std::vector<std::string> fnames_noExt; // will ignore this
	dir_fnames(dir_given, str_exts, fnames_fullpath, fnames, fnames_noExt);
}

// note the std::string input type for str_ext
void dir_fnames(std::string dir_given, std::string str_ext, std::vector<std::string> &fnames_fullpath)
{
	std::vector<std::string>str_exts; str_exts.push_back(str_ext); // make a vector of 1 elmement
	std::vector<std::string> fnames; // will ignore this
	std::vector<std::string> fnames_noExt; // will ignore this
	dir_fnames(dir_given, str_exts, fnames_fullpath, fnames, fnames_noExt);
}


#endif