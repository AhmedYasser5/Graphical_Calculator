empty :=
space := $(empty) $(empty)

SRCDIR := src
INCDIR := include
OBJDIR := build/obj
DEPDIR := build/deps
BINDIR := .

TARGET := $(BINDIR)/Graphical_Calculator_
ifeq ($(RELEASE), 1)
	TARGET := $(TARGET)Release
else
	TARGET := $(TARGET)Debug
endif
TARGET := $(TARGET).exe

MY_PATHS := $(BINDIR) $(INCDIR)
MY_FLAGS := 

###### extra variables #######
MY_PATHS += $(file <.my_paths)
MY_FLAGS += $(file <.my_flags)

###### complier set-up ######
CC = clang
CFLAGS = $(MY_FLAGS) -Wextra -Wno-unused-result -Wno-unused-command-line-argument
CXX = clang++
CXXFLAGS = $(CFLAGS) -std=c++17
LD = clang++
LDFLAGS = $(CXXFLAGS)
DEBUGGER = gdb

ifeq ($(RELEASE), 1)
	maketype := RELEASE
	CFLAGS += -O2 -ftree-vectorize -fomit-frame-pointer -march=native
	# Link Time Optimization
	CFLAGS += -flto
else
	maketype := DEBUG
	CFLAGS += -Og -ggdb2 -DDEBUG=1
	# Overflow protection
	CFLAGS += -D_FORTIFY_SOURCE=2 -fstack-protector-strong -fstack-clash-protection -fcf-protection
	CFLAGS += -Wl,-z,defs -Wl,-z,now -Wl,-z,relro
	CXXFLAGS += -D_GLIBCXX_ASSERTIONS
endif

CFLAGS += -MMD -MP $(foreach i,$(MY_PATHS),-I$(i))

SRCS := $(wildcard $(SRCDIR)/*.cpp)
SRCS += $(wildcard $(SRCDIR)/**/*.cpp)

OBJS := $(patsubst $(SRCDIR)/%,$(OBJDIR)/%.$(maketype).o,$(SRCS))

.PHONY: all
all : $(TARGET)

.PHONY: getTarget
getTarget :
	@echo $(TARGET)

.PHONY: run
run : $(TARGET)
	@echo --------------------------------------------------
	@\time -f "\n--------------------------------------------------\nElapsed Time: %e sec\nCPU Percentage: %P\n"\
		$(TARGET) $(ARGUMENTS)

.PHONY: init
init :
	-@rm -rf build $(wildcard *.exe)
	@mkdir -p $(SRCDIR) $(INCDIR) $(OBJDIR) $(DEPDIR)
	@for i in $(wildcard *.cpp) $(wildcard *.c); do mv ./$$i $(SRCDIR)/$$i; done
	@for i in $(wildcard *.hpp) $(wildcard *.h); do mv ./$$i $(INCDIR)/$$i; done
	@TMP="";\
		for i in $(MY_PATHS); do TMP="$${TMP}-I$$i\\n"; done;\
		echo -e "$$TMP" >| .clang_complete

$(TARGET) : $(OBJS)
	-@echo LD $(maketype) "ALL ->" $@ && \
		$(LD) -o $@ $(OBJS) $(LDFLAGS)

$(OBJDIR)/%.cpp.$(maketype).o : $(SRCDIR)/%.cpp
	@$(eval CUR_DEP := $(patsubst $(SRCDIR)/%,$(DEPDIR)/%.d,$<))
	@mkdir -p $(@D) $(dir $(CUR_DEP))
	-@echo CXX $(maketype) $< "->" $@ && \
		$(CXX) -c $< -o $@ -MF $(CUR_DEP) $(CXXFLAGS)

DEPS := $(patsubst $(SRCDIR)/%,$(DEPDIR)/%.d,$(SRCS))

.PHONY: clean
clean : 
	-$(RM) $(OBJS) $(DEPS) $(TARGET)

.PHONY: debug
debug : $(TARGET)
	$(DEBUGGER) $(TARGET)

-include $(DEPS)
