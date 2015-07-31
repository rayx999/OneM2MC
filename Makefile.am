#Environment Variable required: 
#  $(PROTOBUF_PATH)
#  $(GMOCK_INC) $(GMOCK_LIB)
#  $(GMOCK_GTEST_INC) $(GMOCK_GTEST_LIB)

CC        := g++
LD        := g++

BUILD_DIR := build

# Protobuf resource compile
PB_DIR    := resource
PG_DIR    := src-gen
PB_SRC    := $(wildcard $(PB_DIR)/*.proto)
PB_SRC    := $(patsubst $(PB_DIR)/%.proto,$(PG_DIR)/%.cc,$(PB_SRC))
PB_LIB    := -ljson2pb -ljansson -lprotobuf -lpthread # !!has to be exact order like this!!

# main components
MC_DIR    := src store $(PG_DIR)
MC_SRC    := $(foreach sdir,$(MC_DIR),$(wildcard $(sdir)/*.cc))
MC_OBJ    := $(patsubst %.cc,build/%.o,$(MC_SRC))
MC_FLG    := -Wall -D_WIN32_WINNT=0x0501 -D__USE_W32_SOCKETS -std=gnu++11
MC_INC    := -Iinclude $(addprefix -I,$(MC_DIR))
MC_LIB    := -lboost_iostreams -lboost_filesystem -lboost_system -L/usr/local/lib

# Elements i.e. CSE, AE, etc.
EL_DIR    := cse 
EL_SRC    := $(foreach sdir,$(EL_DIR),$(wildcard $(sdir)/*.cc))
EL_OBJ    := $(patsubst %.cc,build/%.o,$(EL_SRC))
EL_FIL    := %CSEMain.o  # objects filter out for gmock
EL_GMK    := $(filter-out $(EL_FIL),$(EL_OBJ))  # objects for gmock test
EL_FLG    := -Wall -D_WIN32_WINNT=0x0501 -D__USE_W32_SOCKETS -std=gnu++11
EL_INC    := $(addprefix -I,$(EL_DIR))
EL_LIB    := 

# gtest
GT_DIR    := utest/gtest
GT_SRC    := $(foreach sdir,$(GT_DIR),$(wildcard $(sdir)/*.cc))
GT_OBJ    := $(patsubst %.cc,build/%.o,$(GT_SRC))
GT_INC    := $(addprefix -I, $(GT_DIR) $(GMOCK_GTEST_INC))
GT_LIB    :=  -lgtest -L$(GMOCK_GTEST_LIB)

# gmock
GM_DIR    := utest/gmock
GM_SRC    := $(foreach sdir,$(GM_DIR),$(wildcard $(sdir)/*.cc))
GM_OBJ    := $(patsubst %.cc,build/%.o,$(GM_SRC))
GM_INC    := $(addprefix -I, $(GM_DIR) $(GMOCK_INC) $(GMOCK_GTEST_INC))
GM_LIB    := -lgmock  -lgtest -L$(GMOCK_LIB) -L$(GMOCK_GTEST_LIB)

DIR       := $(MC_DIR) $(EL_DIR) $(GT_DIR) $(GM_DIR)
OBJ       := $(MC_OBJ) $(EL_OBJ) $(GT_OBJ) $(GM_OBJ)
FLG       := $(MC_FLG)
INC       := $(MC_INC) $(EL_INC) $(GT_INC) $(GM_INC)
LIB       := $(PB_LIB) $(MC_LIB) $(EL_LIB) $(GT_LIB) $(GM_LIB) -lws2_32
 
.PHONY: all checkdirs clean gtest gmock elem protoc 

all: checkdirs gtest elem build/gmock.exe

checkdirs: $(addprefix $(BUILD_DIR)/,$(DIR))

$(addprefix $(BUILD_DIR)/,$(DIR)):
	@mkdir -p $@
	
$(BUILD_DIR)/gtest.exe: $(MC_OBJ) $(GT_OBJ)
	$(LD) $^ $(LIB) -o $@

gtest: checkdirs $(BUILD_DIR)/gtest.exe
	@rm -rf $(BUILD_DIR)/data
	@cp -rf utest/data $(BUILD_DIR)
	@cd $(BUILD_DIR) && ./gtest.exe
			
elem: checkdirs CSEMain

CSEMain: $(BUILD_DIR)/CSEMain.exe
	@echo
	
$(BUILD_DIR)/CSEMain.exe: $(MC_OBJ) $(EL_OBJ)
	$(LD) $^ $(LIB) -o $@
	

$(BUILD_DIR)/gmock.exe: $(MC_OBJ) $(EL_GMK) $(GM_OBJ)
	$(LD) $^ $(LIB) -o $@
  
gmock: checkdirs $(BUILD_DIR)/gmock.exe
	@rm -rf $(BUILD_DIR)/data 
	@cp -rf utest/data $(BUILD_DIR)
	#@cp $(BUILD_DIR)/data/CSEBase_Sample.res $(BUILD_DIR)/data/CSEBase.res
	#@cd $(BUILD_DIR) && ./gmock.exe

protoc: $(PB_SRC)

$(PG_DIR)/%.cc: $(PB_DIR)/%.proto
	protoc --proto_path=$(PB_DIR) --proto_path=$(PROTOBUF_PATH) --cpp_out=$(PG_DIR) $?
		
clean:
	@rm -rf $(BUILD_DIR) 
	#@rm $(G_DIR)/*

$(BUILD_DIR)/%.o: %.cc
	$(CC) $< $(FLG) $(INC) -g -c  -o $@
