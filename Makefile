all:

pre-build: 
	mkdir -p obj

src_path=./src/
include_path=./include/
obj_path=./obj/

%.o: .$(src_path)%.cc
	g++ -c -O2 -Wall -I $(include_path) -o $@ -Wextra -pedantic $<

HEADERS_OBJS = $(obj_path)AES.o \
  		 					$(obj_path)GaloisField.o
HEADERS = $(include_path)AES.h \
					$(include_path)GaloisField.h
TARGET = aes
TARGET_OBJ = $(obj_path)main.o

header: $(HEADERS_OBJS)

$(HEADERS_OBJS): $(HEADERS)

$(TARGET_OBJ): $(HEADERS)

$(TARGET) : $(TARGET_OBJ) $(HEADERS_OBJS)
	g++ $^ -o $@

all: clean pre-build header $(TARGET)

clean:
	rm -rf obj