CC = g++
LD = g++
WINDRES = windres

CFLAGS = -Wall -Wextra -std=c++17 -DUNICODE -D_UNICODE -I../include
LDFLAGS = -static -mwindows -lshell32 -luser32

BUILD_DIR = ../build
SRCS = Main.cpp \
       controllers/TrayController.cpp \
       models/RecycleBinModel.cpp \
       views/TrayView.cpp

OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))
RC_OBJ = $(BUILD_DIR)/resource.o

TARGET = $(BUILD_DIR)/RecycleBinTray.exe

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS) $(RC_OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(RC_OBJ): ../resources/resource.rc
	$(WINDRES) -I../include -I../resources $< -O coff -o $@

clean:
	rm -rf $(BUILD_DIR)
