/*
 Copyright (c) 2026 ETIB Corporation

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include "guillaume/metadata.hpp"

namespace guillaume
{
	Metadata::Metadata(const std::string &name, const std::string &version,
					   const std::string &identifier,
					   const std::string &creator,
					   const std::string &copyright,
					   const std::string &url,
					   const std::string &type)
		: _name(name)
		, _version(version)
		, _identifier(identifier)
		, _creator(creator)
		, _copyright(copyright)
		, _url(url)
		, _type(type)
	{
	}

	void Metadata::setName(const std::string &name)
	{
		_name = name;
	}

	std::string &Metadata::getName(void)
	{
		return _name;
	}

	void Metadata::setVersion(const std::string &version)
	{
		_version = version;
	}

	std::string &Metadata::getVersion(void)
	{
		return _version;
	}

	void Metadata::setIdentifier(const std::string &identifier)
	{
		_identifier = identifier;
	}

	std::string &Metadata::getIdentifier(void)
	{
		return _identifier;
	}

	void Metadata::setCreator(const std::string &creator)
	{
		_creator = creator;
	}

	std::string &Metadata::getCreator(void)
	{
		return _creator;
	}

	void Metadata::setCopyright(const std::string &copyright)
	{
		_copyright = copyright;
	}

	std::string &Metadata::getCopyright(void)
	{
		return _copyright;
	}

	void Metadata::setUrl(const std::string &url)
	{
		_url = url;
	}

	std::string &Metadata::getUrl(void)
	{
		return _url;
	}

	void Metadata::setType(const std::string &type)
	{
		_type = type;
	}

	std::string &Metadata::getType(void)
	{
		return _type;
	}
}	 // namespace guillaume
