# Defines rules for a C project.

OUT_DIR := $(OUTPUT_DIRECTORY)/$(PROJECT_NAME)
OBJ := $(addprefix $(OUT_DIR)/,$(SRC:.c=.o))

build: $(PROJECT_NAME)
$(PROJECT_NAME): $(OUT_DIR)/$(TARGET_NAME)

$(OUT_DIR)/$(TARGET_NAME): $(OBJ) $(PRECOMPILED_OBJ)
	@echo -e "     LD ($(PROJECT_NAME))\t$(TARGET_NAME)"
	@$(CC) $(GLOBAL_LDFLAGS) $(LDFLAGS) $(OBJ) $(PRECOMPILED_OBJ) -o $@

$(OUT_DIR)/%.o: %.c
	@echo -e "     CC ($(PROJECT_NAME))\t$<"
	@mkdir -p $(dir $@)
	@$(CC) $(GLOBAL_CFLAGS) -I$(shell pwd) $(CFLAGS) -c $< -o $@

-include $(OBJ:.o=.d)
