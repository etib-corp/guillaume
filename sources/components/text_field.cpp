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

#include "guillaume/components/text_field.hpp"

namespace guillaume::components
{
	const std::string &TextField::getPlaceholder(void) const
	{
		return _placeholder;
	}

	TextField &TextField::setPlaceholder(const std::string &placeholder)
	{
		if (_placeholder == placeholder) {
			return *this;
		}
		_placeholder = placeholder;
		setHasChanged(true);
		return *this;
	}

	const std::string &TextField::getLabel(void) const
	{
		return _label;
	}

	TextField &TextField::setLabel(const std::string &label)
	{
		if (_label == label) {
			return *this;
		}
		_label = label;
		setHasChanged(true);
		return *this;
	}

	const std::string &TextField::getSupportingText(void) const
	{
		return _supportingText;
	}

	TextField &TextField::setSupportingText(const std::string &supportingText)
	{
		if (_supportingText == supportingText) {
			return *this;
		}
		_supportingText = supportingText;
		setHasChanged(true);
		return *this;
	}

	const std::string &TextField::getPrefix(void) const
	{
		return _prefix;
	}

	TextField &TextField::setPrefix(const std::string &prefix)
	{
		if (_prefix == prefix) {
			return *this;
		}
		_prefix = prefix;
		setHasChanged(true);
		return *this;
	}

	const std::string &TextField::getSuffix(void) const
	{
		return _suffix;
	}

	TextField &TextField::setSuffix(const std::string &suffix)
	{
		if (_suffix == suffix) {
			return *this;
		}
		_suffix = suffix;
		setHasChanged(true);
		return *this;
	}

	const std::string &TextField::getErrorText(void) const
	{
		return _errorText;
	}

	TextField &TextField::setErrorText(const std::string &errorText)
	{
		if (_errorText == errorText) {
			return *this;
		}
		_errorText = errorText;
		setHasChanged(true);
		return *this;
	}

	bool TextField::isError(void) const
	{
		return _error;
	}

	TextField &TextField::setError(bool error)
	{
		if (_error == error) {
			return *this;
		}
		_error = error;
		setHasChanged(true);
		return *this;
	}

	bool TextField::isLabelFloating(void) const
	{
		return _labelFloating;
	}

	TextField &TextField::setLabelFloating(bool floating)
	{
		if (_labelFloating == floating) {
			return *this;
		}
		_labelFloating = floating;
		setHasChanged(true);
		return *this;
	}
}	 // namespace guillaume::components
