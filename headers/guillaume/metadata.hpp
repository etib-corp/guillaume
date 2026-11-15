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

#pragma once

#include <string>

namespace guillaume
{

	/**
	 * @brief The identity card of an application: its name, version,
	 * author and similar descriptive strings.
	 *
	 * A plain record of seven free-form strings, meant to be filled by the
	 * program and handed to whatever needs to show or report them (a window
	 * title, an about box, a platform layer). The class does not interpret
	 * or validate them, and the framework does not read them: Application
	 * holds no Metadata.
	 *
	 * @code
	 * guillaume::Metadata metadata("Notes", "1.4.0", "com.example.notes",
	 * 	"Example Corp", "Copyright (c) 2026 Example Corp",
	 * 	"https://example.com/notes", "application");
	 * metadata.setVersion("1.4.1");
	 * @endcode
	 *
	 * @note The getters return a mutable reference to the stored string, so
	 * they cannot be called on a const Metadata, and writing through the
	 * reference changes the record like the setter does.
	 */
	class Metadata
	{
		private:
		std::string _name;			///< Name shown to the user
		std::string _version;		///< Version string, such as "1.4.0"
		std::string _identifier;	///< Unique identifier, such as a
									///< reverse-domain name
		std::string _creator;		///< Author or publisher
		std::string _copyright;		///< Copyright notice
		std::string _url;			///< Home page of the application
		std::string _type;			///< Kind of application, free-form

		public:
		/**
		 * @brief Create a record in which every string is empty.
		 */
		Metadata(void) = default;

		/**
		 * @brief Create a record with every string given.
		 * @param name The name shown to the user.
		 * @param version The version string, such as "1.4.0".
		 * @param identifier A unique identifier, such as the reverse-domain
		 * name "com.example.notes".
		 * @param creator The author or publisher.
		 * @param copyright The copyright notice.
		 * @param url The home page of the application.
		 * @param type The kind of application; free-form.
		 */
		Metadata(const std::string &name, const std::string &version,
				 const std::string &identifier, const std::string &creator,
				 const std::string &copyright, const std::string &url,
				 const std::string &type);

		/**
		 * @brief Replace the name shown to the user.
		 * @param name The new name.
		 */
		void setName(const std::string &name);

		/**
		 * @brief Get the name shown to the user.
		 * @return Mutable reference to the stored name; empty when unset.
		 */
		std::string &getName(void);

		/**
		 * @brief Replace the version string.
		 * @param version The new version, such as "1.4.0". No format is
		 * imposed.
		 */
		void setVersion(const std::string &version);

		/**
		 * @brief Get the version string.
		 * @return Mutable reference to the stored version; empty when
		 * unset.
		 */
		std::string &getVersion(void);

		/**
		 * @brief Replace the unique identifier of the application.
		 * @param identifier The new identifier, such as the reverse-domain
		 * name "com.example.notes".
		 */
		void setIdentifier(const std::string &identifier);

		/**
		 * @brief Get the unique identifier of the application.
		 * @return Mutable reference to the stored identifier; empty when
		 * unset.
		 */
		std::string &getIdentifier(void);

		/**
		 * @brief Replace the author or publisher.
		 * @param creator The new author or publisher name.
		 */
		void setCreator(const std::string &creator);

		/**
		 * @brief Get the author or publisher.
		 * @return Mutable reference to the stored creator; empty when
		 * unset.
		 */
		std::string &getCreator(void);

		/**
		 * @brief Replace the copyright notice.
		 * @param copyright The new notice, such as
		 * "Copyright (c) 2026 Example Corp".
		 */
		void setCopyright(const std::string &copyright);

		/**
		 * @brief Get the copyright notice.
		 * @return Mutable reference to the stored notice; empty when unset.
		 */
		std::string &getCopyright(void);

		/**
		 * @brief Replace the home page of the application.
		 * @param url The new address. It is stored as given, not checked.
		 */
		void setUrl(const std::string &url);

		/**
		 * @brief Get the home page of the application.
		 * @return Mutable reference to the stored address; empty when
		 * unset.
		 */
		std::string &getUrl(void);

		/**
		 * @brief Replace the kind of application.
		 * @param type The new kind; free-form, such as "application" or
		 * "game".
		 */
		void setType(const std::string &type);

		/**
		 * @brief Get the kind of application.
		 * @return Mutable reference to the stored kind; empty when unset.
		 */
		std::string &getType(void);
	};

}	 // namespace guillaume
