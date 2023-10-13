# Defines rules for a C project.

CURRENT_BUILD_DIR := $(OUTPUT_DIRECTORY)/$(PROJECT_NAME)
OBJ := $(addprefix $(CURRENT_BUILD_DIR)/,$(SRC:.c=.o))

build: $(PROJECT_NAME)
$(PROJECT_NAME): $(CURRENT_BUILD_DIR)/$(TARGET_NAME)

$(CURRENT_BUILD_DIR)/$(TARGET_NAME): $(OBJ) $(PRECOMPILED_OBJ)
	@echo -e "   LD ($(PROJECT_NAME))\t$(TARGET_NAME)"
	@$(CC) $(GLOBAL_LDFLAGS) $(LDFLAGS) $(OBJ) $(PRECOMPILED_OBJ) -o $@

$(CURRENT_BUILD_DIR)/%.o: %.c
	@echo -e "   CC ($(PROJECT_NAME))\t$<"
	@mkdir -p $(dir $@)
	@$(CC) $(GLOBAL_CFLAGS) $(CFLAGS) -c $< -o $@
