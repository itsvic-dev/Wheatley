# Defines rules for a C project.

CURRENT_BUILD_DIR := $(OUTPUT_DIRECTORY)/$(PROJECT_NAME)
OBJ := $(addprefix $(CURRENT_BUILD_DIR)/,$(SRC:.c=.o))

build: $(PROJECT_NAME)
$(PROJECT_NAME): $(CURRENT_BUILD_DIR)/.folder $(CURRENT_BUILD_DIR)/$(TARGET_NAME)

$(CURRENT_BUILD_DIR)/.folder:
	@echo -e "MKDIR\t($(PROJECT_NAME))"
	@mkdir -p $(CURRENT_BUILD_DIR)
	@touch $(CURRENT_BUILD_DIR)/.folder

$(CURRENT_BUILD_DIR)/$(TARGET_NAME): $(OBJ)
	@echo -e "LD\t($(PROJECT_NAME))\t$(TARGET_NAME)"
	@$(CC) $(GLOBAL_LDFLAGS) $(LDFLAGS) $(OBJ) -o $@

$(CURRENT_BUILD_DIR)/%.o: %.c
	@echo -e "CC\t($(PROJECT_NAME))\t$<"
	@$(CC) $(GLOBAL_CFLAGS) $(CFLAGS) -c $< -o $@
