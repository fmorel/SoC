
test_self: $(TEST_BINARY)
	-./$(TEST_BINARY)  >&/dev/null

$(SIM_BINARY): $(PLATFORM_DESC)
	$(SOCLIB_CC) -P -p $(PLATFORM_DESC) $(SOCLIB_CC_ARGS) $(SOCLIB_CC_ADD_ARGS) -o $@
$(TEST_BINARY):
	$(SOCLIB_CC) -P -p $(TEST_DESC) $(SOCLIB_CC_ARGS) $(SOCLIB_CC_ADD_ARGS) -o $@

clean: soft_clean
	$(SOCLIB_CC) -P $(SOCLIB_CC_ARGS) $(SOCLIB_CC_ADD_ARGS) -x -o $(SIM_BINARY)
	rm -f tty.log
	rm -f vcd_traces.vcd
	rm -f $(OUTPUT_LOG)

test: test_submodules test_self 
test_submodules:
	-@for submodule in $(SUBMODULES); do $(MAKE) -C $$submodule test; done

