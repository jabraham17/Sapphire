-include $(ROOT_PROJECT_DIRECTORY)options.mk
TARGET=$(LIB_DIRECTORY)libparser.a

TARGET_DEPENDS=$(GEN_DIRECTORY)parser/expr_parser_table.inc

-include $(ROOT_PROJECT_DIRECTORY)src/library.mk

$(TARGET_DEPENDS):
	@mkdir -p $(dir $(TARGET_DEPENDS))
	$(PYTHON3) $(SRC_PATH)build-table.py $(SRC_PATH)ExprParsing.ods >$@
