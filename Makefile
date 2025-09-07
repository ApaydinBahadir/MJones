# -------- Proje ayarları --------
APP       := app
SRC_DIR   := src
BUILD_DIR := build
BIN_DIR   := bin

# SQLite amalgamation
SQLITE_DIR := third_party/sqlite
SQLITE_SRC := $(SQLITE_DIR)/sqlite3.c

CC        := cc
CSTD      := gnu17
WARN      := -Wall -Wextra -Wpedantic
OPT_DBG   := -O0 -g3
OPT_REL   := -O2

# POSIX/derleme bayrakları
CFLAGS    := -std=$(CSTD) $(WARN) -MMD -MP
LDFLAGS   :=
LDLIBS    := -lpthread -lm
# (LOAD_EXTENSION kullanacaksan) LDLIBS += -ldl

# SQLite bayrakları
CFLAGS    += -I$(SQLITE_DIR) -DSQLITE_THREADSAFE=1 -DSQLITE_OMIT_LOAD_EXTENSION -DSQLITE_DEFAULT_FOREIGN_KEYS=1
CFLAGS    += -DSQLITE_ENABLE_FTS5 -DSQLITE_ENABLE_JSON1 -DSQLITE_ENABLE_RTREE

# -------- Kaynaklar/Objeler --------
SRCS     := $(shell find $(SRC_DIR) -name '*.c') $(SQLITE_SRC)

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

debug release: $(BIN_DIR)/$(APP)

$(BIN_DIR)/$(APP): $(OBJS) | $(BIN_DIR)
	$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

# Derleme kuralları
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/sqlite/%.o: $(SQLITE_DIR)/%.c | $(BUILD_DIR)/sqlite
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR) $(BUILD_DIR)/sqlite $(BIN_DIR):
	mkdir -p $@

run: $(BIN_DIR)/$(APP)
	./$(BIN_DIR)/$(APP)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)/$(APP)

distclean: clean
	rm -rf $(BIN_DIR)

-include $(DEPS)
