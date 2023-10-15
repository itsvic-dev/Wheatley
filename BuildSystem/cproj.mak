# Defines rules for a C project.

OUT_DIR := $(OUTPUT_DIRECTORY)/$(PROJECT_NAME)
OBJ := $(addprefix $(OUT_DIR)/,$(addsuffix .o,$(addprefix src/,$(SRC))))

build: $(PROJECT_NAME)
$(PROJECT_NAME): $(OUT_DIR)/$(TARGET_NAME)

$(OUT_DIR)/$(TARGET_NAME): $(OBJ) $(PRECOMPILED_OBJ)
	@echo -e "     LD ($(PROJECT_NAME))\t$(TARGET_NAME)"
	@$(CC) $(GLOBAL_LDFLAGS) $(LDFLAGS) $(OBJ) $(PRECOMPILED_OBJ) -o $@

$(OUT_DIR)/%.c.o: %.c
	@echo -e "     CC ($(PROJECT_NAME))\t$<"
	@mkdir -p $(dir $@)
	@$(CC) $(GLOBAL_CFLAGS) -I$(shell pwd)/inc $(CFLAGS) -c $< -o $@

$(OUT_DIR)/%.asm.o: %.asm
	@echo -e "     AS ($(PROJECT_NAME))\t$<"
	@mkdir -p $(dir $@)
	@$(AS) $(GLOBAL_ASFLAGS) $(ASFLAGS) -MD $(patsubst %.o,%.d,$@) -o $@ $<

-include $(OBJ:.o=.d)
