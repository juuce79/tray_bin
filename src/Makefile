CC = g++
LD = g++
WINDRES = windres

CFLAGS = -Wall -Wextra -std=c++17 -DUNICODE -D_UNICODE -I../include
LDFLAGS = -static -mwindows

BUILD_DIR = ../build
SRCS = TrayCycleBin.cpp \
       controllers/TrayCycleBinController.cpp \
       models/TrayCycleBinModel.cpp \
       views/TrayCycleBinView.cpp \
       utils/TrayCycleBinIconLoader.cpp

OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))
RC_OBJ = $(BUILD_DIR)/resource.o

TARGET = $(BUILD_DIR)/TrayCycleBin.exe

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJS) $(RC_OBJ)
	$(LD) $(LDFLAGS) -o $@ $^ -lshell32 -luser32 -lgdiplus -lole32

$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(RC_OBJ): ../resources/resource.rc
	$(WINDRES) -I../include -I../resources $< -O coff -o $@

package: $(TARGET)
	mkdir -p $(BUILD_DIR)/store_package/app
	cp $(TARGET) $(BUILD_DIR)/store_package/app/
	cp -r ../store_package/* $(BUILD_DIR)/store_package/
	MakeAppx pack -d $(BUILD_DIR)/store_package -p $(BUILD_DIR)/TrayCycleBin.msix

clean:
	rm -rf $(BUILD_DIR)
