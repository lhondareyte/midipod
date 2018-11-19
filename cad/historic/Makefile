CLEARANCE=3080
eps:
	pcb2eps midipod.pcb
clearance:
	setClearance logic.pcb $(CLEARANCE)
	setClearance keyboard.pcb $(CLEARANCE)
clean:
	@printf "Cleaning..."
	@rm -f *- *~
	@echo "done."
mrproper: 
	@printf "Cleaning..."
	@rm -f *- *~ *eps *bak
	@echo "done."
