#Environment Variable required: 
#  $(PROTOBUF_PATH)

CC        := g++
LD        := g++

BUILD_DIR := build

# Cygwin flags
CY_FLG    := -D_WIN32_WINNT=0x0501 -D__USE_W32_SOCKETS -D'__MSABI_LONG(x)=x'

# Protobuf resource compile
PB_DIR    := resource
PG_DIR    := src-gen
PB_SRC    := $(wildcard $(PB_DIR)/*.proto)
PB_SRC    := $(patsubst $(PB_DIR)/%.proto,$(PG_DIR)/%.cc,$(PB_SRC))
PB_LIB    := -ljson2pb -ljansson -lprotobuf -lpthread # !!has to be exact order like this!!

# main components
MC_DIR    := src nse store $(PG_DIR)
MC_SRC    := $(foreach sdir,$(MC_DIR),$(wildcard $(sdir)/*.cc))
MC_OBJ    := $(patsubst %.cc,build/%.o,$(MC_SRC))
MC_FLG    := -Wall -std=gnu++1y
MC_INC    := -Iinclude $(addprefix -I,$(MC_DIR))
MC_LIB    := -lboost_regex -lboost_filesystem -lboost_system -lboost_thread -L/usr/local/lib

# Elements i.e. CSE, AE, etc.
EL_DIR    := cse 
EL_SRC    := $(foreach sdir,$(EL_DIR),$(wildcard $(sdir)/*.cc))
EL_OBJ    := $(patsubst %.cc,build/%.o,$(EL_SRC))
EL_FIL    := %CSEMain.o  # objects filter out for gmock
EL_GMK    := $(filter-out $(EL_FIL),$(EL_OBJ))  # objects for gmock test
EL_FLG    := -Wall -std=gnu++1y
EL_INC    := $(addprefix -I,$(EL_DIR))
EL_LIB    := 

#gmock/gtest fused, common files
GF_DIR    := utest/fused utest/common
GF_SRC    := $(foreach sdir,$(GF_DIR),$(wildcard $(sdir)/*.cc))
GF_OBJ    := $(patsubst %.cc,build/%.o,$(GF_SRC))
GF_INC    := $(addprefix -I, $(GF_DIR))

# gtest
GT_DIR    := utest/gtest 
GT_SRC    := $(foreach sdir,$(GT_DIR),$(wildcard $(sdir)/*.cc))
GT_OBJ    := $(patsubst %.cc,build/%.o,$(GT_SRC))
GT_INC    := $(addprefix -I, $(GT_DIR))
GT_LIB    := 

# gmock NSEBase
GM_NSE_DIR     := utest/gmock utest/gmock/NSEBase_mock
GM_NSE_SRC     := $(foreach sdir,$(GM_NSE_DIR),$(wildcard $(sdir)/*.cc))
GM_NSE_OBJ     := $(patsubst %.cc,build/%.o,$(GM_NSE_SRC))
GM_NSE_INC     := $(addprefix -I, $(GM_NSE_DIR))
GM_NSE_LIB     := 

# gmock CoAPBinding
GM_COAP_DIR    := utest/gmock utest/gmock/CoAPInt_mock
GM_COAP_SRC    := $(foreach sdir,$(GM_COAP_DIR),$(wildcard $(sdir)/*.cc))
GM_COAP_OBJ    := $(patsubst %.cc,build/%.o,$(GM_COAP_SRC))
GM_COAP_INC    := $(addprefix -I, $(GM_COAP_DIR))
GM_COAP_LIB    := 

DIR       := $(MC_DIR) $(EL_DIR) $(GF_DIR) $(GT_DIR) $(GM_NSE_DIR) $(GM_COAP_DIR)
OBJ       := $(MC_OBJ) $(EL_OBJ) $(GF_OBJ) $(GT_OBJ) $(GM_NSE_OBJ) $(GM_COAP_OBJ)
FLG       := $(MC_FLG) $(CY_FLG)
INC       := $(MC_INC) $(EL_INC) $(GF_INC) $(GT_INC) $(GM_NSE_INC) $(GM_COAP_INC) 
LIB       := $(PB_LIB) $(MC_LIB) $(EL_LIB) $(GT_LIB) $(GM_NSE_LIB) $(GM_COAP_LIB) -lws2_32
 
.PHONY: all checkdirs clean gtest gmock_nse gmock_coap elem protoc 

all: checkdirs gtest elem gmock_nse gmock_coap

checkdirs: $(addprefix $(BUILD_DIR)/,$(DIR))

$(addprefix $(BUILD_DIR)/,$(DIR)):
	@mkdir -p $@
	
$(BUILD_DIR)/gtest.exe: $(MC_OBJ) $(GF_OBJ) $(GT_OBJ)
	$(LD) $^ $(LIB) -o $@

gtest: checkdirs $(BUILD_DIR)/gtest.exe
	@rm -rf $(BUILD_DIR)/data
	@cp -rf utest/data $(BUILD_DIR)
	#@cd $(BUILD_DIR) && ./gtest.exe
			
elem: checkdirs CSEMain

CSEMain: $(BUILD_DIR)/CSEMain.exe
	@echo
	
$(BUILD_DIR)/CSEMain.exe: $(MC_OBJ) $(EL_OBJ)
	$(LD) $^ $(LIB) -o $@
	

$(BUILD_DIR)/gmock_nse.exe: $(MC_OBJ) $(EL_GMK) $(GF_OBJ) $(GM_NSE_OBJ)
	$(LD) $^ $(LIB) -o $@
  
gmock_nse: checkdirs $(BUILD_DIR)/gmock_nse.exe
	@rm -rf $(BUILD_DIR)/data 
	@cp -rf utest/data $(BUILD_DIR)
	@cp $(BUILD_DIR)/data/CSEBase_Sample.res $(BUILD_DIR)/data/CSEBase.res
	#@cd $(BUILD_DIR) && ./gmock_nse.exe

$(BUILD_DIR)/gmock_coap.exe: $(MC_OBJ) $(EL_GMK) $(GF_OBJ) $(GM_COAP_OBJ)
	$(LD) $^ $(LIB) -o $@
  
gmock_coap: checkdirs $(BUILD_DIR)/gmock_coap.exe
	@rm -rf $(BUILD_DIR)/data 
	@cp -rf utest/data $(BUILD_DIR)
	@cp $(BUILD_DIR)/data/CSEBase_Sample.res $(BUILD_DIR)/data/CSEBase.res
	#@cd $(BUILD_DIR) && ./gmock_coap.exe

protoc: $(PB_SRC)

$(PG_DIR)/%.cc: $(PB_DIR)/%.proto
	protoc --proto_path=$(PB_DIR) --proto_path=$(PROTOBUF_PATH) --cpp_out=$(PG_DIR) $?
		
clean:
	@rm -rf $(BUILD_DIR) 
	#@rm $(G_DIR)/*

$(BUILD_DIR)/%.o: %.cc
	$(CC) $< $(FLG) $(INC) -g -c  -o $@
