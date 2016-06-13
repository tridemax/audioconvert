
CONFIG(debug, debug|release) {
	message("audioconvert_debug")

	TARGET = audioconvert_debug

	DESTDIR = $$_PRO_FILE_PWD_/../.dist
	OBJECTS_DIR = $$_PRO_FILE_PWD_/../.int/audioconvert_debug

} else {
	message("audioconvert_release")

	TARGET = audioconvert

	DESTDIR = $$_PRO_FILE_PWD_/../.dist
	OBJECTS_DIR = $$_PRO_FILE_PWD_/../.int/audioconvert_release
}

TEMPLATE = lib
CONFIG += sharedlib precompile_header unversioned_libname c++14
CONFIG -= qt

#-------------------------------------------------------------------------------------------------
# warnings
#-------------------------------------------------------------------------------------------------
QMAKE_CXXFLAGS_WARN_ON += \
	-Wno-parentheses \
	-Wno-unused-variable \
	-Wno-unused-parameter \
	-Wno-unused-local-typedefs \
	-Wno-unused-but-set-variable \
	-Wno-sign-compare \
	-Wno-unused-function \
	-Wno-switch

#-------------------------------------------------------------------------------------------------
# compiler flags
#-------------------------------------------------------------------------------------------------
QMAKE_CXXFLAGS += \
	-m64 \
	-msse -msse2 -msse3 -mssse3 -msse4 -msse4.1 -msse4.2 -mavx -mf16c \
	-fpic \
	-fdata-sections \
	-ffunction-sections \
	-I$$_PRO_FILE_PWD_/thirdparty/include/linux \
	-I$$_PRO_FILE_PWD_/platform/linux \
	-I$$_PRO_FILE_PWD_/../auxiliary \
	-I$$_PRO_FILE_PWD_/../boost

PRECOMPILED_HEADER = $$_PRO_FILE_PWD_/platform/linux/platform.h

CONFIG(debug, debug|release) {
	DEFINES += _DEBUG DEBUG

} else {
	DEFINES += NDEBUG

	QMAKE_CFLAGS_RELEASE -= -O0 -O1 -O2
	QMAKE_CFLAGS_RELEASE *= -O3

	QMAKE_CXXFLAGS_RELEASE -= -O0 -O1 -O2
	QMAKE_CXXFLAGS_RELEASE *= -O3
}

#-------------------------------------------------------------------------------------------------
# linker flags
#-------------------------------------------------------------------------------------------------
LIBS += \
	-L$$DESTDIR \
	-Wl,--version-script=$$_PRO_FILE_PWD_/platform/linux/audioconvert.lds \
	-Wl,--unresolved-symbols=report-all \
	-Wl,--gc-sections \
	-Wl,-rpath,./

CONFIG(debug, debug|release) {
	LIBS += \
		-lauxiliary_debug \
		-lflac_debug \
		-llame_debug \
		-lmpg123_debug \
		-lvorbis_debug \
		-logg_debug

} else {
	LIBS += \
		-lauxiliary \
		-lflac \
		-llame \
		-lmpg123 \
		-lvorbis \
		-logg \
		-O3
}

QMAKE_LFLAGS_RELEASE -= -Wl,-O0 -Wl,-O1 -Wl,-O2
QMAKE_LFLAGS_RELEASE *= -Wl,-O3

#-------------------------------------------------------------------------------------------------
# files
#-------------------------------------------------------------------------------------------------
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
    source/AudioConverter/AudioConverter.cpp \
    source/AudioConverter/AudioConverterAPI.cpp

HEADERS += \
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
    source/AudioConverter/AudioConverter.h \
    source/AudioConverter/AudioConverterAPI.h

DISTFILES += \
    platform/linux/audioconvert.lds

