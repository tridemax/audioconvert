
QT       -= core gui

TARGET = audioconvert

TEMPLATE = lib
CONFIG += c++14

OBJECTS_DIR = ./
DESTDIR = $$_PRO_FILE_PWD_/../_dist/lib

macx {
	QMAKE_CXXFLAGS += \
		-stdlib=libc++ \
		-ferror-limit=2000

} else {
}

QMAKE_CXXFLAGS_WARN_ON += \
	-Wno-parentheses \
	-Wno-switch \
	-Wno-unused-parameter \
	-Wno-unused-variable

QMAKE_CXXFLAGS += \
	-m64 \
	-msse -msse2 -msse3 -mssse3 -msse4 -msse4.1 -msse4.2 -mavx -mf16c \
	-I$$_PRO_FILE_PWD_/../boost \
	-I$$_PRO_FILE_PWD_/thirdparty/include/linux \
	-I$$_PRO_FILE_PWD_/platform/linux

CONFIG(debug, debug|release) {
	message("This is debug.")

	DEFINES += _DEBUG

} else {
	message("This is release.")

	DEFINES += NDEBUG

	QMAKE_CFLAGS_RELEASE -= -O0 -O1 -O2
	QMAKE_CFLAGS_RELEASE *= -O3

	QMAKE_CXXFLAGS_RELEASE -= -O0 -O1 -O2
	QMAKE_CXXFLAGS_RELEASE *= -O3
}

LIBS += \
	-L$$_PRO_FILE_PWD_/thirdparty/lib/linux \
	-Wl,--version-script=$$_PRO_FILE_PWD_/platform/linux/audioconvert.lds \
	-Wl,--unresolved-symbols=report-all

CONFIG(debug, debug|release) {
	LIBS += \
		-lflac_debug \
		-llame_debug \
		-lmpg123_debug \
		-lvorbis_debug \
		-logg_debug

} else {
	LIBS += \
		-lflac_release \
		-llame_release \
		-lmpg123_release \
		-lvorbis_release \
		-logg_release
}

SOURCES += \
    source/AudioCodecs/AudioFile.cpp \
    source/AudioCodecs/FlacDecoder.cpp \
    source/AudioCodecs/FlacEncoder.cpp \
    source/AudioCodecs/MpegDecoder.cpp \
    source/AudioCodecs/MpegEncoder.cpp \
    source/AudioCodecs/OpusDecoder.cpp \
    source/AudioCodecs/OpusEncoder.cpp \
    source/AudioCodecs/VorbisDecoder.cpp \
    source/AudioCodecs/VorbisEncoder.cpp \
    source/AudioCodecs/WaveDecoder.cpp \
    source/AudioCodecs/WaveEncoder.cpp \
    source/Auxiliary/FixedStream.cpp \
    source/Auxiliary/VectorStream.cpp \
    source/BusinessLogic/AudioConverter/AudioConverter.cpp \
    source/BusinessLogic/AudioConverter/AudioConverterAPI.cpp

HEADERS += ../Platform/linux/stdafx.h \
    source/AudioCodecs/AudioFile.h \
    source/AudioCodecs/AudioMetadata.h \
    source/AudioCodecs/AudioSink.h \
    source/AudioCodecs/FlacDecoder.h \
    source/AudioCodecs/FlacEncoder.h \
    source/AudioCodecs/MpegDecoder.h \
    source/AudioCodecs/MpegEncoder.h \
    source/AudioCodecs/OpusDecoder.h \
    source/AudioCodecs/OpusEncoder.h \
    source/AudioCodecs/VorbisDecoder.h \
    source/AudioCodecs/VorbisEncoder.h \
    source/AudioCodecs/WaveDecoder.h \
    source/AudioCodecs/WaveEncoder.h \
    source/Auxiliary/ChunkedStorage.h \
    source/Auxiliary/CompressedStorage.h \
    source/Auxiliary/FixedArray.h \
    source/Auxiliary/FixedStream.h \
    source/Auxiliary/IStream.h \
    source/Auxiliary/JsonPrinter.h \
    source/Auxiliary/Miscellaneous.h \
    source/Auxiliary/VectorStream.h \
    source/BusinessLogic/AudioConverter/AudioConverter.h \
    source/BusinessLogic/AudioConverter/AudioConverterAPI.h \
    platform/linux/stdafx.h

DISTFILES +=
