/************************************************************************
 * This file is part of the AREG SDK core engine.
 * AREG SDK is dual-licensed under Free open source (Apache version 2.0
 * License) and Commercial (with various pricing models) licenses, depending
 * on the nature of the project (commercial, research, academic or free).
 * You should have received a copy of the AREG SDK license description in LICENSE.txt.
 * If not, please contact to info[at]aregtech.com
 *
 * \copyright   (c) 2017-2022 Aregtech UG. All rights reserved.
 * \file        areg/base/private/FileBase.cpp
 * \ingroup     AREG SDK, Asynchronous Event Generator Software Development Kit 
 * \author      Artak Avetyan
 * \brief       Base class of file objects.
 *
 ************************************************************************/

#include "areg/base/FileBase.hpp"
#include "areg/base/NEUtilities.hpp"
#include "areg/base/WideString.hpp"
#include "areg/base/IEByteBuffer.hpp"
#include "areg/base/DateTime.hpp"
#include "areg/base/Process.hpp"

//////////////////////////////////////////////////////////////////////////
// Local methods.
//////////////////////////////////////////////////////////////////////////
namespace {

template<typename CharType, class ClassType>
inline int _readString(const FileBase & file, ClassType & outValue)
{
    constexpr unsigned int maxBufSize = 1024;

    unsigned int result = 0;
    outValue.clear();
    if ((file.isOpened() == false) || (file.canRead() == false))
    {
        OUTPUT_ERR("Either file is not opened or forbidden to read data. Data cannot be read.");
        return static_cast<int>(result);
    } 

    CharType * buffer = DEBUG_NEW CharType[maxBufSize];
    if (buffer != nullptr)
    {
        unsigned int strLength  = maxBufSize - 1;
        CharType * context      = nullptr;
        unsigned int length     = 0;
        do 
        {
            buffer[0]               = NEString::EndOfString;
            unsigned int oldPos     = file.getPosition();
            unsigned int readLength = file.read(reinterpret_cast<unsigned char *>(buffer), strLength * sizeof(CharType)) / sizeof(CharType);
            readLength              = MACRO_MIN(strLength, readLength);
            buffer[readLength]      = NEString::EndOfString;

            length = readLength;
            if ( readLength != 0 )
            {
                const CharType * str = NEString::getPrintable<CharType>( buffer, static_cast<NEString::CharCount>(readLength), &context );
                length = context != nullptr ? MACRO_ELEM_COUNT( buffer, context ) : readLength;

                outValue    += str;
                result      += length;
                int newPos   = static_cast<int>(result * sizeof(CharType)) + static_cast<int>(oldPos);
                file.setPosition(newPos, IECursorPosition::eCursorPosition::PositionBegin);
                if ( context != (buffer + readLength))
                    length = 0; // break loop
            }
        } while ( length != 0 );

        delete [] buffer;
    }

    return static_cast<int>(result);
}

template<typename CharType, class ClassType>
inline int _readLine(const FileBase & file, ClassType & outValue)
{
    constexpr unsigned int maxBufSize = 1024;

    unsigned int result = 0;
    outValue.clear();
    if ((file.isOpened() == false) || (file.canRead() == false))
    {
        OUTPUT_ERR("Either file is not opened or forbidden to read data. Data cannot be read.");
        return static_cast<int>(result);
    } 

    CharType * buffer = DEBUG_NEW CharType[maxBufSize];
    if (buffer != nullptr)
    {
        unsigned int strLength  = maxBufSize - 1;
        CharType * context      = nullptr;
        unsigned int length     = 0;
        do 
        {
            buffer[0]               = NEString::EndOfString;
            unsigned int oldPos     = file.getPosition();
            unsigned int readLength = file.read(reinterpret_cast<unsigned char *>(buffer), strLength * sizeof(CharType)) / sizeof(CharType);
            readLength              = MACRO_MIN(strLength, readLength);
            buffer[readLength]      = NEString::EndOfString;

            length = readLength;
            if ( readLength != 0 )
            {
                readLength = NEString::removeChar<CharType>(static_cast<CharType>('\r'), buffer, static_cast<NEString::CharCount>(readLength), true);
                const CharType * str = NEString::getLine<CharType>( buffer, static_cast<NEString::CharCount>(readLength), &context );
                length   = context != nullptr ? MACRO_ELEM_COUNT(buffer, context) : readLength;
                outValue+= str;
                result  += length;
                int newPos  = static_cast<int>( (result * sizeof(CharType)) + oldPos );
                file.setPosition(newPos, IECursorPosition::eCursorPosition::PositionBegin);
                if ( context != (buffer + readLength))
                    length = 0; // break loop
            }
        } while ( length > 0 );

        delete [] buffer;
    }

    return static_cast<int>(result);
}

template<typename CharType>
inline int _readString(const FileBase & file, CharType * buffer, int charCount)
{
    unsigned int result = 0;
    if ((file.isOpened() == false) || (file.canRead() == false))
    {
        OUTPUT_ERR("Either file is not opened or forbidden to read data. Data cannot be read.");
    } 
    else if ((buffer != nullptr) && (charCount > 0))
    {
        unsigned int strLength  = static_cast<unsigned int>(charCount) - 1;
        buffer[0]               = NEString::EndOfString;
        unsigned int oldPos     = file.getPosition();
        CharType * context      = nullptr;
        unsigned int readLength = file.read(reinterpret_cast<unsigned char *>(buffer), strLength * sizeof(CharType)) / sizeof(CharType);
        readLength              = MACRO_MIN(strLength, readLength);
        buffer[readLength]      = NEString::EndOfString;

        if ( readLength > 0 )
        {
            NEString::getPrintable<CharType>( buffer, charCount, &context );
            ASSERT((context == nullptr) || (context > buffer));
            result = context != nullptr ? MACRO_ELEM_COUNT( buffer, context ) : readLength;
            int newPos = static_cast<int>( (result * sizeof(CharType)) + oldPos );
            file.setPosition(newPos, IECursorPosition::eCursorPosition::PositionBegin);
        }
    }
    else
    {
        OUTPUT_ERR("The required buffer to fill is empty, cannot read string");
    }

    return static_cast<int>(result);
}

template<typename CharType>
inline int _readLine(const FileBase & file, CharType * buffer, int charCount)
{
    unsigned int result = 0;
    if ((file.isOpened() == false) || (file.canRead() == false))
    {
        OUTPUT_ERR("Either file is not opened or forbidden to read data. Data cannot be read.");
    } 
    else if ((buffer != nullptr) && (charCount > 0))
    {
        unsigned int strLength  = static_cast<unsigned int>(charCount - 1);
        buffer[0]               = NEString::EndOfString;
        unsigned int oldPos     = file.getPosition();
        CharType * context      = nullptr;
        unsigned int readLength = file.read(reinterpret_cast<unsigned char *>(buffer), strLength * sizeof(CharType)) / sizeof(CharType);
        readLength              = MACRO_MIN(strLength, readLength);
        buffer[readLength]      = NEString::EndOfString;
        if ( readLength != 0 )
        {
            NEString::getLine<CharType>(buffer, charCount, &context);
            ASSERT((context == nullptr) || (context > buffer));
            result = context != nullptr ? MACRO_ELEM_COUNT(buffer, context) : readLength;
            int newPos = static_cast<int>( (result * sizeof(CharType)) + oldPos );
            file.setPosition(newPos, IECursorPosition::eCursorPosition::PositionBegin);
        }
    }
    else
    {
        OUTPUT_ERR("The required buffer to fill is empty, cannot read string");
    }

    return static_cast<int>(result);
}

template<typename CharType>
inline bool _writeString(FileBase & file, const CharType * buffer, int strLen = static_cast<int>(NEString::COUNT_ALL) )
{
    bool result = false;
    if (file.isOpened() && file.canWrite())
    {
        if (buffer != nullptr)
        {
            unsigned int len = strLen >= 0 ? static_cast<unsigned int>(strLen) : static_cast<unsigned int>(NEString::getStringLength<CharType>( buffer ));
            len += file.isBinaryMode() ? 1 : 0;
            len *= sizeof(CharType);

            result = (file.write( reinterpret_cast<const unsigned char *>(buffer), len ) == len);
        }
        else
        {
            OUTPUT_ERR("The buffer is nullptr, nothing to write.");
        }
    }
    else
    {
        OUTPUT_ERR("Either file is not opened or data cannot be written.");
    }

    return result;
}

template<typename CharType>
inline  bool _writeLine(FileBase & file, const CharType * buffer)
{
    bool result = false;
    if (file.isOpened() && file.canWrite())
    {
        if (buffer != nullptr)
        {
            unsigned int len = static_cast<unsigned int>(NEString::getStringLineLength<CharType>(buffer));
            len *= sizeof(CharType);
            if ( file.write(reinterpret_cast<const unsigned char *>(buffer), len) == len )
                result = file.writeChar( TEString<CharType>::NewLine );
        }
        else
        {
            OUTPUT_ERR("The buffer is nullptr, nothing to write.");
        }
    }
    else
    {
        OUTPUT_ERR("Either file is not opened or data cannot be written.");
    }

    return result;
}

} // namespace

//////////////////////////////////////////////////////////////////////////
// FileBase class implementation.
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Constructor / Destructor
//////////////////////////////////////////////////////////////////////////
FileBase::FileBase( void )
    : IEIOStream        ( )
    , IECursorPosition  ( )

    , mFileName         (String::getEmptyString())
    , mFileMode         (static_cast<unsigned int>(FO_MODE_INVALID))
    , mReadConvert      (static_cast<IEInStream &>(self()), static_cast<IECursorPosition &>(self()) )
    , mWriteConvert     (static_cast<IEOutStream &>(self()), static_cast<IECursorPosition &>(self()) )
{
}

//////////////////////////////////////////////////////////////////////////
// Methods
//////////////////////////////////////////////////////////////////////////

unsigned int FileBase::normalizeMode(unsigned int mode) const
{
    if (mode != FO_MODE_INVALID)
        mode |= FO_MODE_READ;

    if (mode & FOB_ATTACH)
    {
        mode &= ~(FOB_DETACH | FOB_TRUNCATE | FOB_TEMP_FILE | FOB_SHARE_WRITE | FOB_WRITE);
        mode |= FO_MODE_ATTACH;
    }

    if (mode & FOB_DETACH)
    {
        mode &= ~(FOB_ATTACH | FOB_TEMP_FILE | FOB_SHARE_WRITE);
        mode |= FO_MODE_DETACH;
    }

    if (mode & FOB_TEMP_FILE)
    {
        mode &= ~(FOB_FOR_DELETE | FOB_EXIST | FOB_ATTACH | FOB_DETACH | FOB_SHARE_READ | FOB_SHARE_WRITE);
        mode |= FO_MODE_CREATE_TEMP;
    }

    if (mode & FOB_TEXT)
    {
        mode &= ~FOB_BINARY;
        mode |= FO_MODE_TEXT;
    }
    else
    {
        mode |= FO_MODE_BINARY;
    }

    if (mode & FOB_BINARY)
    {
        mode &= ~FOB_TEXT;
        mode |= FO_MODE_BINARY;
    }

    if ((mode & FOB_WRITE) == 0 && (mode & FOB_READ) != 0)
        mode |= FOB_EXIST;
    else if (mode & FOB_CREATE)
        mode |= FO_MODE_CREATE;

    if (mode & FOB_EXIST)
        mode |= FO_MODE_EXIST;
    else
        mode &= ~FOB_TRUNCATE;

    if (mode & FOB_TRUNCATE)
        mode |= FO_MODE_TRUNCATE;

    if (mode & FOB_WRITE_DIRECT)
        mode |= FO_MODE_WRITE_DIRECT;

    return mode;
}

int FileBase::append( const unsigned char* buffer, unsigned int length )
{
    unsigned int result = 0;
    if ((length != 0) && (buffer != nullptr) && isOpened() && canWrite())
    {
        moveToEnd();
        result = write(buffer, length);
    }

    return static_cast<int>(result);
}

int FileBase::readInvert( unsigned char * buffer, unsigned int length ) const
{
    unsigned int result = 0;
    if ((length != 0) && (buffer != nullptr) && isOpened() && canRead())
    {
        result = read(buffer, length);
        unsigned int count = result / 2;
        for (unsigned int i = 0; i < count; ++ i)
        {
            unsigned char ch = buffer[i];
            buffer[i] = buffer[length - 1 - i];
            buffer[length - 1 - i] = ch;
        }
    }

    return static_cast<int>(result);
}

int FileBase::writeInvert( const unsigned char * buffer, int unsigned length )
{
    unsigned int result = 0;
    if ((length != 0) && (buffer != nullptr) && isOpened() && canWrite())
    {
        unsigned char* temp = (buffer != nullptr) && (length > 0) ? DEBUG_NEW unsigned char[length] : nullptr;
        if (temp != nullptr )
        {
            for (unsigned int i = 0; i < length; ++ i)
                temp[i] = buffer[length - 1 - i];
            
            result = write(temp, length);
            delete [] temp;
        }
    }

    return static_cast<int>(result);
}

int FileBase::readString( char * IN OUT buffer, int IN charCount) const
{
    return _readString<char>(self(), buffer, charCount);
}

int FileBase::readString( wchar_t * IN OUT buffer, int IN charCount ) const
{
    return _readString<wchar_t>(self(), buffer, charCount);
}

int FileBase::readString(String & OUT outValue ) const
{
    return _readString<char, String>(self(), outValue);
}

int FileBase::readString(WideString & OUT outValue) const
{
    return _readString<char, WideString>(self(), outValue);
}

int FileBase::readLine( char * IN OUT buffer, int IN charCount) const
{
    return _readLine<char>(self(), buffer, charCount);
}

int FileBase::readLine( wchar_t * IN OUT buffer, int IN charCount ) const
{
    return _readLine<wchar_t>(self(), buffer, charCount);
}

int FileBase::readLine( String & OUT outBuffer) const
{
    return _readLine<char, String>(self(), outBuffer);
}

int FileBase::readLine(WideString & OUT outBuffer) const
{
    return _readLine<wchar_t, WideString>(self(), outBuffer);
}

bool FileBase::writeString( const char* buffer )
{
    return _writeString<char>(self(), buffer, -1);
}

bool FileBase::writeString( const wchar_t* buffer)
{
    return _writeString<wchar_t>(self(), buffer, -1);
}

bool FileBase::writeString(const String& buffer)
{
    return _writeString<char>(self(), buffer.getString(), static_cast<int>(buffer.getLength()));
}

bool FileBase::writeString(const WideString& buffer)
{
    return _writeString<wchar_t>(self(), buffer.getString(), static_cast<int>(buffer.getLength()));
}

bool FileBase::writeLine( const char* buffer)
{
    return _writeLine<char>(self(), buffer);
}

bool FileBase::writeLine( const wchar_t* buffer)
{
    return _writeLine<wchar_t>(self(), buffer);
}

bool FileBase::writeLine(const String& buffer)
{
    return _writeLine<char>(self(), buffer);
}

bool FileBase::writeLine(const WideString& buffer)
{
    return _writeLine<wchar_t>(self(), buffer);
}

unsigned int FileBase::resizeAndFill(unsigned int newSize, unsigned char fillValue )
{
    unsigned int curPos = getPosition();
    unsigned int result = curPos;

    if (newSize > 0)
    {
        unsigned int newPos = reserve(newSize);
        if ((newPos != IECursorPosition::INVALID_CURSOR_POSITION) && (newPos > curPos))
        {
            setPosition(static_cast<int>(curPos), IECursorPosition::eCursorPosition::PositionBegin);
            for (unsigned int i = 0; i < newPos; ++ i)
            {
                write( &fillValue, sizeof( unsigned char ) );
            }

            ASSERT(getPosition() == newPos);
            result = newPos;
        }
    }

    return result;
}

void FileBase::resetCursor(void) const
{
    setPosition(0, IECursorPosition::eCursorPosition::PositionBegin);
}

unsigned int FileBase::read(IEByteBuffer & buffer) const
{
    unsigned int result = 0;
    buffer.invalidate();

    if ( isOpened() && canRead() )
    {
        signed   int sizeReserve= 0;
        unsigned int sizeRead   = 0;

        if (readInt(sizeReserve) && (sizeReserve > 0))
        {
            sizeRead = buffer.reserve(static_cast<unsigned int>(sizeReserve), false);
            unsigned char * data = sizeRead != 0 ? buffer.getBuffer() : nullptr;
            if ( (data != nullptr) && (read(data, sizeRead) == sizeRead) )
            {
                result = sizeRead + sizeof(int);
            }
            else
            {
                OUTPUT_ERR("Either was not able to reserve [ %d ] bytes of space, or failed read file [ %s ].", sizeReserve, mFileName.getString());
            }
        }
        else
        {
            OUTPUT_ERR("Unable to read the size of byte-buffer, the file [ %s ]", mFileName.getString());
        }
    }
    else
    {
        OUTPUT_ERR("Either file [ %s ] is not opened [ %s ], or reading mode is not set (mode = [ %d ]).", mFileName.getString(), isOpened() ? "true" : "false", mFileMode);
    }

    return result;
}

unsigned int FileBase::read(String & asciiString) const
{
    return static_cast<unsigned int>(readString(asciiString));
}

unsigned int FileBase::read(WideString & wideString) const
{
    return static_cast<unsigned int>(readString(wideString));
}

unsigned int FileBase::write(const IEByteBuffer & buffer)
{
    unsigned int result = 0;

    if ( isOpened() && canWrite() )
    {
        const unsigned char * data  = buffer.getBuffer();
        unsigned int sizeUsed       = buffer.getSizeUsed();

        if (writeInt(static_cast<int>(sizeUsed)) && (write(data, sizeUsed) == sizeUsed))
        {
            result = sizeUsed + sizeof(int);
        }
        else
        {
            OUTPUT_ERR("Failed to write [ %u ] bytes of data in file [ %s ]", sizeUsed + sizeof(int), mFileName.getString());
        }
    }
    else
    {
        OUTPUT_ERR("Either files [ %s ] is not opened or it is not opened to write file", mFileName.getString());
    }

    return result;
}

unsigned int FileBase::write(const String & asciiString)
{
    const char * buffer = asciiString.getString();
    unsigned int space  = isTextMode() != 0 ? asciiString.getLength() * sizeof(char) : asciiString.getSpace();

    return write(reinterpret_cast<const unsigned char *>(buffer), space);
}

unsigned int FileBase::write(const WideString & wideString)
{
    const wchar_t * buffer  = wideString.getString();
    unsigned int space      = isTextMode() != 0 ? wideString.getLength() * sizeof(wchar_t) : wideString.getSpace();

    return write(reinterpret_cast<const unsigned char *>(buffer), space);
}

bool FileBase::write(const char * asciiString)
{
    return writeString(asciiString);
}

bool FileBase::write(const wchar_t * wideString)
{
    return writeString(wideString);
}

void FileBase::flush(void)
{
    ; // do nothing
}

void FileBase::normalizeName(String & IN OUT name)
{
    // replace all "%time%"
    char fmt[32];
    NEUtilities::sSystemTime st;
    DateTime::getNow(st, true);
    String::formatString(fmt, 32, FileBase::TIMESTAMP_FORMAT.data(), st.stYear, st.stMonth, st.stDay, st.stHour, st.stMinute, st.stSecond, st.stMillisecs);
    name.replace(FileBase::FILE_MASK_TIMESTAMP, fmt, NEString::START_POS, true);

    // replace all "%appname%"
    const String & appName = Process::getInstance().getAppName();
    name.replace(FileBase::FILE_MASK_APPNAME, appName, NEString::START_POS, true);
}
