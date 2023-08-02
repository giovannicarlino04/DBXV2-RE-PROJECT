CC = gcc
CXX = g++

OUTPUT := xinput1_3.dll
OBJS=main.o debug.o cpk_patch.o UtfLowLevel.o chara_patch.o  autogenportrait_dumper.o chara_patch_asm.o
OBJS += ../eternity_common/crypto/sha1.o ../eternity_common/crypto/md5.o ../eternity_common/crypto/rijndael.o
OBJS += ../eternity_common/Utils.o ../eternity_common/BaseFile.o ../eternity_common/IniFile.o  ../eternity_common/IggyFile.o ../eternity_common/Xv2PreBakedFile.o
OBJS += ../eternity_common/Stream.o ../eternity_common/MemoryStream.o ../eternity_common/FixedMemoryStream.o ../eternity_common/FileStream.o
OBJS += ../eternity_common/PatchUtils.o ../eternity_common/EpatchFile.o
OBJS += ../eternity_common/tinyxml/tinyxml.o ../eternity_common/tinyxml/tinystr.o ../eternity_common/tinyxml/tinyxmlerror.o ../eternity_common/tinyxml/tinyxmlparser.o
CFLAGS +=-Wall -I. -I../eternity_common -std=gnu99 -mms-bitfields -s -O2 -masm=intel -shared -Wl,--subsystem,windows,--kill-at,--enable-stdcall-fixup
CFLAGS += -static -static-libgcc -static-libstdc++
CFLAGS += -DXV2 
CXXFLAGS +=-Wall -Wno-strict-aliasing -I../eternity_common -O2 -std=c++11 -mms-bitfields -DTIXML_USE_STL
CXXFLAGS += -static-libgcc -static-libstdc++ -static -Wl,--subsystem,windows,--kill-at -fpermissive 
CPPFLAGS += -DXV2 -DNO_ZLIB 
LDFLAGS=-L. -static xinput.def
LIBS = -lstdc++ -lversion -lminhook

all: $(OUTPUT)	

clean:
	rm -f $(OUTPUT) *.o
	rm -f ../eternity_common/*.o
	rm -f ../eternity_common/tinyxml/*.o	
	rm -f ../eternity_common/crypto/*.o

$(OUTPUT): $(OBJS)
	$(LINK.c) $(LDFLAGS) -o $@ $^ $(LIBS)
#cp $(OUTPUT) ../ssssmins/bin/steam_api.dll
