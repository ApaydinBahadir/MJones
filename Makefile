# -------- Proje ayarları --------
APP       := app
SRC_DIR   := src
BUILD_DIR := build
BIN_DIR   := bin

# SQLite amalgamation
SQLITE_DIR := third_party/sqlite
SQLITE_SRC := $(SQLITE_DIR)/sqlite3.c

# -------- Platform algılama --------
# Windows'ta make genelde OS=Windows_NT verir
ifeq ($(OS),Windows_NT)
  EXEEXT := .exe
else
  EXEEXT :=
endif

# -------- Derleyici/Bayraklar --------
CC        := gcc
CSTD      := gnu17
WARN      := -Wall -Wextra -Wpedantic
OPT_DBG   := -O0 -g3
OPT_REL   := -O2

# Bağımlılık dosyaları için
CFLAGS    := -std=$(CSTD) $(WARN) -MMD -MP
LDFLAGS   :=

# pthread/m ile ilgili: Windows (MinGW) tarafında ayrıca -lpthread vermeye gerek yok
ifeq ($(OS),Windows_NT)
  LDLIBS    := -lm
else
  CFLAGS    += -pthread
  LDFLAGS   += -pthread
  LDLIBS    := -lm -lpthread
endif
# (LOAD_EXTENSION kullanacaksan) Linux/macOS'ta: LDLIBS += -ldl

# SQLite bayrakları
CFLAGS    += -I$(SQLITE_DIR) -DSQLITE_THREADSAFE=1 -DSQLITE_OMIT_LOAD_EXTENSION -DSQLITE_DEFAULT_FOREIGN_KEYS=1
CFLAGS    += -DSQLITE_ENABLE_FTS5 -DSQLITE_ENABLE_JSON1 -DSQLITE_ENABLE_RTREE

# -------- Yardımcı makrolar (platform bağımlı komutlar) --------
# Dizin oluşturma
ifeq ($(OS),Windows_NT)
  # Windows cmd: forward slash'ları backslash yapalım
  define MKDIR
    if not exist "$(subst /,\,$(1))" mkdir "$(subst /,\,$(1))"
  endef
  RM_DIR  := rmdir /S /Q
  RM_FILE := del /Q
  NULLDEV := NUL
  RUN_BIN = $(BIN_DIR)\$(APP)$(EXEEXT)
else
  define MKDIR
    mkdir -p "$(1)"
  endef
  RM_DIR  := rm -rf
  RM_FILE := rm -f
  NULLDEV := /dev/null
  RUN_BIN = ./$(BIN_DIR)/$(APP)$(EXEEXT)
endif

# -------- Recursive wildcard (external 'find' yok) --------
# Kullanım: $(call rwildcard, base_dir, pattern)
rwildcard = $(wildcard $(1)$(2)) \
            $(foreach d,$(wildcard $(1)*/),$(call rwildcard,$(d),$(2)))

# -------- Kaynaklar/Objeler --------
SRCS     := $(call rwildcard,$(SRC_DIR)/,*.c) $(SQLITE_SRC)

# src/*.c -> build/*.o
OBJS_SRC := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(filter $(SRC_DIR)/%,$(SRCS)))
# sqlite3.c -> build/sqlite/sqlite3.o
OBJS_SQL := $(patsubst $(SQLITE_DIR)/%.c,$(BUILD_DIR)/sqlite/%.o,$(filter $(SQLITE_DIR)/%,$(SRCS)))

OBJS := $(OBJS_SRC) $(OBJS_SQL)
DEPS := $(OBJS:.o=.d)

# -------- Hedefler --------
.PHONY: all debug release run clean distclean

all: debug

debug:   CFLAGS += $(OPT_DBG)
release: CFLAGS += $(OPT_REL)

debug release: $(BIN_DIR)/$(APP)$(EXEEXT)

$(BIN_DIR)/$(APP)$(EXEEXT): $(OBJS) | $(BIN_DIR)
	$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

# Derleme kuralları
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@$(call MKDIR,$(dir $@))
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/sqlite/%.o: $(SQLITE_DIR)/%.c | $(BUILD_DIR)/sqlite
	@$(call MKDIR,$(dir $@))
	$(CC) $(CFLAGS) -c $< -o $@

# Dizin hedefleri
$(BUILD_DIR) $(BUILD_DIR)/sqlite $(BIN_DIR):
	@$(call MKDIR,$@)

run: $(BIN_DIR)/$(APP)$(EXEEXT)
	$(RUN_BIN)

clean:
	-$(RM_DIR) $(BUILD_DIR) 2>$(NULLDEV) || true
	-$(RM_FILE) "$(BIN_DIR)/$(APP)$(EXEEXT)" 2>$(NULLDEV) || true

distclean: clean
	-$(RM_DIR) $(BIN_DIR) 2>$(NULLDEV) || true

-include $(DEPS)
