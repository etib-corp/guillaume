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

namespace guillaume {

/**
 * @brief Class for storing application metadata information.
 *
 * This class stores various metadata properties related to the application.
 */
class Metadata {
  private:
    std::string _name;       ///< Application name
    std::string _version;    ///< Application version
    std::string _identifier; ///< Application identifier
    std::string _creator;    ///< Application creator
    std::string _copyright;  ///< Application copyright information
    std::string _url;        ///< Application URL
    std::string _type;       ///< Application type

  public:
    /**
     * @brief Default constructor
     */
    Metadata(void) = default;

    /**
     * @brief Constructor with all metadata parameters
     * @param name Application name
     * @param version Application version
     * @param identifier Application identifier
     * @param creator Application creator
     * @param copyright Application copyright information
     * @param url Application URL
     * @param type Application type
     */
    Metadata(const std::string &name, const std::string &version,
             const std::string &identifier, const std::string &creator,
             const std::string &copyright, const std::string &url,
             const std::string &type)
        : _name(name), _version(version), _identifier(identifier),
          _creator(creator), _copyright(copyright), _url(url), _type(type) {}

    /**
     * @brief Set the application name.
     * @param name The name to set.
     */
    void setName(const std::string &name) { _name = name; }

    /**
     * @brief Get the application name.
     * @return The application name.
     */
    std::string &getName(void) { return _name; }

    /**
     * @brief Set the application version.
     * @param version The version to set.
     */
    void setVersion(const std::string &version) { _version = version; }

    /**
     * @brief Get the application version.
     * @return The application version.
     */
    std::string &getVersion(void) { return _version; }

    /**
     * @brief Set the application identifier.
     * @param identifier The identifier to set.
     */
    void setIdentifier(const std::string &identifier) {
        _identifier = identifier;
    };

    /**
     * @brief Get the application identifier.
     * @return The application identifier.
     */
    std::string &getIdentifier(void) { return _identifier; }

    /**
     * @brief Set the application creator.
     * @param creator The creator to set.
     */
    void setCreator(const std::string &creator) { _creator = creator; }

    /**
     * @brief Get the application creator.
     * @return The application creator.
     */
    std::string &getCreator(void) { return _creator; }

    /**
     * @brief Set the application copyright information.
     * @param copyright The copyright information to set.
     */
    void setCopyright(const std::string &copyright) { _copyright = copyright; }

    /**
     * @brief Get the application copyright information.
     * @return The application copyright information.
     */
    std::string &getCopyright(void) { return _copyright; }

    /**
     * @brief Set the application URL.
     * @param url The URL to set.
     */
    void setUrl(const std::string &url) { _url = url; }

    /**
     * @brief Get the application URL.
     * @return The application URL.
     */
    std::string &getUrl(void) { return _url; }

    /**
     * @brief Set the application type.
     * @param type The type to set.
     */
    void setType(const std::string &type) { _type = type; }

    /**
     * @brief Get the application type.
     * @return The application type.
     */
    std::string &getType(void) { return _type; }
};

} // namespace guillaume
